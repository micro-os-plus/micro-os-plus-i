/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskBlinkNested.h"

// ---------------------------------------------------------------------------

#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

// tasks allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, TICKS * 1);
TaskBlinkNested task2("B", APP_CONFIG_LED2, TICKS * 9);


// ---------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)

// artificially make the system timer interrupt longer, so that we
// increase the chance of having nested interrupts.

#define APP_INCLUDE_LENGTHEN_SYSTICK    1
#define APP_INCLUDE_BUSY_WAIT           1
#define APP_BUSY_PROCENTAGE             40
#define APP_EVENT_DUMMY                 0x1111
#define APP_CFGINT_NOTIFIES             100

void
OSApplicationImpl::interruptTick(void)
{
#if defined(APP_INCLUDE_LENGTHEN_SYSTICK)
#if defined(APP_INCLUDE_BUSY_WAIT)
  OS::busyWaitMicros(1000000/OS_CFGINT_TICK_RATE_HZ*APP_BUSY_PROCENTAGE/100);
#else
  for (int i = APP_CFGINT_NOTIFIES; --i;)
    os.sched.eventNotify(APP_EVENT_DUMMY);
#endif
#endif
}

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */

// ---------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[] = "AVI nestedInterrupts v1.1";
#endif
