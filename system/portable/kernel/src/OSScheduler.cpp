/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

// ----- OSSchedulerLock static variables -------------------------------------

uint8_t volatile OSSchedulerLock::ms_nestingLevel;

// ----- OSScheduler static variables -----------------------------------------

// Used in contextSave/Restore.
// It points to the address of the m_pStack field of the current running thread.
// This approach is used in order to make code faster.
volatile OSStack_t** volatile OSScheduler::ms_ppCurrentStack;

OSThread* OSScheduler::ms_pThreadIdle;
OSThread* volatile OSScheduler::ms_pThreadRunning;

bool OSScheduler::ms_isRunning;
bool volatile OSScheduler::ms_isPreemptive;

unsigned char volatile OSScheduler::ms_threadsCount;
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
unsigned char OSScheduler::ms_notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

// The +1 is here to reserve space for the idle thread.
OSThread* OSScheduler::ms_threads[OS_CFGINT_THREADS_ARRAY_SIZE + 1];

OSSchedulerLock OSScheduler::lock;
OSCriticalSection OSScheduler::critical;

#if !defined(OS_EXCLUDE_OSTIMER)
OSTimerTicks OSScheduler::timerTicks;
#endif /* !defined(OS_EXCLUDE_OSTIMER) */

#if defined(OS_INCLUDE_OSTIMERSECONDS)
OSTimerSeconds OSScheduler::timerSeconds;
#endif /* defined(OS_INCLUDE_OSTIMERSECONDS) */

#if defined(OS_INCLUDE_OSTIMERRTC)
OSTimerRtc OSScheduler::timerRtc;
#endif /* defined(OS_INCLUDE_OSTIMERRTC) */

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
bool OSScheduler::ms_allowDeepSleep;
#endif /* defined(OS_INCLUDE_OSTHREAD_SLEEP) */

// ----- OSActiveThreads static variables -------------------------------------

unsigned char volatile OSActiveThreads::ms_count;
OSThread* volatile OSActiveThreads::ms_array[OS_CFGINT_THREADS_ARRAY_SIZE + 1];

// ============================================================================

#if defined(DEBUG)
OSSchedulerLock::OSSchedulerLock()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSSchedulerLock"), this);
}
#endif /* defined(DEBUG) */

// ============================================================================

// Runs before all constructors
void
OSScheduler::earlyInit(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OSScheduler::earlyInit()"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

  ms_isRunning = false;

#if !defined(OS_EXCLUDE_PREEMPTION)
  ms_isPreemptive = true;
#else /* defined(OS_EXCLUDE_PREEMPTION) */
  ms_isPreemptive = false;
#endif /* !defined(OS_EXCLUDE_PREEMPTION) */

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  ms_allowDeepSleep = true;
#endif /* defined(OS_INCLUDE_OSTHREAD_SLEEP) */

  // pIdleThread = 0;      // will be set by IdleThread constructor
  // do not reset if not run before any constructor (<=.init6)
  // since thread constructors can increment it

  ms_threadsCount = 0;

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  ms_notifyIndex = 0;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */
}

#if defined(DEBUG)
OSScheduler::OSScheduler()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSScheduler"), this);
}
#endif /* defined(DEBUG) */

void
OSScheduler::start(void)
{
  // Insert all threads in the ready list; equal priority threads
  // will have the same order as they were defined.
  // Interrupts should be disabled here, no need for critical sections.
  int i;
  for (i = 0; i < ms_threadsCount; ++i)
    {
      OSThread* pt;
      pt = ms_threads[i];
      if (!pt->isSuspended())
        OSActiveThreads::insert(pt);

      if (pt->getPriority() == OSThread::IDLE_PRIORITY)
        setThreadIdle(pt);
    }

#if defined(DEBUG)
  OSActiveThreads::dump();
#endif /* defined(DEBUG) */

  if (getThreadIdle() == 0)
    {
#if defined(DEBUG)
      OSDeviceDebug::putString_P(PSTR("No IDLE thread"));
      OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
    }
  else
    {
      // Get the thread with the highest priority.
      ms_pThreadRunning = OSActiveThreads::getTop();
      // Prepare the global value with the pointer to the m_pStack.
      OSScheduler::ms_ppCurrentStack
          = (volatile OSStack_t**) &ms_pThreadRunning->m_pStack;

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_START)
      OSDeviceDebug::putString_P(PSTR("OSScheduler::start() "));
      OSSchedulerImpl::dumpContextInfo(ms_pThreadRunning);

      OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_START) */

