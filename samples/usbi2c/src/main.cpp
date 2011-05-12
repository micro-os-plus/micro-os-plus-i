/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskCli.h"
#include "TaskDbgIn.h"
#include "DeviceCharacterI2C.h"

// ----------------------------------------------------------------------------
// USB device

DeviceCharacterUsb devUsb;

// ----------------------------------------------------------------------------
// Input active object, must be the first one to init I2C

// I2C buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERMI2C_RXBUF_SIZE];

DeviceCharacterI2C devI2C(tx_store,
    sizeof(tx_store), rx_store,
    sizeof(rx_store));

TaskDbgIn taskDbgIn("input", devUsb,  devI2C);

// ----------------------------------------------------------------------------
// Cli Task

// active object
TaskCli taskCli("usb", devUsb, devI2C);

// ----------------------------------------------------------------------------
// Blink Task
TaskPitpalac taskBlink("blink");

// ----------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
// and in the CLI session at open
const char greeting[] = APP_CFGSTR_GREETING;

