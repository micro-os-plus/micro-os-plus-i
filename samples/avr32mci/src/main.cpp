/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#if defined(APP_INCLUDE_MEMORYCARDSPI)
#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardSpi.h"
#else
//#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"
#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"
#endif /* defined(APP_INCLUDE_MEMORYCARDSPI) */

#include "TaskCli.h"

// ----------------------------------------------------------------------------
// Cli Tasks

//avr32::uc3::Mci mci;

#if defined(APP_INCLUDE_MEMORYCARDSPI)
avr32::uc3::MemoryCardSpi card(avr32::uc3::spi::MODULE_1, 41);
#else
avr32::uc3::MemoryCardMci card(avr32::uc3::mci::CardSlot::B, 19);
#endif /* defined(APP_INCLUDE_MEMORYCARDSPI) */

// USB device
DeviceCharacterUsb devUsb;

// active objects allocated on static storage
TaskCli taskCli("usb", devUsb, card);

// ----------------------------------------------------------------------------
// Blink Task

// active objects allocated on static storage
//TaskBlink taskBlink("blink");

// ----------------------------------------------------------------------------

