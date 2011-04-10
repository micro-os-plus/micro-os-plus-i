/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

// ----- OSSchedulerLock static variables -------------------------------------

unsigned char volatile OSSchedulerLock::ms_nestingLevel;

// ----- OSCriticalSection static variables -----------------------------------

// Used in contextSave/Restore.
OSStack_t volatile OSCriticalSection::ms_nestingLevel;

// ----- OSScheduler static variables -----------------------------------------

// Used in contextSave/Restore.
// It points to the address of the m_pStack field of the current running task.
// This approach is used in order to make code faster.
volatile OSStack_t** volatile OSScheduler::ms_ppCurrentStack;

OSTask* OSScheduler::ms_pTaskIdle;
OSTask* volatile OSScheduler::ms_pTaskRunning;

bool OSScheduler::ms_isRunning;
bool OSScheduler::ms_isPreemptive;

unsigned char OSScheduler::ms_tasksCount;
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
unsigned char OSScheduler::ms_notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

// The +1 is here to reserve space for the idle task.
OSTask *OSScheduler::ms_tasks[OS_CFGINT_TASKS_TABLE_SIZE + 1];

OSSchedulerLock OSScheduler::lock;
OSCriticalSection OSScheduler::critical;

#if !defined(OS_EXCLUDE_OSTIMER)
OSTimerTicks OSScheduler::timerTicks;
#endif

#if defined(OS_INCLUDE_OSTIMERSECONDS)
OSTimerSeconds OSScheduler::timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTASK_SLEEP)
bool OSScheduler::ms_allowDeepSleep;
#endif

// ----- OSActiveTasks static variables ---------------------------------------

unsigned char OSActiveTasks::ms_count;
OSTask *OSActiveTasks::ms_array[OS_CFGINT_TASKS_TABLE_SIZE + 1];

// ============================================================================

#if defined(DEBUG)
OSSchedulerLock::OSSchedulerLock()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSSchedulerLock()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}
#endif

// ============================================================================

#if defined(DEBUG)
OSCriticalSection::OSCriticalSection()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSCriticalSection()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}
#endif

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

// When we cannot use the stack, we no longer need to inline, so here are
// the usual routines to enter()/exit() critical sections.
void
OSCriticalSection::enter(void)
  {
#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();
    tmp |= (OS_CFGINT_OSCRITICALSECTION_MASK);
    OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
    OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

    ++ms_nestingLevel;
  }

void
OSCriticalSection::exit(void)
  {
    if ((ms_nestingLevel > 0) && (--ms_nestingLevel == 0))
      {
#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
        register OSStack_t tmp;

        tmp = OSCPUImpl::getInterruptsMask();
        tmp &= ~(OS_CFGINT_OSCRITICALSECTION_MASK);
        OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
        OSCPUImpl::interruptsEnable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
      }
  }

#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

// ============================================================================

// Runs before all constructors
void
OSScheduler::earlyInit(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OSScheduler::earlyInit()"));
  OSDeviceDebug::putNewLine();
#endif

  ms_isRunning = false;

#if !defined(OS_EXCLUDE_PREEMPTION)
  ms_isPreemptive = true;
#else
  ms_isPreemptive = false;
#endif

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  ms_allowDeepSleep = true;
#endif

  OSCriticalSection::ms_nestingLevel = 0;

  // pIdleTask = 0;      // will be set by IdleTask constructor

  // do not reset if not run before any constructor (<=.init6)
  // since task constructors can increment it
  ms_tasksCount = 0;

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  ms_notifyIndex = 0;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */
}

#if defined(DEBUG)
OSScheduler::OSScheduler()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSScheduler()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}
#endif

void
OSScheduler::start(void)
{
  // Insert all tasks in the ready list; equal priority tasks
  // will have the same order as they were defined.
  // Interrupts should be disabled here, no need for critical sections.
  int i;
  for (i = 0; i < ms_tasksCount; ++i)
    {
      OSTask *pt;
      pt = ms_tasks[i];
      if (!pt->isSuspended())
        OSActiveTasks::insert(pt);

      if (pt->getPriority() == OSTask::IDLE_PRIORITY)
        setTaskIdle(pt);
    }

#if defined(DEBUG)
  OSActiveTasks::dump();
#endif

  if (getTaskIdle() == 0)
    {
#if defined(DEBUG)
      OSDeviceDebug::putString_P(PSTR("No IDLE task"));
      OSDeviceDebug::putNewLine();
#endif
    }
  else
    {
      // Get the task with the highest priority.
      ms_pTaskRunning = OSActiveTasks::getTop();
      // Prepare the global value with the pointer to the m_pStack.
      OSScheduler::ms_ppCurrentStack
          = (volatile OSStack_t**) &ms_pTaskRunning->m_pStack;

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_START)
        {
          OSDeviceDebug::putString_P(PSTR("Start "));
          OSSchedulerImpl::dumpContextInfo(ms_pTaskRunning);

          OSDeviceDebug::putNewLine();
        }
#endif

#if !defined(OS_EXCLUDE_OSTIMER)
      timerTicks.init();
#endif

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
      timerSeconds.init();
#endif

      ms_isRunning = true;

      // Be sure we start with scheduler unlocked
      OSSchedulerLock::clear();

      // Interrupts will be enabled during context restore,
      // after popping the flags/status register,
      OSSchedulerImpl::start();
      // Execution will continue with the top-most task (guaranteed to exist,
      // since the idle task was already checked to be there).
    }

  // Should never get here.
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OSScheduler::start() failed, loop"));
  OSDeviceDebug::putNewLine();
