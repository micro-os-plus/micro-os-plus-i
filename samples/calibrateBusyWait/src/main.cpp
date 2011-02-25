/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink1.h"
#include "TaskBlink2.h"

// ---------------------------------------------------------------------------

/*
 * This sample is used to calibrate the busyWait constant.
 *
 * Task 1 will blink the first LED, using the system sleep.
 *
 * Task 2 will blink the second LED, using busyWait.
 *
 */

// ---------------------------------------------------------------------------

/*
 * Perform a series of short busy wait steps of 1 tick separated
 * by yields.
 */

void delayWithYield(schedTicks_t n)
  {
    for (; n--;)
      {
        os.busyWaitMillis(1); // busy wait one tick
        os.sched.yield(); // release control to next task
      }
  }

// tasks allocated on static storage
TaskBlink1 task1("A", APP_CONFIG_LED1, OS_CFGINT_TICK_RATE_HZ / 2);
TaskBlink2 task2("B", APP_CONFIG_LED2, OS_CFGINT_TICK_RATE_HZ / 2);

// ------------------------------------------------------------------------------

