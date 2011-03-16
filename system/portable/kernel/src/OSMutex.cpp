/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSMUTEX)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSMutex.h"

OSMutex::OSMutex()
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
:
  m_waitingTasks(0, 0)
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutex()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_isAcquired = false;
  m_pOwnerTask = 0;
  m_event = (OSEvent_t) this;
  m_eventRet = OSEventWaitReturn::OS_NONE;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
OSMutex::OSMutex(OSTask* pWaitingTasksArray[], int waitingTasksArraySize)
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
:
  m_waitingTasks(pWaitingTasksArray, waitingTasksArraySize)
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutex()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_isAcquired = false;
  m_pOwnerTask = 0;
  m_event = (OSEvent_t) this;
  m_eventRet = OSEventWaitReturn::OS_NONE;
}
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

int
OSMutex::acquire(bool doNotBlock)
{
  m_eventRet = OSEventWaitReturn::OS_NONE;

  bool isAcquired;
  for (;;)
    {
      OSEventWaitReturn_t ret;
      bool doWait;
      doWait = false;
      OSScheduler::criticalEnter();
        {
          isAcquired = m_isAcquired;
          m_isAcquired = true;
          if (!isAcquired)
            {
              // then acquire for the current task
              m_pOwnerTask = OSScheduler::getTaskCurrent();
            }
          else
            {
              if (!doNotBlock)
                {
                  // if we are allowed, prepare to block
                  doWait = OSScheduler::eventWaitPrepare(m_event);
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
                  if (doWait)
                    {
                      // add current task to waiting list
                      m_waitingTasks.add(OSScheduler::getTaskCurrent());
                    }
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */
                }
            }
        }
      OSScheduler::criticalExit();

      if (!isAcquired)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_ACQUIRE)
          OSDeviceDebug::putString("OSMutex::acquire() OK");
          OSDeviceDebug::putNewLine();
#endif
          return OS_OK;
        }

      if (doNotBlock)
        return OS_WOULD_BLOCK;

      if (doWait)
        {
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
          OSScheduler::criticalEnter();
            {
              OSScheduler::eventWaitPerform();
              // remove current task from waiting list
              m_waitingTasks.remove(OSScheduler::getTaskCurrent());
            }
          OSScheduler::criticalExit();
#else
          OSScheduler::eventWaitPerform();
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */
        }
      ret = OSScheduler::getEventWaitReturn();

      if (ret != (OSEventWaitReturn_t) m_event && ret
          != OSEventWaitReturn::OS_ALL)
        {
          m_eventRet = ret;
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_ACQUIRE)
          OSDeviceDebug::putString("OSMutex::acquire() event=");
          OSDeviceDebug::putHex((unsigned short)ret);
          OSDeviceDebug::putNewLine();
#endif
          return OS_OTHER_EVENT;
        }
    }
}

int
OSMutex::release(OSTask * pTask)
{
  if (pTask == 0)
    pTask = OSScheduler::getTaskCurrent();

  if (pTask != m_pOwnerTask)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
      OSDeviceDebug::putString("OSMutex::release() not owner");
      OSDeviceDebug::putNewLine();
#endif
      return OS_NOT_OWNER;
    }

  bool isAcquired;
  OSScheduler::criticalEnter();
    {
      isAcquired = m_isAcquired;
      m_isAcquired = false;
      m_pOwnerTask = 0;
    }
  OSScheduler::criticalExit();

  if (!isAcquired)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
      OSDeviceDebug::putString("OSMutex::release() not acquired");
      OSDeviceDebug::putNewLine();
#endif
      return OS_NOT_ACQUIRED;
    }
  // notify all tasks waiting for this mutex
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
  if (m_waitingTasks.getSize() == 0)
    {
      // notify all existing tasks
      OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
    }
  else
    {
      // notify only tasks waiting, in the order of entering acquire()
      for (int i = 0; i < m_waitingTasks.getCount(); ++i)
        m_waitingTasks.getItem(i)->eventNotify(m_event,
            (OSEventWaitReturn_t) m_event);
    }
#else
  OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
#endif
  // give other tasks a chance to acquire the mutex
  OSScheduler::yield();

#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
  OSDeviceDebug::putString("OSMutex::release() OK");
  OSDeviceDebug::putNewLine();
#endif
  return OS_OK;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)

OSMutexWaitingTasks::OSMutexWaitingTasks(OSTask** pWaitingTasksArray,
    int waitingTasksArraySize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutexWaitingTasks()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
  m_pWaitingTasksArray = pWaitingTasksArray;
  m_waitingTasksArraySize = waitingTasksArraySize;
  m_waitingTasksCount = 0;
}

// Add task to the end of the array
OSReturn_t
OSMutexWaitingTasks::add(OSTask* pTask)
{
  if (m_waitingTasksCount < m_waitingTasksArraySize)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS_ADD)
      OSDeviceDebug::putString("OSMutexWaitingTasks.add() [");
      OSDeviceDebug::putDec(m_waitingTasksCount);
      OSDeviceDebug::putString("]=");
      OSDeviceDebug::putPtr(pTask);
      OSDeviceDebug::putNewLine();
#endif
      m_pWaitingTasksArray[m_waitingTasksCount++] = pTask;
      return OSReturn::OS_OK;
    }
  else
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS)
      OSDeviceDebug::putString("OSMutexWaitingTasks.add() not enough space");
      OSDeviceDebug::putNewLine();
#endif
      return OSReturn::OS_NOT_ENOUGH_SPACE;
    }
}

// Remove given task from the array
OSReturn_t
OSMutexWaitingTasks::remove(OSTask* pTask)
{
  int j;
  j = -1;
  for (int i = 0; i < (int)m_waitingTasksCount; ++i)
    if (m_pWaitingTasksArray[i] == pTask)
      j = i;

  if (j == -1)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS)
      OSDeviceDebug::putString("OSMutexWaitingTasks.remove() item not found ");
      OSDeviceDebug::putPtr(pTask);
      OSDeviceDebug::putNewLine();
#endif
      return OSReturn::OS_ITEM_NOT_FOUND;
    }

  for (; j < (int)m_waitingTasksCount - 1; ++j)
    m_pWaitingTasksArray[j] = m_pWaitingTasksArray[j + 1];

#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS_REMOVE)
  OSDeviceDebug::putString("OSMutexWaitingTasks.remove() [");
  OSDeviceDebug::putDec(m_waitingTasksCount);
  OSDeviceDebug::putString("]=");
  OSDeviceDebug::putPtr(pTask);
  OSDeviceDebug::putNewLine();
#endif
  --m_waitingTasksCount;

  return OSReturn::OS_OK;
}

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

#endif /* defined(OS_INCLUDE_OSMUTEX) */