#if !defined(OS_EXCLUDE_OSTIMER)
      timerTicks.init();
#endif /* !defined(OS_EXCLUDE_OSTIMER) */

#if defined(OS_INCLUDE_OSTIMERSECONDS)
      timerSeconds.init();
#endif /* defined(OS_INCLUDE_OSTIMERSECONDS) */

#if defined(OS_INCLUDE_OSTIMERRTC)
      timerRtc.init();
#endif /* defined(OS_INCLUDE_OSTIMERRTC) */

      ms_isRunning = true;

      // Be sure we start with scheduler unlocked
      OSSchedulerLock::clear();

      // Interrupts will be enabled during context restore,
      // after popping the flags/status register,
      OSSchedulerImpl::start();
      // Execution will continue with the top-most thread (guaranteed to exist,
      // since the idle thread was already checked to be there).
    }

  // Should never get here.
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OSScheduler::start() failed, loop"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

  for (;;)
    ;
}

// Release the processor to the next ready thread.
void
OSScheduler::yield(void)
{
  //ms_pThreadRunning->m_hasReturnValue = false;
  OSSchedulerImpl::yield();
}

// Sleep until the event occurs.
OSEventWaitReturn_t
OSScheduler::eventWait(OSEvent_t event)
{
  if (ms_pThreadRunning->eventWaitPrepare(event))
    {
      OSSchedulerImpl::yield();

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_EVENTWAIT)
      OSDeviceDebug::putString(" wk ");
#endif
    }
  return ms_pThreadRunning->m_eventWaitReturn;
}

// Wake all threads waiting for the given event.
int
OSScheduler::eventNotify(OSEvent_t event, OSEventWaitReturn_t ret)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_EVENTNOTIFY)
  OSDeviceDebug::putString("eventNotify(");
  OSDeviceDebug::putHex((unsigned short)event);
  OSDeviceDebug::putString(",");
  OSDeviceDebug::putHex((unsigned short)ret);
  OSDeviceDebug::putString(")");
  OSDeviceDebug::putNewLine();
#endif
#if false
  if (event == OSEvent::OS_NONE)
  return 0;
#endif

  int cnt;
  cnt = 0;

  int i;

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)

  // Instead of always starting the notification from the first registered thread,
  // the round-robin notification mechanism remembers the first notified thread
  // and next time starts from the next one.

  int notifyIndex;
  int firstNotified = -1;
  notifyIndex = ms_notifyIndex;

#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  for (i = 0; i < ms_threadsCount; ++i)
    {
      OSThread* pt;
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
      pt = ms_threads[notifyIndex];
#else /* !defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */
      pt = ms_threads[i];
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

      if (pt != 0)
        {
          int r;
          r = pt->eventNotify(event, ret);
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
          // Remember the first notified thread index
          if (firstNotified == -1 && r != 0)
          firstNotified = notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

          cnt += r;
        }

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
      notifyIndex++;
      if (notifyIndex >= ms_threadsCount)
      notifyIndex = 0;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */
    }

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  OSCriticalSection::enter();
    {
      if (firstNotified > 0)
      ms_notifyIndex = firstNotified;

      ms_notifyIndex++;
      if (ms_notifyIndex >= ms_threadsCount)
        {
          ms_notifyIndex = 0;
        }
    }
  OSCriticalSection::exit();
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  // Return the number of notified threads
  return cnt;
}

// Quick check if a context switch is necessary.
// It is executed on interrupts to save a few cycles by not entering the
// context switch routine.

