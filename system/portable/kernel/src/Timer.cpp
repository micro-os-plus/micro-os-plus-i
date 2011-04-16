/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#if defined(OS_INCLUDE_TIMER)

//#include "portable/kernel/include/Timer.h"

#include "portable/kernel/include/OS.h"


Timer::Timer(OSTimer *pTimer)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("Timer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    m_event = OSEvent::OS_NONE;
    m_ret = OSEventWaitReturn::OS_VOID;
    m_ticks = 0;
    m_periodic = false;
    m_pTimer = pTimer;
  }

Timer::~Timer()
  {
    ;
  }

// virtual, may be set to trigger any events
void Timer::timerEvent(void)
  {
    OSScheduler::eventNotify(m_event, m_ret);
  }

// start timer (ticks must be non-zero)
void Timer::start(OSTimerTicks_t ticks, bool periodic)
  {
    m_ticks = ticks;
    m_periodic = periodic;

    m_pTimer->eventNotify(ticks, OSEvent::OS_CUSTOM_TIMER,
        (OSEventWaitReturn_t)this);
  }

// stop timer
void Timer::stop()
  {
    m_ticks = 0;
    m_periodic = false;
  }

// cancel timer waiting
void Timer::cancel(OSEventWaitReturn_t ret)
  {
    if (m_event != OSEvent::OS_NONE)
      OSScheduler::eventNotify(m_event, ret);
  }

// set event and return value
void Timer::eventSet(OSEvent_t event, OSEventWaitReturn_t ret)
  {
    m_event = event;
    m_ret = ret;
  }

// wait the configured event and return result
OSEventWaitReturn_t Timer::eventWait()
  {
    if (m_event != OSEvent::OS_NONE)
      return OSScheduler::eventWait(m_event);
    else
      return OSEventWaitReturn::OS_NONE;
  }

#endif /*OS_INCLUDE_TIMER*/

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
