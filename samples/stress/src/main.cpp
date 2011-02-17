/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskStress.h"

// ---------------------------------------------------------------------------

#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

// tasks allocated on static storage
TaskBlink taskA("A", APP_CONFIG_LED1, APP_CFGINT_BLINK_TICKS);
TaskStress task0("0", 0, 50, 1,50);
TaskStress task1("1", 0, 50, 1,50);
TaskStress task2("2", 0, 50, 1,50);
TaskStress task3("3", 30, 70, 1, 100);
#if true
TaskStress task4("4", 30, 70, 1, 100);
TaskStress task5("5", 30, 70, 1, 100);
TaskStress task6("6", 10, 90, 10, 200);
TaskStress task7("7", 10, 90, 10, 200);
TaskStress task8("8", 10, 90, 10, 200);
TaskStress task9("9", 10, 90, 10, 200);
#endif

// ---------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)

// artificially make the system timer interrupt longer, so that we
// increase the chance of having nested interrupts.

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
const char greeting[] = "AVI stress v1.1";
const char buildDateTime[] = "(" __DATE__ " " __TIME__ ")";
#endif
