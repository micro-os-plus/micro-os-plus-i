/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSMUTEX_H_
#define OSMUTEX_H_

#include "portable/kernel/include/OS.h"

class OSMutex
  {
public:
  OSMutex();

  static const int OK = 1;
  static const int ERROR_NOT_OWNER = -1;
  static const int ERROR_NOT_ACQUIRED = -2;
  static const int ERROR_EVENT = -3;
  static const int ERROR_WOULD_BLOCK = -4;

  // try to acquire mutex
  int acquire(bool noBlock = false);

  // release owned mutex
  int release(OSTask * pTask = 0);

  OSTask * getOwnerTask(void);

  OSEvent_t getEvent(void);
  void setEvent(OSEvent_t event);

  OSEventWaitReturn_t getEventReturn(void);

private:
  OSTask * m_pOwnerTask;
  bool m_isAcquired;
  OSEvent_t m_event;
  OSEventWaitReturn_t m_eventRet;
  };

inline OSTask * OSMutex::getOwnerTask(void)
  {
    return m_pOwnerTask;
  }

inline OSEvent_t OSMutex::getEvent(void)
  {
    return m_event;
  }

inline void OSMutex::setEvent(OSEvent_t event)
  {
    m_event = event;
  }

inline OSEventWaitReturn_t OSMutex::getEventReturn(void)
  {
    return m_eventRet;
  }

#endif /*OSMUTEX_H_ */
