/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "Application.h"

// ----------------------------------------------------------------------------
// All application classes are here.

Application app;

// ----------------------------------------------------------------------------
// Blink Task

TaskBlink taskBlink("blink");

// ----------------------------------------------------------------------------
