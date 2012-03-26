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

  // The purpose of this class is to avoid the race condition.
  // If the condition is satisfied the code returns, so there is no
  // problem any more, but if a wait is needed,
  // and eventNotify() is issued before entering eventWait()
  // the notification is lost and the wait will either be delayed
  // till the next event, or hang if it never comes again.

  // The solution is to perform the check and the eventWaitPrepare
  // within a critical region, so that the eventNotify() is guaranteed to
  // be issued after entering eventWait().

  for (;;)
    {
      // Prepare and pre-check condition

      ret = prepareCheckCondition();
      if (ret != OSReturn::OS_SHOULD_WAIT)
        {
          // The condition is already satisfied
          break;
        }

      // If the notification arrives before entering the critical section
      // again there is no problem, since the next check will detect the
      // condition as satisfied and quit.

      bool doWait;
      doWait = false;

      bool shouldWait;
      shouldWait = false;

      // Enter a critical section, eventNotify() sent from other threads
      // cannot occur.
      criticalSectionEnter();
        {
          ret = checkSynchronisedCondition();
          shouldWait = (ret == OSReturn::OS_SHOULD_WAIT);
          if (shouldWait)
            {
              if (!doNotBlock)
                {
                  doWait = pThread->eventWaitPrepareWhileLocked(event);
                }
            }
        }
      criticalSectionExit();

      if (ret == OSReturn::OS_SHOULD_RETRY)
        {
          // If the condition is not stable (checking the condition is not
          // possible) go to prepare it again.

          // Since we are in a loop, it's a good idea to be nice with the
          // other threads
          OSScheduler::yield();
          continue;
        }

      if (!shouldWait)
        {
          // If the condition was satisfied, arrange to return
          break;
        }

      // Here we know that the condition was not satisfied

      if (doNotBlock)
        {
          OSDeviceDebug::putString(" WOULD_BLOCK ");
          ret = OSReturn::OS_WOULD_BLOCK;
          break;
        }

      // In other words, did eventWaitPrepareWhileLocked() succeed?
      if (!doWait)
        {
          // If not, m_eventWaitReturn provides more details
          OSDeviceDebug::putString(" CANNOT_WAIT ");
          ret = OSReturn::OS_CANNOT_WAIT;
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

      // TODO: should we check more conditions (like OS_LOCKED)?

#if defined(DEBUG)

      // Regular event wait return values are the same as the event
      // For debug purposes, check and report other cases
      if (eventWaitReturn != event)
        {
          OSDeviceDebug::putString(" uXevent(");
          OSDeviceDebug::putHex(eventWaitReturn);
          OSDeviceDebug::putString(") ");
        }

#endif /* defined(DEBUG) */

      // Ignore all other events and loop until the condition is met
    }

  m_eventWaitReturn = pThread->getEventWaitReturn();

  return ret;
}

OSReturn_t
OSCondition::prepareCheckCondition(void)
{
  return OSReturn::OS_SHOULD_WAIT;
}

bool
OSCondition::isCancelled(void)
{
  return false;
}

void
OSCondition::criticalSectionEnter(void)
{
  OSSchedulerLock::enter();
}

void
OSCondition::criticalSectionExit(void)
{
  OSSchedulerLock::exit();
}

// ============================================================================

#if defined(OS_INCLUDE_OSREALTIME)

OSRealTimeCondition::OSRealTimeCondition()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSRealTimeCondition"), this);
}

OSRealTimeCondition::~OSRealTimeCondition()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSRealTimeCondition"), this);
}

void
OSRealTimeCondition::criticalSectionEnter(void)
{
  OSRealTimeCriticalSection::enter();
}

void
OSRealTimeCondition::criticalSectionExit(void)
{
  OSRealTimeCriticalSection::exit();
}

#endif /* defined(OS_INCLUDE_OSREALTIME) */

// ----------------------------------------------------------------------------

#endif
