/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink1.h"
#include "TaskBlink2.h"
#include "TaskBlink3.h"
#include "TaskBlink4.h"

// ---------------------------------------------------------------------------

/*
 * This sample shows how to run four different threads with some simple
 * form of inter-thread synchronization.
 *
 * Thread 1 will alternate between two types of busy wait and thread sleep.
 *
 * Thread 2 will do a true sleep, and notify thread 3 every second time.
 *
 * Thread 3 will wait for the thread 2 notification.
 *
 * Thread 4 will step between 4 states and toggle both preeption and another
 * flag to set the type of busy wait performed in thread 1.
 *
 * Each thread will blink a different led, with different rates. For a
 * good visual experience, the rates will emulate a binary counter.
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
        os.busyWaitMillis( 1); // busy wait one tick
        os.sched.yield(); // release control to next thread
      }
  }

// actie objects allocated on static storage
TaskBlink1 task1("A", APP_CONFIG_LED1, OS_CFGINT_TICK_RATE_HZ / 4);
TaskBlink2 task2("B", APP_CONFIG_LED2, OS_CFGINT_TICK_RATE_HZ / 2);
TaskBlink3 task3("C", APP_CONFIG_LED3);
TaskBlink4 task4("D", OS_CFGINT_TICK_RATE_HZ * 2, task1);

// ------------------------------------------------------------------------------
