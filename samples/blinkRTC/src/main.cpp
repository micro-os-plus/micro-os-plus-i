/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskBlink.h"
#include "TaskBlinkSeconds.h"

// ---------------------------------------------------------------------------

#define TICKS  (OS_CFGINT_TICK_RATE_HZ)

// tasks allocated on static storage
TaskBlink task1("A", APP_CONFIG_LED1, TICKS * 1);
TaskBlinkSeconds task2("B", APP_CONFIG_LED2, 1);

// ---------------------------------------------------------------------------