bool
OSScheduler::isContextSwitchRequired()
{
  // should be used only here, in scheduler core routines!!!
  OSCPU::watchdogReset();

  bool bRequire;
  bRequire = (ms_isPreemptive || (ms_pThreadRunning == ms_pThreadIdle))
      && (OSActiveThreads::getCount() > 1);

  return bRequire;
}

// Perform the context switch operations.
// At exit the static variable ms_pThreadRunning points to the new
// running thread and the global variable OSScheduler::ms_ppCurrentStack to the
// threads context to be restored.

void
OSScheduler::performContextSwitch()
{
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
    {
      //if (ms_pThreadRunning->m_isPreempted)
      //  OSDeviceDebug::putChar('*');

      OSDeviceDebug::putChar('<');
      OSSchedulerImpl::dumpContextInfo(ms_pThreadRunning);
    }
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH) */

  // Should be used only here, in scheduler core routines!!!
  OSCPU::watchdogReset();

  // If scheduler is not locked, perform the context switch
  if (!OSSchedulerLock::isSet())
    {
      // The critical section is mandatory when running the scheduler
      // on multiple interrupt levels and also for the yield() code

      register OSStack_t tmp;
      tmp = OSCPUImpl::getInterruptsMask();
#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
      OSCPUImpl::setInterruptsMask(tmp | OS_CFGINT_OSCRITICALSECTION_MASK);
#else
      OSCPUImpl::interruptsDisable();
#endif
        {
          OSThread* pThread;
          pThread = ms_pThreadRunning;

          // Remove the running thread from the ready list
          OSActiveThreads::remove(pThread);

          // Eventually reinsert it at the end of the list (round robin)
          if (!pThread->isSuspended() && !pThread->isWaiting())
            OSActiveThreads::insert(pThread);

          // Select the running thread from the top of the list
          pThread = ms_pThreadRunning = OSActiveThreads::getTop();
          // Prepare the global variable with the pointer to the m_pStack.
          ms_ppCurrentStack = (volatile OSStack_t**) &pThread->m_pStack;
        }
      OSCPUImpl::setInterruptsMask(tmp);
    }
  else
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
      OSDeviceDebug::putChar('L');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH) */
      // if scheduler is locked, return OS_LOCKED
      ms_pThreadRunning->m_eventWaitReturn = OSEventWaitReturn::OS_LOCKED;
    }

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
  //OSActiveThreads::dump();
  OSDeviceDebug::putChar('>');
  OSSchedulerImpl::dumpContextInfo(ms_pThreadRunning);
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH) */
}

// Get the address of the thread registered under 'id' in threads array,
// or NULL if 'id' is too high.
OSThread*
OSScheduler::getThread(int id)
{
  if (id < ms_threadsCount)
    return ms_threads[id];
  else
    return (OSThread*) 0;
}

// Internal methods
unsigned char
OSScheduler::threadRegister(OSThread* pThread)
{
  unsigned char id;
  id = 0xFF;

  OSCriticalSection::enter();
    {
      if (ms_threadsCount < (sizeof(ms_threads) / sizeof(ms_threads[0])))
        {
          id = ms_threadsCount;
          ms_threads[ms_threadsCount++] = pThread;
        }
      else
        {
#if defined(DEBUG)
          OSDeviceDebug::putString_P(PSTR("thread table full"));
          OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
        }

      // Initial threads are inserted in the ready list at start()
      // later threads should be inserted when constructed
      if (id != 0xFF && ms_isRunning)
        OSActiveThreads::insert(pThread); // insert thread in ready list
    }
  OSCriticalSection::exit();

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_THREADREGISTER)
  if (false)
    {
      OSDeviceDebug::putString_P(PSTR("Register "));
      OSDeviceDebug::putDec((unsigned short) id);
      OSDeviceDebug::putNewLine();
    }
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_THREADREGISTER) */

  return id;
}

// Called from the timer interrupt
// Warning: interrupts are enabled!

