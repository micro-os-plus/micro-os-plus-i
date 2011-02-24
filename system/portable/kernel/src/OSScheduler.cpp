/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

// ----- Static Data ----------------------------------------------------------

// Global pointer, used in contextSave/Restore.
// It points to the address of the m_pStack field of the current running task.
// This approach is used in order to make code faster.
OSStack_t **g_ppCurrentStack;

OSTask *OSScheduler::ms_pTaskIdle;
OSTask *OSScheduler::ms_pTaskRunning;

bool OSScheduler::ms_isRunning;
bool OSScheduler::ms_isLocked;
bool OSScheduler::ms_isPreemptive;

unsigned char OSScheduler::ms_tasksCount;
// The +1 is here to reserve space for the idle task.
OSTask *OSScheduler::ms_tasks[OS_CFGINT_TASKS_TABLE_SIZE + 1];

#if !defined(OS_EXCLUDE_OSTIMER)
OSTimerTicks OSScheduler::timerTicks;
#endif

#if defined(OS_INCLUDE_OSTIMERSECONDS)
OSTimerSeconds OSScheduler::timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTASK_SLEEP)
bool OSScheduler::ms_allowDeepSleep;
#endif

unsigned char OSReadyList::ms_count;
OSTask *OSReadyList::ms_array[OS_CFGINT_TASKS_TABLE_SIZE + 1];

// ----------------------------------------------------------------------------

// runs before all constructors
void
OSScheduler::earlyInit(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OSScheduler::earlyInit()"));
  OSDeviceDebug::putNewLine();
#endif

  ms_isRunning = false;
  ms_isLocked = false;

#if !defined(OS_EXCLUDE_PREEMPTION)
  ms_isPreemptive = true;
#else
  ms_isPreemptive = false;
#endif

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  ms_allowDeepSleep = true;
#endif

  // pIdleTask = 0;      // will be set by IdleTask constructor

  // do not reset if not run before any constructor (<=.init6)
  // since task constructors can increment it
  ms_tasksCount = 0;
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
  // will have the same order as they were defined
  int i;
  for (i = 0; i < ms_tasksCount; ++i)
    {
      OSTask *pt;
      pt = ms_tasks[i];
      if (!pt->isSuspended())
        OSReadyList::insert(pt);

      if (pt->getPriority() == OSTask::IDLE_PRIORITY)
        setTaskIdle(pt);
    }

#if defined(DEBUG)
  OSReadyList::dump();
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
      ms_pTaskRunning = OSReadyList::getTop();
      // Prepare the global value with the pointer to the m_pStack.
      g_ppCurrentStack = &ms_pTaskRunning->m_pStack;

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
#if false
  OSScheduler::criticalEnter();
    {
      int i;
      for (i = 0; i < ms_tasksCount; ++i)
        {
          OSTask *pt;
          pt = ms_tasks[i];
          if (pt != 0)
            {
              // not-suspended waiting tasks are notified
              if ((pt->m_isWaiting) && (!pt->m_isSuspended))
                {
                  OSEvent_t ev;
                  ev = pt->m_event;
                  if (ev == OSEvent::OS_ALL || ev == event)
                    {
                      // wakeup tasks waiting for event or for 0
                      pt->m_isWaiting = false;
                      pt->m_event = OSEvent::OS_NONE; // no longer wait for it
                      pt->m_eventWaitReturn = ret;
#if defined(DEBUG)
                      if (ret == OSEventWaitReturn::OS_NONE)
                      OSDeviceDebug::putChar('^');
#endif
                      OSReadyList::insert(pt);

                      ++cnt;
                    }
                }
            }
        }
    }
  OSScheduler::criticalExit();
#else
  int i;
  for (i = 0; i < ms_tasksCount; ++i)
    {
      OSTask *pt;
      pt = ms_tasks[i];
      if (pt != 0)
        {
          cnt += pt->eventNotify(event, ret);
        }
    }
#endif
  // return the number of notified tasks
  return cnt;
}

// quick check if a context switch is necessary
// executed on interrupts to save a few cycles by not entering the
// context switch routine

bool
OSScheduler::isContextSwitchRequired()
{
  // should be used only here, in scheduler core routines!!!
  OS::WDTreset();

  bool bRequire;
  bRequire = (ms_isPreemptive || (ms_pTaskRunning == ms_pTaskIdle))
      && (OSReadyList::getCount() > 1);

  return bRequire;
}

// Perform the context switch operations.
// At exit the static variable ms_pTaskRunning points to the new
// running task and the global variable g_ppCurrentStack to the
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

  // should be used only here, in scheduler core routines!!!
  OS::WDTreset();

  // if scheduler is not locked, perform the context switch
  if (!ms_isLocked)
    {
      OSScheduler::criticalEnter();
        {
          // remove the running task from the ready list
          OSReadyList::remove(ms_pTaskRunning);

          // eventually reinsert it at the end of the list (round robin)
          if (!ms_pTaskRunning->m_isSuspended && !ms_pTaskRunning->m_isWaiting)
            OSReadyList::insert(ms_pTaskRunning);

          // select the running task from the top of the list
          ms_pTaskRunning = OSReadyList::getTop();
          // Prepare the global value with the pointer to the m_pStack.
          g_ppCurrentStack = &ms_pTaskRunning->m_pStack;
        }
      OSScheduler::criticalExit();
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
      //OSReadyList::dump();
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

// internal methods
unsigned char
OSScheduler::taskRegister(OSTask *pTask)
{
  unsigned char id;
  id = 0xFF;

  OSScheduler::criticalEnter();
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
        OSReadyList::insert(pTask); // insert task in ready list
    }
  OSScheduler::criticalExit();

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

// called from timer interrupt
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

// ==========================================================================

OSReadyList::OSReadyList()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSReadyList()="));
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

// insert order by increasing priority
void
OSReadyList::insert(OSTask *pTask)
{
  int i;
  i = find(pTask);
  if (i != -1)
    {
      return; // already in
    }

  unsigned int prio;
  prio = pTask->m_staticPriority;
  for (i = 0; i < ms_count; ++i)
    {
      // for identical priority, insert at the end
      if (prio > ms_array[i] ->m_staticPriority)
        break;
    }

  if (i < ms_count)
  // i is the place where to insert
    {
      int j;
      // shift right to make space
      for (j = ms_count; j > i; j--)
        {
          ms_array[j] = ms_array[j - 1];
        }
    }
  // i might also be == ms_count

  ms_array[i] = pTask;
  ms_count++;

#if defined(DEBUG) && defined(OS_DEBUG_OSREADYLIST_INSERT)
    {
      OSDeviceDebug::putString_P(PSTR("ins "));
      OSDeviceDebug::putString(pTask->getName());
      OSDeviceDebug::putNewLine();
    }
#endif
}

void
OSReadyList::remove(OSTask * pTask)
{
  if (pTask == OSScheduler::getTaskIdle())
    return; // do not remove the idle task

  int i;
  i = find(pTask);
  if (i == -1)
    {
      return; // task not found, nothing to remove
    }

  // move the list one step to the left
  for (; i < ms_count - 1; ++i)
    {
      ms_array[i] = ms_array[i + 1];
    }
  ms_count--;

#if defined(DEBUG) && defined(OS_DEBUG_OSREADYLIST_REMOVE)
    {
      OSDeviceDebug::putString_P(PSTR("rem "));
      OSDeviceDebug::putString(pTask->getName());
      OSDeviceDebug::putNewLine();
    }
#endif
}

int
OSReadyList::find(OSTask *pTask)
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
OSReadyList::dump(void)
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

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

