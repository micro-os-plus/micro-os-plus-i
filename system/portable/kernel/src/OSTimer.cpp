/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"
//#include "portable/kernel/include/OSTimer.h"

OSTimer::OSTimer(OSTimerStruct_t *pArray, int size)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OSTimer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif  /* OS_DEBUG_CONSTRUCTORS */
    m_pArray = pArray;
    m_size = size;
    m_count = 0;
  }

// sleep the given ticks (non-zero)
// normally return OSEventWaitReturn::OS_VOID
// return OSEventWaitReturn::OS_IMMEDIATELY if no sleep
// return custom if canceled
OSEventWaitReturn_t OSTimer::sleep(OSTimerTicks_t ticks, OSEvent_t event)
  {
    OSEventWaitReturn_t ret;
    if (ticks != 0)
      {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_SLEEP)
        OSDeviceDebug::putString_P(PSTR("sleep("));
        OSDeviceDebug::putDec((unsigned short) ticks);
        OSDeviceDebug::putChar(')');
        //OSDeviceDebug::putNewLine();
#endif /* OS_DEBUG_OSTIMER_SLEEP */

        if (event == 0)
          {
            // set event on the current task address, return void
            event = (OSEvent_t) (OSScheduler::getTaskCurrent());
          }

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)

        if (OSScheduler::getTaskCurrent()->isInterrupted())
          {
            ret = OSEventWaitReturn::OS_CANCELED;
          }
        else
#endif /* OS_INCLUDE_OSTASK_INTERRUPTION */
          {
            OSScheduler::criticalEnter();
              {
                insert(ticks, event, OSEventWaitReturn::OS_VOID);
              }
            OSScheduler::criticalExit();
            ret = OSScheduler::eventWait(event);
          }
      }
    else
      ret = OSEventWaitReturn::OS_IMMEDIATELY;

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_SLEEP)
    OSDeviceDebug::putString_P(PSTR(" ret="));
    OSDeviceDebug::putHex(ret);
    OSDeviceDebug::putNewLine();
#endif
    return ret;
  }

// Warning: not yet interruptible

void OSTimer::eventNotify(OSTimerTicks_t ticks, OSEvent_t event,
    OSEventWaitReturn_t ret)
  {
    if (ticks != 0)
      {
        OSScheduler::criticalEnter();
          {
            insert(ticks, event, ret);
          }
        OSScheduler::criticalExit();
      }
  }

int OSTimer::eventRemove(OSEvent_t event)
  {
    int ret;
    ret = 0;

    OSScheduler::criticalEnter();
      {
        int cnt;
        cnt = m_count;

        OSTimerStruct_t *p;
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
    OSScheduler::criticalExit();

    return ret;
  }

// insert an entry in the timer list
// WARNING: needs synchronization
void OSTimer::insert(OSTimerTicks_t ticks, OSEvent_t event,
    OSEventWaitReturn_t ret)
  {
    if (ticks == 0)
      {
#if defined(DEBUG)
        OSDeviceDebug::putString_P(PSTR("insert() ticks=0"));
        OSDeviceDebug::putNewLine();
#endif
        return;
      }

    int cnt;
    cnt = m_count;

    if (cnt == m_size)
      {
#if defined(DEBUG)
        OSDeviceDebug::putString_P(PSTR("insert() full"));
        OSDeviceDebug::putNewLine();
#endif
        return;
      }

    OSTimerStruct_t *p;
    p = m_pArray;

    int i;
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
        OSTimerStruct_t *q;
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
    OSDeviceDebug::putString_P(PSTR("timer insert() "));
    OSDeviceDebug::putPtr(p);
    OSDeviceDebug::putNewLine();
#endif
  }

// remove the i-th list entry
// WARNING: needs synchronization
void OSTimer::remove(int i)
  {
    int cnt;
    cnt = m_count;
    if (cnt > 0)
      {
        if (cnt > 1)
          {
            OSTimerStruct_t *p;
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
void OSTimer::interruptTick(void)
  {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
    OSDeviceDebug::putChar('[');
#endif
    // decrement timer and eventually wake up related tasks
    if (m_count > 0) // is there anything in the list?
      {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
        OSDeviceDebug::putChar('*');
#endif
        OSTimerStruct_t *p;
        p = m_pArray;

        if (--(p->ticks) == 0)
          {
            do
              {
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
                OSDeviceDebug::putString("Tick() ");
                OSDeviceDebug::putPtr(p);
                OSDeviceDebug::putNewLine();
#endif
                if (p->event == OSEvent::OS_CUSTOM_TIMER)
                  {
                    Timer *pt;

                    pt = p->u.pTimer;
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
                    OSDeviceDebug::putString("Task ");
                    OSDeviceDebug::putPtr(pt);
                    OSDeviceDebug::putNewLine();
#endif
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
                    OSScheduler::eventNotify(p->event, p->u.ret);
                    // remove top timer from the array
                    remove(0);
                  }

              } while ((p->ticks == 0) && (m_count > 0));
          }
      }
#if defined(DEBUG) && defined(OS_DEBUG_OSTIMER_INTERRUPTTICK)
    OSDeviceDebug::putChar(']');
#endif
  }

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