#endif
  for (;;)
    ;
}

// Release the processor to the next ready task.
void
OSScheduler::yield(void)
{
  //ms_pTaskRunning->m_hasReturnValue = false;
  OSSchedulerImpl::yield();
}

// Sleep until the event occurs.
OSEventWaitReturn_t
OSScheduler::eventWait(OSEvent_t event)
{
  if (ms_pTaskRunning->eventWaitPrepare(event))
    {
      OSSchedulerImpl::yield();

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_EVENTWAIT)
      OSDeviceDebug::putString(" wk ");
#endif
    }
  return ms_pTaskRunning->m_eventWaitReturn;
}

// Wake all tasks waiting for the given event.
int
OSScheduler::eventNotify(OSEvent_t event, OSEventWaitReturn_t ret)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_EVENTNOTIFY)
  OSDeviceDebug::putString( "eventNotify(" );
  OSDeviceDebug::putHex((unsigned short)event);
  OSDeviceDebug::putString( "," );
  OSDeviceDebug::putHex((unsigned short)ret);
  OSDeviceDebug::putString( ")" );
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

  // Instead of always starting the notification from the first registered task,
  // the round-robin notification mechanism remembers the first notified task
  // and next time starts from the next one.

  int notifyIndex;
  int firstNotified = -1;
  notifyIndex = ms_notifyIndex;

#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  for (i = 0; i < ms_tasksCount; ++i)
    {
      OSTask *pt;
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
      pt = ms_tasks[notifyIndex];
#else
      pt = ms_tasks[i];
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

      if (pt != 0)
        {
          int r;
          r = pt->eventNotify(event, ret);
#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
          // Remember the first notified task index
          if (firstNotified == -1 && r != 0)
          firstNotified = notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

          cnt += r;
        }

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
      notifyIndex++;
      if (notifyIndex >= ms_tasksCount)
      notifyIndex = 0;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */
    }

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  OSCriticalSection::enter();
    {
      if (firstNotified > 0)
      ms_notifyIndex = firstNotified;

      ms_notifyIndex++;
      if (ms_notifyIndex >= ms_tasksCount)
        {
          ms_notifyIndex = 0;
        }
    }
  OSCriticalSection::exit();
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  // Return the number of notified tasks
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
  bRequire = (ms_isPreemptive || (ms_pTaskRunning == ms_pTaskIdle))
      && (OSActiveTasks::getCount() > 1);

  return bRequire;
}

// Perform the context switch operations.
// At exit the static variable ms_pTaskRunning points to the new
// running task and the global variable OSScheduler::ms_ppCurrentStack to the
// tasks context to be restored.

void
OSScheduler::performContextSwitch()
{
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
    {
      //if (ms_pTaskRunning->m_isPreempted)
      //  OSDeviceDebug::putChar('*');

      OSDeviceDebug::putChar('<');
      OSSchedulerImpl::dumpContextInfo(ms_pTaskRunning);
    }
#endif

  // Should be used only here, in scheduler core routines!!!
  OSCPU::watchdogReset();

  // If scheduler is not locked, perform the context switch
  if (!OSSchedulerLock::isSet())
    {
      OSCriticalSection::enter();
        {
          OSTask* pTask;
          pTask = ms_pTaskRunning;

          // Remove the running task from the ready list
          OSActiveTasks::remove(pTask);

          // Eventually reinsert it at the end of the list (round robin)
          if (!pTask->isSuspended() && !pTask->isWaiting())
            OSActiveTasks::insert(pTask);

          // Select the running task from the top of the list
          ms_pTaskRunning = OSActiveTasks::getTop();
          // Prepare the global variable with the pointer to the m_pStack.
          ms_ppCurrentStack = (volatile OSStack_t**) &ms_pTaskRunning->m_pStack;
        }
      OSCriticalSection::exit();
    }
  else
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
        {
          OSDeviceDebug::putChar('L');
        }
#endif
      // if scheduler is locked, return OS_LOCKED
      ms_pTaskRunning->m_eventWaitReturn = OSEventWaitReturn::OS_LOCKED;
    }

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH)
    {
      //OSActiveTasks::dump();
      OSDeviceDebug::putChar('>');
      OSSchedulerImpl::dumpContextInfo(ms_pTaskRunning);
      OSDeviceDebug::putNewLine();
    }
#endif
}

// Get the address of the task registered under 'id' in tasks array,
// or NULL if 'id' is too high.
OSTask *
OSScheduler::getTask(int id)
{
  if (id < ms_tasksCount)
    return ms_tasks[id];
  else
    return NULL;
}

