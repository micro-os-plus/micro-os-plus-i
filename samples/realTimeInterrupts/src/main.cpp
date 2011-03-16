/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/tasks/include/TaskReportStacks.h"

#include "TaskBlink.h"
#include "TaskBlinkRealTime.h"

// ----------------------------------------------------------------------------

TaskBlink taskA("A", APP_CFGINT_TASKBLINK_LEDBIT, APP_CFGINT_TASKBLINK_TICKS);

TaskBlinkRealTime taskB("B", APP_CFGINT_TASKBLINKREALTIME_LEDBIT,
    APP_CFGINT_TASKBLINKREALTIME_TICKS);

TaskReportStacks taskR("R", APP_CFGINT_DUMP_INTERVAL_SECONDS,
    APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS,
    APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS);

// ----------------------------------------------------------------------------

bool g_flagNotify;

// ----------------------------------------------------------------------------

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
  if (g_flagNotify)
    {
      g_flagNotify = false;
      os.sched.eventNotify(APP_CFGINT_TASKBLINKREALTIME_EVENT);
    }
}

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */

// ----------------------------------------------------------------------------
