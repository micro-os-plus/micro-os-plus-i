/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"
#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"
#include "TaskCli.h"

// ------------------------------------------------------------------------------
// Cli Tasks

//avr32::uc3::Mci mci;

avr32::uc3::MemoryCardMci card;

// USB device
DeviceCharacterUsb devUsb;

// active objects allocated on static storage
TaskCli taskCli("usb", devUsb, card);

// ------------------------------------------------------------------------------
// Blink Task

// active objects allocated on static storage
//TaskBlink taskBlink("blink");

// ----------------------------------------------------------------------------