// Internal methods
unsigned char
OSScheduler::taskRegister(OSTask *pTask)
{
  unsigned char id;
  id = 0xFF;

  OSCriticalSection::enter();
    {
      if (ms_tasksCount < (sizeof(ms_tasks) / sizeof(ms_tasks[0])))
        {
          id = ms_tasksCount;
          ms_tasks[ms_tasksCount++] = pTask;
        }
      else
        {
#if defined(DEBUG)
          OSDeviceDebug::putString_P(PSTR("task table full"));
          OSDeviceDebug::putNewLine();
#endif
        }

      // initial tasks are inserted in ready list at start()
      // later tasks should be inserted when constructed
      if (id != 0xFF && ms_isRunning)
        OSActiveTasks::insert(pTask); // insert task in ready list
    }
  OSCriticalSection::exit();

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_TASKREGISTER)
  if (false)
    {
      OSDeviceDebug::putString_P(PSTR("Register "));
      OSDeviceDebug::putDec((unsigned short) id);
      OSDeviceDebug::putNewLine();
    }
#endif

  return id;
}

// Called from the timer interrupt
// Warning: interrupts are enabled!

void
OSScheduler::interruptTick(void)
{
#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK) || defined(OS_INCLUDE_OSTASK_INTERRUPTION)

  int i;
  for ( i = 0; i < ms_tasksCount; ++i )
    {
      OSTask *pt;
      pt = ms_tasks[ i ];
#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK)
      pt->schedulerTick();
#endif /* OS_INCLUDE_OSTASK_SCHEDULERTICK */

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
      if (pt->isInterrupted())
      ISRcancelTask(pt);
#endif /* OS_INCLUDE_OSTASK_INTERRUPTION */
    }

#endif /* defined(OS_INCLUDE_OSTASK_SCHEDULERTICK) || defined(OS_INCLUDE_OSTASK_INTERRUPTION) */

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)
  OSApplicationImpl::interruptTick();
#endif /* OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK */
}

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)

// warning: not synchronised
void OSScheduler::ISRcancelTask(OSTask *pTask)
  {
    if (pTask->isWaiting())
      {
        OSEvent_t event;
        event = pTask->getEvent();

        // cancel all timer related events
        timerTicks.eventRemove(event);
        timerSeconds.eventRemove(event);

        // cancel current event
        OSScheduler::eventNotify(event, OSEventWaitReturn::OS_CANCELED);
      }
  }

#endif

// ============================================================================

OSActiveTasks::OSActiveTasks()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSActiveTasks()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  int i;
  for (i = 0; i < OS_CFGINT_TASKS_TABLE_SIZE + 1; ++i)
    {
      ms_array[i] = 0;
    }
  ms_count = 0;
}

// Insert tasks in order of decreasing priority.
// Must be called in a critical section.
void
OSActiveTasks::insert(OSTask *pTask)
{
  int i;

  // Check if already in
  i = find(pTask);
  if (i != -1)
    {
      return; // already in, we're done
    }

  unsigned int prio;
  prio = pTask->m_staticPriority;
  for (i = 0; i < ms_count; ++i)
    {
      // If tasks with identical priority exist, insert at the end
      // in other words, insert before task with higher priority.
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

  ms_array[i] = pTask;
  ms_count++;

#if defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_INSERT)
    {
      OSDeviceDebug::putString_P(PSTR("ins "));
      OSDeviceDebug::putString(pTask->getName());
      OSDeviceDebug::putNewLine();
    }
#endif
}

// Must be called in a critical section.
void
OSActiveTasks::remove(OSTask * pTask)
{
  if (pTask == OSScheduler::getTaskIdle())
    return; // do not remove the idle task

  int i;

  // Try to find the task in the active list
  i = find(pTask);
  if (i == -1)
    {
      return; // task not found, nothing to remove
    }

  // Remove the task by copying the list one step to the left
  for (; i < ms_count - 1; ++i)
    {
      ms_array[i] = ms_array[i + 1];
    }
  ms_count--;

#if defined(DEBUG) && defined(OS_DEBUG_OSACTIVETASKS_REMOVE)
    {
      OSDeviceDebug::putString_P(PSTR("rem "));
      OSDeviceDebug::putString(pTask->getName());
      OSDeviceDebug::putNewLine();
    }
#endif
}

// Return index of given task, or -1
int
OSActiveTasks::find(OSTask *pTask)
{
  int i;
  for (i = 0; i < ms_count; ++i)
    {
      if (ms_array[i] == pTask)
        return i;
    }

  return -1; // task not found
}

#if defined(DEBUG)
void
OSActiveTasks::dump(void)
{
  int i;
  OSDeviceDebug::putString_P(PSTR("Tasks: "));
  for (i = 0; i < ms_count; ++i)
    {
      OSDeviceDebug::putChar('\'');
      OSDeviceDebug::putString(ms_array[i]->getName());
      OSDeviceDebug::putChar('\'');
      OSDeviceDebug::putChar(' ');
    }
  OSDeviceDebug::putNewLine();
}
#endif

// ----------------------------------------------------------------------------

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