void
OSScheduler::interruptTick(void)
{
#if defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK) || defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

  int i;
  for (i = 0; i < ms_threadsCount; ++i)
    {
      OSThread* pt;
      pt = ms_threads[i];
#if defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK)
      pt->schedulerTick();
#endif /* defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK) */

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)
      if (pt->isInterrupted())
      ISRcancelThread(pt);
#endif /* defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) */
    }

#endif /* defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK) || defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) */

#if defined(OS_INCLUDE_OSREALTIME)
  OSRealTime::interruptTick();
#endif /* defined(OS_INCLUDE_OSREALTIME) */

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)
  OSApplicationImpl::interruptTick();
#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */
}

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

// warning: not synchronised
void
OSScheduler::ISRcancelThread(OSThread* pThread)
  {
    if (pThread->isWaiting())
      {
        OSEvent_t event;
        event = pThread->getEvent();

        // cancel all timer related events
        timerTicks.eventRemove(event);
        timerSeconds.eventRemove(event);

        // cancel current event
        OSScheduler::eventNotify(event, OSEventWaitReturn::OS_CANCELLED);
      }
  }

#endif /* defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) */

// ============================================================================

OSActiveThreads::OSActiveThreads()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSActiveThreads"), this);

  int i;
  for (i = 0; i < OS_CFGINT_THREADS_ARRAY_SIZE + 1; ++i)
    {
      ms_array[i] = 0;
    }
  ms_count = 0;
}

// Insert threads in order of decreasing priority.
// Must be called in a critical section.
void
OSActiveThreads::insert(OSThread* pThread)
{
  int i;

  // Check if already in
  i = find(pThread);
  if (i != -1)
    {
      return; // already in, we're done
    }

  uint_t prio;
  prio = pThread->m_staticPriority;
  for (i = 0; i < ms_count; ++i)
    {
      // If threads with identical priority exist, insert at the end
      // in other words, insert before thread with higher priority.
      if (prio > ms_array[i]->m_staticPriority)
        break;
    }

  if (i < ms_count)
    {
      // i is the place where to insert

      int j;
      // shift array right to make space
      for (j = ms_count; j > i; j--)
        {
          ms_array[j] = ms_array[j - 1];
        }
    }

  // but i might also be == ms_count, i.e. we insert at the end

  ms_array[i] = pThread;
  ms_count++;

#if defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_INSERT)
    {
      OSDeviceDebug::putString_P(PSTR("ins "));
      OSDeviceDebug::putString(pThread->getName());
      OSDeviceDebug::putNewLine();
    }
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_INSERT) */
}

// Must be called in a critical section.
void
OSActiveThreads::remove(OSThread* pThread)
{
  if (pThread == OSScheduler::getThreadIdle())
    return; // do not remove the idle thread

  int i;

  // Try to find the thread in the active list
  i = find(pThread);
  if (i == -1)
    {
      return; // thread not found, nothing to remove
    }

  // Remove the thread by copying the list one step to the left
  for (; i < ms_count - 1; ++i)
    {
      ms_array[i] = ms_array[i + 1];
    }
  ms_count--;

#if defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_REMOVE)
    {
      OSDeviceDebug::putString_P(PSTR("rem "));
      OSDeviceDebug::putString(pThread->getName());
      OSDeviceDebug::putNewLine();
    }
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_REMOVE) */
}

// Return index of given thread, or -1
int
OSActiveThreads::find(OSThread* pThread)
{
  int i;
  for (i = 0; i < ms_count; ++i)
    {
      if (ms_array[i] == pThread)
        return i;
    }

  return -1; // thread not found
}

#if defined(DEBUG)
void
OSActiveThreads::dump(void)
{
  int i;
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR("Threads: "));
  for (i = 0; i < ms_count; ++i)
    {
      OSDeviceDebug::putChar('\'');
      OSDeviceDebug::putString(ms_array[i]->getName());
      OSDeviceDebug::putChar('\'');
      OSDeviceDebug::putChar(' ');
    }
  OSDeviceDebug::putNewLine();
}
#endif /* defined(DEBUG) */

// ----------------------------------------------------------------------------

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

