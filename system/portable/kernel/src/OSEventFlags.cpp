/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSEVENTFLAGS)

#include "portable/kernel/include/OSEventFlags.h"

OSEventFlags::OSEventFlags()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("OSEventFlags()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    m_flags = 0;
  }

/*
 * Set bits and send an internally identified event.
 * Return the new flags.
 */

OSEventFlagsBits_t OSEventFlags::notify(OSEventFlagsBits_t bits)
  {
    OSScheduler::criticalEnter();
      {
        m_flags |= bits;
      }
    OSScheduler::criticalExit();
    OSScheduler::eventNotify((OSEvent_t)this, (OSEventWaitReturn_t)this);

    return m_flags;
  }

/*
 * Clear bits.
 * Return the new flags.
 */

OSEventFlagsBits_t OSEventFlags::clear(OSEventFlagsBits_t bits)
  {
    OSScheduler::criticalEnter();
      {
        m_flags &= ~bits;
      }
    OSScheduler::criticalExit();

    return m_flags;
  }

/*
 * Loop until one of the requested bits is set. 
 * If any flag is already set, return immediately.
 * 
 * For timeouts please set timers to send events; this function will return
 * the timer event.
 */

OSReturn_t OSEventFlags::wait(OSEventFlagsBits_t bits, bool isStrict)
  {
    OSEventFlagsBits_t flags;
    OSScheduler::criticalEnter();
      {
        flags = m_flags;
      }
    OSScheduler::criticalExit();

    // if requested bits are already set, return immediately
    if (((flags & bits) != 0))
      return OSReturn::OS_IMMEDIATELY;

    OSEventWaitReturn_t ret;
    do
      {
        ret = OSScheduler::eventWait((OSEvent_t)this);

        OSScheduler::criticalEnter();
          {
            flags = m_flags;
          }
        OSScheduler::criticalExit();

        // Event could be notified by task interruption
        // in this case the event return is OS_CANCELED
        if (!isStrict && (ret != (OSEventWaitReturn_t)this))
          break;

      } while ((flags & bits) == 0);

    return OSReturn::OS_OK;
  }

#endif

