/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEDEBUGI2C_ADDRESSES_DEFINES_H_
#define HAL_DEVICEDEBUGI2C_ADDRESSES_DEFINES_H_

// This is the addresses hard-wired in the USB-I2C
// debug device (Available from Aviation Instruments Intl)
#define OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR	 	(0x28)

// This is our own address on the I2C bus. Needed just as a setting,
// not really used, the debugged device does not accept commands on I2C.
#define OS_CFGINT8_DEBUG_I2C_OWN_ADDR			(0xFE)

#endif /*HAL_DEVICEDEBUGI2C_ADDRESSES_DEFINES_H_*/
