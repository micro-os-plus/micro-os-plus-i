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
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
:
  m_waitingThreads(0, 0)
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutex()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_isAcquired = false;
  m_pOwnerThread = 0;
  m_event = (OSEvent_t) this;
  m_eventRet = OSEventWaitReturn::OS_NONE;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
OSMutex::OSMutex(OSThread* pWaitingThreadsArray[], int waitingThreadsArraySize)
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
:
  m_waitingThreads(pWaitingThreadsArray, waitingThreadsArraySize)
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutex()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_isAcquired = false;
  m_pOwnerThread = 0;
  m_event = (OSEvent_t) this;
  m_eventRet = OSEventWaitReturn::OS_NONE;
}
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

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
      OSCriticalSection::enter();
        {
          isAcquired = m_isAcquired;
          m_isAcquired = true;
          if (!isAcquired)
            {
              // then acquire for the current thread
              m_pOwnerThread = OSScheduler::getThreadCurrent();
            }
          else
            {
              if (!doNotBlock)
                {
                  // if we are allowed, prepare to block
                  doWait = OSScheduler::eventWaitPrepare(m_event);
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
                  if (doWait)
                    {
                      // add current thread to waiting list
                      m_waitingThreads.add(OSScheduler::getThreadCurrent());
                    }
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */
                }
            }
        }
      OSCriticalSection::exit();

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
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
          OSCriticalSection::enter();
            {
              OSScheduler::eventWaitPerform();
              // remove current thread from waiting list
              m_waitingThreads.remove(OSScheduler::getThreadCurrent());
            }
          OSCriticalSection::exit();
#else
          OSScheduler::eventWaitPerform();
#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */
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
OSMutex::release(OSThread * pThread)
{
  if (pThread == 0)
    pThread = OSScheduler::getThreadCurrent();

  if (pThread != m_pOwnerThread)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
      OSDeviceDebug::putString("OSMutex::release() not owner");
      OSDeviceDebug::putNewLine();
#endif
      return OS_NOT_OWNER;
    }

  bool isAcquired;
  OSCriticalSection::enter();
    {
      isAcquired = m_isAcquired;
      m_isAcquired = false;
      m_pOwnerThread = 0;
    }
  OSCriticalSection::exit();

  if (!isAcquired)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
      OSDeviceDebug::putString("OSMutex::release() not acquired");
      OSDeviceDebug::putNewLine();
#endif
      return OS_NOT_ACQUIRED;
    }
  // notify all threads waiting for this mutex
#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)
  if (m_waitingThreads.getSize() == 0)
    {
      // notify all existing threads
      OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
    }
  else
    {
      // notify only threads waiting, in the order of entering acquire()
      for (int i = 0; i < m_waitingThreads.getCount(); ++i)
        m_waitingThreads.getItem(i)->eventNotify(m_event,
            (OSEventWaitReturn_t) m_event);
    }
#else
  OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
#endif
  // give other threads a chance to acquire the mutex
  OSScheduler::yield();

#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
  OSDeviceDebug::putString("OSMutex::release() OK");
  OSDeviceDebug::putNewLine();
#endif
  return OS_OK;
}

#if defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS)

OSMutexWaitingThreads::OSMutexWaitingThreads(OSThread** pWaitingThreadsArray,
    int waitingThreadsArraySize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSMutexWaitingThreads()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
  m_pWaitingThreadsArray = pWaitingThreadsArray;
  m_waitingThreadsArraySize = waitingThreadsArraySize;
  m_waitingThreadsCount = 0;
}

// Add thread to the end of the array
OSReturn_t
OSMutexWaitingThreads::add(OSThread* pThread)
{
  if (m_waitingThreadsCount < m_waitingThreadsArraySize)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS_ADD)
      OSDeviceDebug::putString("OSMutexWaitingThreads.add() [");
      OSDeviceDebug::putDec(m_waitingThreadsCount);
      OSDeviceDebug::putString("]=");
      OSDeviceDebug::putPtr(pThread);
      OSDeviceDebug::putNewLine();
#endif
      m_pWaitingThreadsArray[m_waitingThreadsCount++] = pThread;
      return OSReturn::OS_OK;
    }
  else
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS)
      OSDeviceDebug::putString("OSMutexWaitingThreads.add() not enough space");
      OSDeviceDebug::putNewLine();
#endif
      return OSReturn::OS_NOT_ENOUGH_SPACE;
    }
}

// Remove given thread from the array
OSReturn_t
OSMutexWaitingThreads::remove(OSThread* pThread)
{
  int j;
  j = -1;
  for (int i = 0; i < (int)m_waitingThreadsCount; ++i)
    if (m_pWaitingThreadsArray[i] == pThread)
      j = i;

  if (j == -1)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS)
      OSDeviceDebug::putString("OSMutexWaitingThreads.remove() item not found ");
      OSDeviceDebug::putPtr(pThread);
      OSDeviceDebug::putNewLine();
#endif
      return OSReturn::OS_ITEM_NOT_FOUND;
    }

  for (; j < (int)m_waitingThreadsCount - 1; ++j)
    m_pWaitingThreadsArray[j] = m_pWaitingThreadsArray[j + 1];

#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEXWAITINGTASKS_REMOVE)
  OSDeviceDebug::putString("OSMutexWaitingThreads.remove() [");
  OSDeviceDebug::putDec(m_waitingThreadsCount);
  OSDeviceDebug::putString("]=");
  OSDeviceDebug::putPtr(pThread);
  OSDeviceDebug::putNewLine();
#endif
  --m_waitingThreadsCount;

  return OSReturn::OS_OK;
}

#endif /* defined(OS_INCLUDE_OSMUTEX_WAITING_THREADS) */

#endif /* defined(OS_INCLUDE_OSMUTEX) */
