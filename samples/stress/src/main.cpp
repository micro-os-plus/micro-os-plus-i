/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"

#include "portable/active/include/StacksReporter.h"

#include "TaskStress.h"

#if defined(APP_INCLUDE_TASKBLINKREALTIME)
#include "TaskBlinkRealTime.h"
#endif

// ---------------------------------------------------------------------------

//#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

TaskBlink taskBlink("B", APP_CONFIG_LED1, APP_CFGINT_BLINK_TICKS);

#if !defined(APP_CFGINT_SIMULTANEOUS_THREADS)
#define APP_CFGINT_SIMULTANEOUS_THREADS 9
#endif

#if APP_CFGINT_SIMULTANEOUS_THREADS > 0
TaskStress task0("0", 0, 50, 1, 150);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 1
TaskStress task1("1", 0, 50, 1, 150);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 2
TaskStress task2("2", 0, 50, 1, 150);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 3
TaskStress task3("3", 30, 70, 1, 200);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 4
TaskStress task4("4", 500, 2500, 1, 200);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 5
TaskStress task5("5", 30, 70, 1, 100);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 6
TaskStress task6("6", 10, 90, 1, 200);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 7
TaskStress task7("7", 10, 90, 1, 400);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 8
TaskStress task8("8", 10, 90, 1, 600);
#if APP_CFGINT_SIMULTANEOUS_THREADS > 9
TaskStress task9("9", 10, 90, 1, 800);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif /* APP_CFGINT_SIMULTANEOUS_THREADS > 0 */

#if defined(APP_INCLUDE_TASKBLINKREALTIME)
TaskBlinkRealTime taskRT("RT", APP_CFGINT_TASKBLINKREALTIME_LEDBIT,
    APP_CFGINT_TASKBLINKREALTIME_TICKS);

bool g_flagNotify;
#endif

StacksReporter taskReporter("R", APP_CFGINT_DUMP_INTERVAL_SECONDS,
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
#else /* !defined(APP_INCLUDE_BUSY_WAIT) */
    for (int i = APP_CFGINT_NOTIFIES; --i;)
    os.sched.eventNotify(APP_EVENT_DUMMY);
#endif /* defined(APP_INCLUDE_BUSY_WAIT) */
#endif /* defined(APP_INCLUDE_LENGTHEN_SYSTICK) */

#if defined(APP_INCLUDE_TASKBLINKREALTIME)

#if defined(APP_INCLUDE_LOCAL_NOTIFY)

    if (g_flagNotify)
      {
        g_flagNotify = false;
        os.sched.eventNotify(APP_CFGINT_TASKBLINKREALTIME_EVENT);
      }

#endif /* defined(APP_INCLUDE_LOCAL_NOTIFY) */

#endif /* defined(APP_INCLUDE_TASKBLINKREALTIME) */

  }

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */
