/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#include "portable/kernel/include/OS.h"

OSTimer::OSTimer(OSTimerStruct_t* pArray, int size)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSTimer"), this);

  m_pArray = pArray;
  m_size = size;
  m_count = 0;

  m_ticks = 0;
}

// sleep the given ticks (non-zero)
// normally return OSEventWaitReturn::OS_VOID
// return OSEventWaitReturn::OS_IMMEDIATELY if no sleep
// return custom if cancelled
OSEventWaitReturn_t
OSTimer::sleep(OSTimerTicks_t ticks, OSEvent_t event)
{
  OSEventWaitReturn_t ret;

  if (ticks != 0)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_SLEEP)
      OSDeviceDebug::putString_P(PSTR("OSTimer::sleep("));
      OSDeviceDebug::putDec((unsigned short) ticks);
      OSDeviceDebug::putChar(')');
      //OSDeviceDebug::putNewLine();
#endif /* OS_DEBUG_OSTIMER_SLEEP */

#if true
      if (OSSchedulerLock::isSet())
        {
          OSTimerTicks_t begTicks;
          begTicks = getTicks();

          while ((getTicks() - begTicks) < ticks)
            {
              // Busy wait
              OSCPUImpl::watchdogReset();
            }

          ret = OSEventWaitReturn::OS_VOID;
          return ret;
        }
#endif

#if defined(DEBUG)
      OSTimerTicks_t begTicks;
      begTicks = getTicks();
#endif

      if (event == 0)
        {
          // set event on the current thread address, return void
          event = (OSEvent_t) (OSScheduler::getThreadCurrent());
        }

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

      if (OSScheduler::getThreadCurrent()->isInterrupted())
        {
          ret = OSEventWaitReturn::OS_CANCELLED;
        }
      else
#endif /* OS_INCLUDE_OSTHREAD_INTERRUPTION */
        {

#if true
          // Normally the event should not be there, but...
          eventRemove(event);

          bool doWait;
          ret = OSEventWaitReturn::OS_FAILED;
          OSCriticalSection::enter();
            {
              doWait = insert(ticks, event, (OSEventWaitReturn_t) event)
                  && OSScheduler::eventWaitPrepare(event);
            }
          OSCriticalSection::exit();
          if (doWait)
            {
              OSScheduler::eventWaitPerform();
            }
          ret = OSScheduler::getEventWaitReturn();

#if defined(DEBUG)
          if (ret != (OSEventWaitReturn_t) event)
            {
              OSDeviceDebug::putString_P(PSTR("OSTimer::sleep() ret="));
              OSDeviceDebug::putHex(ret);
              OSDeviceDebug::putNewLine();
            }
          else
            {
              ret = OSEventWaitReturn::OS_VOID;
            }

          OSTimerTicks_t durationTicks;
          durationTicks = getTicks() - begTicks;

          if (durationTicks != ticks)
            {
              OSDeviceDebug::putString_P(PSTR("OSTimer::sleep("));
              OSDeviceDebug::putDec(ticks);
              OSDeviceDebug::putString_P(PSTR(") took "));
              OSDeviceDebug::putDec(durationTicks);
              OSDeviceDebug::putChar(' ');
              OSDeviceDebug::putString(
                  OSScheduler::getThreadCurrent()->getName());
              OSDeviceDebug::putNewLine();
            }
#endif

#else
          OSCriticalSection::enter();
            {
              if (insert(ticks, event, OSEventWaitReturn::OS_VOID))
              ret = OSScheduler::eventWait(event);
              else
              ret = OSEventWaitReturn::OS_FAILED;
            }
          OSCriticalSection::exit();
#endif
        }
    }
  else
    ret = OSEventWaitReturn::OS_IMMEDIATELY;

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_SLEEP)
  OSDeviceDebug::putString_P(PSTR("OSTimer::sleep() ret="));
  OSDeviceDebug::putHex(ret);
  OSDeviceDebug::putNewLine();
#endif

  return ret;
}

// Warning: not yet interruptible

void
OSTimer::eventNotify(OSTimerTicks_t ticks, OSEvent_t event,
    OSEventWaitReturn_t ret)
{
  if (ticks != 0)
    {
      // If the event is already in, remove it
      eventRemove(event);

      OSCriticalSection::enter();
        {
          insert(ticks, event, ret);
        }
      OSCriticalSection::exit();
    }
}

int
OSTimer::eventRemove(OSEvent_t event)
{
  int ret;
  ret = 0;

  OSCriticalSection::enter();
    {
      int cnt;
      cnt = m_count;

      OSTimerStruct_t* p;
      p = m_pArray;

      int i;
      for (i = 0; i < cnt;)
        {
          if (event == p->event)
            {
              int ticks;
              ticks = p->ticks;

              remove(i);
#if defined(_DEBUG)
              OSDeviceDebug::putChar('-');
#endif
              ret++;
              cnt = m_count; // reload local variable
              if (cnt > 0)
                p->ticks += ticks;
            }
          else
            {
              ++i;
              ++p;
            }
        }
    }
  OSCriticalSection::exit();

  return ret;
}

int
OSTimer::find(OSEvent_t event)
{
  int cnt;
  cnt = m_count;

  OSTimerStruct_t* p;
  p = m_pArray;

  for (int i = 0; i < cnt; ++i, ++p)
    {
      if (event == p->event)
        {
          return i;
        }
    }

  return -1;
}

