/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OS.h"

#if true

#include "portable/kernel/include/OSCondition.h"

OSCondition::OSCondition()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSCondition"), this);
}

OSCondition::~OSCondition()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSCondition"), this);
}

OSReturn_t
OSCondition::wait(OSEvent_t event, bool doNotBlock)
{
  OSThread* pThread;
  pThread = OSScheduler::getThreadCurrent();

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  pThread->setEventWaitReturn(OSEventWaitReturn::OS_IMMEDIATELY);

  for (;;)
    {
      bool doWait;
      doWait = false;

      bool shouldWait;
      shouldWait = false;

      // The purpose of this class is to avoid the race condition
      // that may occur between checking the condition and
      // entering eventWaitPrepare

      OSSchedulerLock::enter();
        {
          ret = checkSynchronisedCondition();
          shouldWait = (ret == OSReturn::OS_SHOULD_WAIT);
          if (shouldWait)
            {
              //OSDeviceDebug::putString(" shouldWait ");
              if (!doNotBlock)
                {
                  doWait = pThread->eventWaitPrepareWhileLocked(event);
                }
            }
        }
      OSSchedulerLock::exit();

      if (!shouldWait)
        {
          //OSDeviceDebug::putString(" done ");
          // IF the condition was satisfied, need to prepare the response
          m_eventWaitReturn = pThread->getEventWaitReturn();
          // The return value was set by
          // - the above eventWaitPrepare()
          // - the previous eventWait()
          // - or at init time if the condition is satisfied from the beginning
          break;
        }

      if (doNotBlock)
        {
          OSDeviceDebug::putString(" WOULD_BLOCK ");
          ret = OSReturn::OS_WOULD_BLOCK;
          break;
        }

      if (!doWait)
        {
          // return value was set by eventWaitPrepare
          break;
        }

      if (isCancelled())
        {
          pThread->eventWaitClear();
          OSDeviceDebug::putString(" CANCELLED ");
          ret = OSReturn::OS_CANCELLED;
          break;
        }

#if defined(OS_DEBUG_OSCONDITION_WAIT)
      OSDeviceDebug::putString(" wait(");
      OSDeviceDebug::putHex(event);
      OSDeviceDebug::putString(") ");
#endif /* defined(OS_DEBUG_OSCONDITION_WAIT) */

      // Perform the actual scheduler yield
      OSEventWaitReturn_t eventWaitReturn;
      eventWaitReturn = pThread->eventWaitPerform();

#if defined(OS_DEBUG_OSCONDITION_WAIT)
      OSDeviceDebug::putString(" eWR=");
      OSDeviceDebug::putHex(eventWaitReturn);
      OSDeviceDebug::putString(" ");
#endif /* defined(OS_DEBUG_OSCONDITION_WAIT) */

      // When the control returns here, it might not be necessarily due to
      // the expected event, some special conditions must be checked
      // to prematurely terminate the wait

      if ((eventWaitReturn == OSEventWaitReturn::OS_CANCELLED))
        {
          OSDeviceDebug::putString(" CANCELLED ");
          ret = OSReturn::OS_CANCELLED;
          break;
        }

      if (eventWaitReturn == OSEventWaitReturn::OS_TIMEOUT)
        {
          OSDeviceDebug::putString(" TIMEOUT ");
          ret = OSReturn::OS_TIMEOUT;
          break;
        }

#if false
      if (eventWaitReturn == OSEventWaitReturn::OS_LOCKED)
        {
          return OSReturn::OS_OK;
        }
#endif

      // For other events, loop until the condition is met
    }

  m_eventWaitReturn = pThread->getEventWaitReturn();

  return ret;
}

bool
OSCondition::isCancelled(void)
{
  return false;
}

#endif
