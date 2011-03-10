/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskStress.h"

// ---------------------------------------------------------------------------

// the mutex which protects the shared region
OSMutex mutex();

// tasks allocated on static storage

// the main task, used to output statistics
TaskBlink taskA("A", APP_CONFIG_LED1, APP_CFGINT_BLINK_TICKS);

// the tasks, which will attempt to access the protected region
TaskStress task0("0", 0);
//TaskStress task1("1", 1);
//TaskStress task2("2", 2);
//TaskStress task3("3", 3);
//TaskStress task4("4", 4);
//TaskStress task5("5", 5);
//TaskStress task6("6", 6);
//TaskStress task7("7", 7);
//TaskStress task8("8", 8);
//TaskStress task9("9", 9);

// ---------------------------------------------------------------------------
// global variables shared by all tasks

// the random global variable
unsigned int TaskStress::ms_rand;



// the actual protected region (basically a global counter)
int resourceValue;

// global array where each tasks increments its number of access of the resource
// it is used for debugging and live statistics
int resourceAccessNum[APP_CONFIG_TASKS_NUM_MAX];

// ---------------------------------------------------------------------------

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
