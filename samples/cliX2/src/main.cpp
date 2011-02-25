/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/devices/character/include/DeviceCharacterUsb.h"

#include "portable/tasks/include/TaskBlink.h"

#include "TaskCli.h"

// ------------------------------------------------------------------------------

// USB device
DeviceCharacterUsb devUsb0;

// Cli Tasks
TaskCli taskCliUsb("usb", devUsb0);

// ------------------------------------------------------------------------------
// Blink Task

TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------
