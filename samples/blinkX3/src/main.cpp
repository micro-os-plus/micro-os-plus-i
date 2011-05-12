/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"

// ---------------------------------------------------------------------------

/*
 * This sample shows how to run three different instances of the
 * same thread code, with different parameters.
 *
 * Each instance will blink a different led, with different rates. For a
 * good visual experience, the rates will emulate a binary counter.
 *
 */

// ---------------------------------------------------------------------------

#define TICKS   (OS_CFGINT_TICK_RATE_HZ / 4)

// active objects allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, TICKS * 1);
TaskBlink task2("B", APP_CONFIG_LED2, TICKS * 2);
TaskBlink task3("C", APP_CONFIG_LED3, TICKS * 4);

// ---------------------------------------------------------------------------
