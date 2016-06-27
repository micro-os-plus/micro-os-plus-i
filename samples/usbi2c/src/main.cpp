/*
 *	Copyright (C) 2007-2012 Liviu Ionescu.
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

std::istream cin(&devUsb);
std::ostream cout(&devUsb);

// ----------------------------------------------------------------------------
// Input active object, must be the first one to init I2C

// I2C buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERI2C_RXBUF_SIZE];

DeviceCharacterI2C devI2C(tx_store, sizeof(tx_store), rx_store,
    sizeof(rx_store));

// ----------------------------------------------------------------------------
// CLI thread

// active object
TaskCli taskCli("usb", devUsb, devI2C, cin, cout);

// ----------------------------------------------------------------------------
// I2C input thread

TaskDbgIn taskDbgIn("input", devUsb, devI2C, cout);

// ----------------------------------------------------------------------------
// Blink Task
TaskPitpalac taskBlink("blink");

// ----------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
// and in the CLI session at open
const char greeting[] = APP_CFGSTR_GREETING;

// ---------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)

void
OSApplicationImpl::interruptTick(void)
{
  DeviceCharacterI2C::pDevice->interruptTimerTick();
}

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */
