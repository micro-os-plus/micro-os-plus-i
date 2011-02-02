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
TaskBlinkNested task2("B", APP_CONFIG_LED2, TICKS * 1);


// ---------------------------------------------------------------------------

void
busyWaitMicros(unsigned int n)
{
  int i;
  // calibrate from OSC
  for (i = (OS_CFGLONG_SYSCLOCK_HZ / 1000 * n / OS_CFGINT_BUSYWAIT_CALIBRATION); i--;)
    {
      OSImpl::NOP();
    }
}

// artificially make the system timer interrupt longer, so that we
// increase the chance of having nested interrupts.

#define APP_INCLUDE_LENGTHEN_SYSTICK    1
#define APP_INCLUDE_BUSY_WAIT           1
#define APP_BUSY_PROCENTAGE             40
#define APP_EVENT_DUMMY                 0x1111
#define APP_CFGINT_NOTIFIES             100

void
OSSchedulerImpl::applicationInterruptTick(void)
{
#if defined(APP_INCLUDE_LENGTHEN_SYSTICK)
#if defined(APP_INCLUDE_BUSY_WAIT)
  busyWaitMicros(1000000/OS_CFGINT_TICK_RATE_HZ*APP_BUSY_PROCENTAGE/100);
#else
  for (int i = APP_CFGINT_NOTIFIES; --i;)
    os.sched.eventNotify(APP_EVENT_DUMMY);
#endif
#endif
}

// ---------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[] = "AVI nestedInterrupts v1.1";
#endif
