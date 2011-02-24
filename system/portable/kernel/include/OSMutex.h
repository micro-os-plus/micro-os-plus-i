/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSMUTEX_H_
#define OSMUTEX_H_

#include "portable/kernel/include/OS.h"

// The mutual exclusion (mutex) inter-task synchronisation mechanism.
class OSMutex
{
public:
  // Initialise the internal variables.
  OSMutex();

  // Constants

  // OK is commented out because it is already defined (as macro) in compiler.h
  static const int OS_OK = 1;
  static const int OS_NOT_OWNER = -1;
  static const int OS_NOT_ACQUIRED = -2;
  static const int OS_OTHER_EVENT = -3;
  static const int OS_WOULD_BLOCK = -4;

  // try to acquire mutex
  // if noBlock is false, the task is blocked until this mutex is released
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

#endif /* OSMUTEX_H_ */
