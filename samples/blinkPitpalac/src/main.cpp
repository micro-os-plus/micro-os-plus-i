/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

// ------------------------------------------------------------------------------
// Blink Task

// task allocated on static storage
TaskPitpalac taskPitpalac("pitpalac");
TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[]= "AVI blinkPitpalac v1.2";
#endif
