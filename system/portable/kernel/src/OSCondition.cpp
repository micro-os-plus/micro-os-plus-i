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

OSEventWaitReturn_t
OSCondition::wait(OSEvent_t event, bool doNotBlock)
{
  OSThread* pThread;
  pThread = OSScheduler::getThreadCurrent();

  OSEventWaitReturn_t ret;
  pThread->setEventWaitReturn(OSEventWaitReturn::OS_IMMEDIATELY);
  ret = OSEventWaitReturn::OS_IMMEDIATELY;

  for (;;)
    {
      bool doWait;
      doWait = false;

      bool isConditionSatistied;
      isConditionSatistied = false;

      // The purpose of this class is to avoid the race condition
      // that may occur between checking the condition and
      // entering eventWaitPrepare

      OSSchedulerLock::enter();
        {
          isConditionSatistied = !isConditionSatisfied();
          if (isConditionSatistied)
            {
              if (!doNotBlock)
                {
                  doWait = pThread->eventWaitPrepare(event);
                }
            }
        }
      OSSchedulerLock::exit();

      if (isConditionSatistied)
        {
          ret = pThread->getEventWaitReturn();
          // The return value was set by
          // - the above eventWaitPrepare()
          // - the previous eventWait()
          // - or at init time if the condition is satisfied from the beginning
          break;
        }

      if (doNotBlock)
        {
          ret = OSEventWaitReturn::OS_WOULD_BLOCK;
          break;
        }

      if (!doWait)
        {
          // ret was set by eventWaitPrepare
          ret = pThread->getEventWaitReturn();
          break;
        }

      if (isCancelled())
        {
          pThread->eventWaitClear();
          ret = OSEventWaitReturn::OS_CANCELLED;
          break;
        }

      // Perform the actual scheduler yield
      ret = pThread->eventWaitPerform();

      // When the control returns here, it might not be necessarily due to
      // the expected event, some special conditions must be checked
      // to prematurely terminate the wait

      if ((ret == OSEventWaitReturn::OS_CANCELLED) || (ret
          == OSEventWaitReturn::OS_TIMEOUT) || (ret
          == OSEventWaitReturn::OS_LOCKED))
        {
          // Prematurely terminate the wait on special conditions
          break;
        }

      // For other events, loop until the condition is met
    }

  return ret;
}

bool
OSCondition::isCancelled(void)
{
  return false;
}

#endif
