/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/devices/character/include/DeviceCharacterUsb.h"

#include "portable/tasks/include/TaskBlink.h"

#include "TaskCli.h"
#include "TaskTest.h"

// ----------------------------------------------------------------------------

// USB device
DeviceCharacterUsb devUsb0;

#if defined OS_INCLUDE_SIMPLECLI
// Cli Tasks
TaskCli taskCliUsb("usb", devUsb0);
#else
// Test Tasks
TaskTest taskTestUsb("test", devUsb0);
#endif

// ----------------------------------------------------------------------------
// Blink Task

TaskBlink taskBlink("blink");

// ----------------------------------------------------------------------------
