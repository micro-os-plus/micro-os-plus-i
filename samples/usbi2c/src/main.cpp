/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskCli.h"
#include "TaskDbgIn.h"
#include "DeviceCharacterI2C.h"

// USB device
DeviceCharacterUsb devUsb;

// ------------------------------------------------------------------------------
// Input task, must be the first one to init I2C

// USART buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_RXBUF_SIZE];

// USART device
DeviceCharacterMultiUsart1 devUsart(tx_store,
    OS_CFGINT_DEVICECHARACTERMULTIUSART1_TXBUF_SIZE, rx_store,
    OS_CFGINT_DEVICECHARACTERMULTIUSART1_RXBUF_SIZE);

DeviceCharacterI2C devI2C(tx_store,
    OS_CFGINT_DEVICECHARACTERMULTIUSART1_TXBUF_SIZE, rx_store,
    OS_CFGINT_DEVICECHARACTERMULTIUSART1_RXBUF_SIZE);

TaskDbgIn taskDbgIn("input", devUsb, devUsart, devI2C);

// ------------------------------------------------------------------------------
// Cli Task

// task
TaskCli taskCli("usb", devUsb, devI2C);

// ------------------------------------------------------------------------------
// Blink Task
TaskPitpalac taskBlink("blink");

// ------------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
// and in the CLI session at open
const char greeting[]= "AVI USB-I2C v" APP_CONFIG_VERSION ", " __DATE__ " " __TIME__;

