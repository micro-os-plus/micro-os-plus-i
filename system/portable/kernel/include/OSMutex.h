/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSMUTEX_H_
#define OSMUTEX_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)

class OSMutexWaitingTasks
{
public:
  OSMutexWaitingTasks(OSTask** pWaitingTasksArray, int waitingTasksArraySize);

  OSReturn_t
  add(OSTask* pTask);
  OSReturn_t
  remove(OSTask * pTask);
  OSTask*
  getItem(int i);

  int
  getSize(void);
  int
  getCount(void);

private:
  OSTask** m_pWaitingTasksArray;
  unsigned short m_waitingTasksArraySize;
  unsigned short m_waitingTasksCount;
};

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

// The mutual exclusion (mutex) inter-task synchronisation mechanism.
class OSMutex
{
public:
  // Initialise the internal variables.
  OSMutex();
#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
  OSMutex(OSTask** pWaitingTasksArray, int waitingTasksArraySize);
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

  // Constants ; all are used as result code for acquire and release methods.
  static const int OS_OK = 1;
  static const int OS_NOT_OWNER = -1;
  static const int OS_NOT_ACQUIRED = -2;
  static const int OS_OTHER_EVENT = -3;
  static const int OS_WOULD_BLOCK = -4;

  // try to acquire mutex
  // if doNotBlock is false, the task is blocked until this mutex is released
  int
  acquire(bool doNotBlock = false);

  // release owned mutex
  int
  release(OSTask * pTask = 0);

  // return the task owner of the mutex
  OSTask *
  getOwnerTask(void);

  // return the event used for notification the acquire and release of the mutex
  OSEvent_t
  getEvent(void);

  // set the given event used for notification the acquire and release of the mutex
  void
  setEvent(OSEvent_t event);

  // return the event return value for notification
  OSEventWaitReturn_t
  getEventReturn(void);

private:
  // the task which uses this mutex
  OSTask * m_pOwnerTask;

  // true if the mutex is currently acquired
  bool m_isAcquired;

  // the event used for notifying
  OSEvent_t m_event;

  // the event return value of the event used for notification
  OSEventWaitReturn_t m_eventRet;

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)
  OSMutexWaitingTasks m_waitingTasks;
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

};

inline OSTask *
OSMutex::getOwnerTask(void)
{
  return m_pOwnerTask;
}

inline OSEvent_t
OSMutex::getEvent(void)
{
  return m_event;
}

inline void
OSMutex::setEvent(OSEvent_t event)
{
  m_event = event;
}

inline OSEventWaitReturn_t
OSMutex::getEventReturn(void)
{
  return m_eventRet;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS)

// Get the first task in the array
inline OSTask*
OSMutexWaitingTasks::getItem(int i)
{
  return m_pWaitingTasksArray[i];
}

inline int
OSMutexWaitingTasks::getSize(void)
{
  return m_waitingTasksArraySize;
}

inline int
OSMutexWaitingTasks::getCount(void)
{
  return m_waitingTasksCount;
}

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_TASKS) */

#endif /* OSMUTEX_H_ */
