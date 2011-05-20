/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSMUTEX_H_
#define OSMUTEX_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)

class OSMutexWaitingThreads
{
public:
  OSMutexWaitingThreads(OSThread** pWaitingThreadsArray, int waitingThreadsArraySize);

  OSReturn_t
  add(OSThread* pThread);
  OSReturn_t
  remove(OSThread* pThread);
  OSThread*
  getItem(int i);

  int
  getSize(void);
  int
  getCount(void);

private:
  OSThread** m_pWaitingThreadsArray;
  unsigned short m_waitingThreadsArraySize;
  unsigned short m_waitingThreadsCount;
};

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

// The mutual exclusion (mutex) inter-thread synchronisation mechanism.
class OSMutex
{
public:
  // Initialise the internal variables.
  OSMutex();
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
  OSMutex(OSThread** pWaitingThreadsArray, int waitingThreadsArraySize);
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

  // Constants ; all are used as result code for acquire and release methods.
  static const int OS_OK = 1;
  static const int OS_NOT_OWNER = -1;
  static const int OS_NOT_ACQUIRED = -2;
  static const int OS_OTHER_EVENT = -3;
  static const int OS_WOULD_BLOCK = -4;

  // try to acquire mutex
  // if doNotBlock is false, the thread is blocked until this mutex is released
  int
  acquire(bool doNotBlock = false);

  // release owned mutex
  int
  release(OSThread* pThread = 0);

  // return the thread owner of the mutex
  OSThread* 
  getOwnerThread(void) const;

  // return the event used for notification the acquire and release of the mutex
  OSEvent_t
  getEvent(void) const;

  // set the given event used for notification the acquire and release of the mutex
  void
  setEvent(OSEvent_t event);

  // return the event return value for notification
  OSEventWaitReturn_t
  getEventReturn(void) const;

private:
  // the thread which uses this mutex
  OSThread* m_pOwnerThread;

  // true if the mutex is currently acquired
  bool m_isAcquired;

  // the event used for notifying
  OSEvent_t m_event;

  // the event return value of the event used for notification
  OSEventWaitReturn_t m_eventRet;

#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
  OSMutexWaitingThreads m_waitingThreads;
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

};

inline OSThread* 
OSMutex::getOwnerThread(void) const
{
  return m_pOwnerThread;
}

inline OSEvent_t
OSMutex::getEvent(void) const
{
  return m_event;
}

inline void
OSMutex::setEvent(OSEvent_t event)
{
  m_event = event;
}

inline OSEventWaitReturn_t
OSMutex::getEventReturn(void) const
{
  return m_eventRet;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)

// Get the first thread in the array
inline OSThread*
OSMutexWaitingThreads::getItem(int i)
{
  return m_pWaitingThreadsArray[i];
}

inline int
OSMutexWaitingThreads::getSize(void)
{
  return m_waitingThreadsArraySize;
}

inline int
OSMutexWaitingThreads::getCount(void)
{
  return m_waitingThreadsCount;
}

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

#endif /* OSMUTEX_H_ */
