/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"

// ---------------------------------------------------------------------------

/*
 * Perform a series of short (1 millisecond) busy waits  separated
 * by yields.
 */

void
delayWithYield(schedTicks_t n)
{
  for (; n--;)
    {
      os.busyWaitMillis(1); // busy wait one millisecond
      os.sched.yield(); // release control to next task
    }
}

// ---------------------------------------------------------------------------

// Since all tasks share the processor, to compute the individual
// rate, divide the tick rate by the number of tasks
#define NTASKS  (3)
#define MILLIS  (OS_CFGINT_TICK_RATE_HZ/NTASKS)

// Tasks allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, MILLIS * 1);
TaskBlink task2("B", APP_CONFIG_LED2, MILLIS * 2);
TaskBlink task3("C", APP_CONFIG_LED3, MILLIS * 4);

// ---------------------------------------------------------------------------
