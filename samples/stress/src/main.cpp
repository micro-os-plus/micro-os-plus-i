/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskReportStacks.h"
#include "TaskStress.h"

#if defined(APP_INCLUDE_TASKBLINKREALTIME)
#include "TaskBlinkRealTime.h"
#endif

// ---------------------------------------------------------------------------

//#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

TaskBlink taskA("A", APP_CONFIG_LED1, APP_CFGINT_BLINK_TICKS);

TaskStress task0("0", 0, 50, 1, 150);
TaskStress task1("1", 0, 50, 1, 150);
TaskStress task2("2", 0, 50, 1, 150);
TaskStress task3("3", 30, 70, 1, 200);
#if true
TaskStress task4("4", 500, 2500, 1, 200);
TaskStress task5("5", 30, 70, 1, 100);
TaskStress task6("6", 10, 90, 1, 200);
TaskStress task7("7", 10, 90, 1, 400);
TaskStress task8("8", 10, 90, 1, 600);
TaskStress task9("9", 10, 90, 1, 800);
#endif

#if defined(APP_INCLUDE_TASKBLINKREALTIME)
TaskBlinkRealTime taskRt("B", APP_CFGINT_TASKBLINKREALTIME_LEDBIT,
    APP_CFGINT_TASKBLINKREALTIME_TICKS);
bool g_flagNotify;
#endif

TaskReportStacks taskR("R", APP_CFGINT_DUMP_INTERVAL_SECONDS,
    APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS,
    APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS);

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

#if defined(APP_INCLUDE_TASKBLINKREALTIME)

  if (g_flagNotify)
    {
      g_flagNotify = false;
      os.sched.eventNotify(APP_CFGINT_TASKBLINKREALTIME_EVENT);
    }

#endif

}

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */
