/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSMUTEX)

#include "portable/kernel/include/OSMutex.h"

OSMutex::OSMutex()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("OSMutex()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    m_isAcquired = false;
    m_pOwnerTask = 0;
    m_event = (OSEvent_t)this;
    m_eventRet = OSEventWaitReturn::OS_NONE;
  }

int OSMutex::acquire(bool noBlock)
  {
    m_eventRet = OSEventWaitReturn::OS_NONE;

    bool isAcquired;
    for (;;)
      {
        OSScheduler::criticalEnter();
          {
            isAcquired = m_isAcquired;
            m_isAcquired = true;
            if (!isAcquired)
              m_pOwnerTask = OSScheduler::getTaskCurrent();
          }
        OSScheduler::criticalExit();

        if (!isAcquired)
          {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_ACQUIRE)
            OSDeviceDebug::putString("OSMutex::acquire() OK");
            OSDeviceDebug::putNewLine();
#endif
            return OK;
          }

        if (noBlock)
          return ERROR_WOULD_BLOCK;

        OSEventWaitReturn_t ret;
        ret = OSScheduler::eventWait(m_event);
        if (ret != (OSEventWaitReturn_t)m_event && ret
            != OSEventWaitReturn::OS_ALL)
          {
            m_eventRet = ret;
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_ACQUIRE)
            OSDeviceDebug::putString("OSMutex::acquire() event=");
            OSDeviceDebug::putHex((unsigned short)ret);
            OSDeviceDebug::putNewLine();
#endif
            return ERROR_EVENT;
          }
      }
  }

int OSMutex::release(OSTask * pTask)
  {
    if (pTask == 0)
      pTask = OSScheduler::getTaskCurrent();

    if (pTask != m_pOwnerTask)
      {
#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
        OSDeviceDebug::putString("OSMutex::release() not owner");
        OSDeviceDebug::putNewLine();
#endif
        return ERROR_NOT_OWNER;
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
        return ERROR_NOT_ACQUIRED;
      }
    // notify all tasks waiting for this mutex
    OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t)m_event);
    // give other tasks a chance to acquire the mutex
    OSScheduler::yield();

#if defined(DEBUG) && defined(OS_DEBUG_OSMUTEX_RELEASE)
    OSDeviceDebug::putString("OSMutex::release() OK");
    OSDeviceDebug::putNewLine();
#endif
    return OK;
  }

#endif
