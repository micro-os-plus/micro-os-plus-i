/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

// ------------------------------------------------------------------------------
// Blink Task

// active objects allocated on static storage
TaskPitpalac taskPitpalac("pitpalac");
TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------
