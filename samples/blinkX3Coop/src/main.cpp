/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"

// ---------------------------------------------------------------------------

/*
 * Perform a series of short busy wait steps of 1 millisecond separated
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

#define MILLIS  (1000/3)

// tasks allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, MILLIS * 1);
TaskBlink task2("B", APP_CONFIG_LED2, MILLIS * 2);
TaskBlink task3("C", APP_CONFIG_LED3, MILLIS * 4);

// ---------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[] = "AVI blinkX3Coop v1.1";
#endif
