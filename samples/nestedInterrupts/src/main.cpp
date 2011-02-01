/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskBlinkNested.h"

// ---------------------------------------------------------------------------

#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

// tasks allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, TICKS * 1);
//TaskBlinkNested task2("B", APP_CONFIG_LED3, 100 /*TICKS * 4*/);

// ---------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[] = "AVI nestedInterrupts v1.1";
#endif