// Insert an entry in the timer list.
// WARNING: needs synchronisation
bool
OSTimer::insert(OSTimerTicks_t ticks, OSEvent_t event, OSEventWaitReturn_t ret)
{
  if (ticks == 0)
    {
#if defined(DEBUG)
      OSDeviceDebug::putString_P(PSTR("OSTimer::insert(0)"));
      OSDeviceDebug::putNewLine();
#endif
      return false;
    }

  int cnt;
  cnt = m_count;

  if (cnt == m_size)
    {
#if defined(DEBUG)
      OSDeviceDebug::putString_P(PSTR("OSTimer::insert() full "));
      OSDeviceDebug::putPtr(this);
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putString(OSScheduler::getThreadCurrent()->getName());
      OSDeviceDebug::putNewLine();

      OSTimerStruct_t* p;
      p = m_pArray;

      int i;
      i = 0;
      // find position where to insert
      for (i = 0; i < m_size; ++i, ++p)
        {
          OSDeviceDebug::putHex(p->event);
          OSDeviceDebug::putChar('/');
          OSDeviceDebug::putDec(p->ticks);
          OSDeviceDebug::putNewLine();
        }

#endif
      return false;
    }

  volatile OSTimerStruct_t* p;
  int i;

#if defined(DEBUG)

  p = m_pArray;

  for (i = 0; i < cnt; ++i, ++p)
    {
      if (p->event == event)
        {
          OSDeviceDebug::putString_P(PSTR("OSTimer::insert() already in "));
          OSDeviceDebug::putHex(event);
          OSDeviceDebug::putChar('/');
          OSDeviceDebug::putDec(ticks);
          OSDeviceDebug::putChar('/');
          OSDeviceDebug::putHex(ret);
          OSDeviceDebug::putChar(' ');
          OSDeviceDebug::putDec(p->ticks);
          OSDeviceDebug::putChar('/');
          OSDeviceDebug::putHex(p->u.ret);
          OSDeviceDebug::putNewLine();
        }
    }

#endif

  p = m_pArray;
  i = 0;
  if (cnt > 0)
    {
      // find position where to insert
      for (i = 0; i < cnt; ++i, ++p)
        {
          if (ticks < p->ticks)
            {
              p->ticks -= ticks; // adjust current
              break;
            }
          ticks -= p->ticks;
        }
      // return i = position where to insert,
      // may be count, i.e. not found, end of array

      // move right to make space
      OSTimerStruct_t* q;
      int j;
      for (j = cnt, q = &m_pArray[j]; j > i; j--, q--)
        {
          *q = *(q - 1);
        }
    }

  // fill in OSTimerStruct_t
  p->ticks = ticks;
  p->event = event;
  p->u.ret = ret;

  m_count++;

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INSERT)
  OSDeviceDebug::putString_P(PSTR("OSTimer::insert() "));
  OSDeviceDebug::putPtr(p);
  OSDeviceDebug::putNewLine();
#endif
  return true;
}

// Remove the i-th list entry.
// WARNING: needs synchronisation.
void
OSTimer::remove(int i)
{
  int cnt;
  cnt = m_count;
  if (cnt > 0)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
      OSDeviceDebug::putString("remove()");
      OSDeviceDebug::putNewLine();
#endif
      if (cnt > 1)
        {
          OSTimerStruct_t* p;
          p = &m_pArray[i];

          for (; i < cnt - 1; i++, p++)
            {
              *p = *(p + 1);
            }
        }
      m_count--;
    }
}

// called from the interrupt routine to notify time events
void
OSTimer::interruptTick(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
  OSDeviceDebug::putChar('[');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK) */

  m_ticks++;

  // decrement timer and eventually wake up related threads
  if (m_count > 0) // is there anything in the list?
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
      OSDeviceDebug::putChar('*');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK) */

      OSTimerStruct_t* p;
      p = m_pArray;

      if (--(p->ticks) == 0)
        {
          do
            {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
              OSDeviceDebug::putString("Tick() ");
              OSDeviceDebug::putPtr(p);
              OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK) */
              if (p->event == OSEvent::OS_CUSTOM_TIMER)
                {
                  Timer *pt;

                  pt = p->u.pTimer;
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
                  OSDeviceDebug::putString("Thread ");
                  OSDeviceDebug::putPtr(pt);
                  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK) */
                  // trigger timer event (run virtual method)
                  pt->timerEvent();
                  // remove top timer from the array
                  remove(0);

                  // if periodic, reinsert it
                  if (pt->isPeriodic())
                    insert(pt->getTicksStart(), OSEvent::OS_CUSTOM_TIMER,
                        (OSEventWaitReturn_t) pt);

                }
              else
                {
                  if (OSScheduler::eventNotify(p->event, p->u.ret) == 0)
                    {
                      // Missing task to notify
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK_MISSED_NOTIFY)
                      OSDeviceDebug::putChar('m');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK_MISSED_NOTIFY) */
                    }
                  // remove top timer from the array
                  remove(0);
                  // MANDATORY, removing it enters infinite loop
                }
            }
          while ((p->ticks == 0) && (m_count > 0));
        }
    }
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
  OSDeviceDebug::putChar(']');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK) */
}

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
