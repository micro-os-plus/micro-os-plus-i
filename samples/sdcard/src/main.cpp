/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskCli.h"

// ------------------------------------------------------------------------------
// Cli Tasks

// Memory card
DeviceMemCard card;

// USB device
DeviceCharacterUsb devUsb;

// active objects allocated on static storage
TaskCli taskCli1("usb", devUsb, card);

#if false
// USART buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_TXBUF_SIZE ];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_RXBUF_SIZE ];

// USART device
DeviceCharacterMultiUsart1 devUsart(tx_store, sizeof(tx_store), rx_store,
    sizeof(rx_store));

TaskCli taskCli2("usart", devUsart, card );
#endif

// ------------------------------------------------------------------------------
// Blink Task

// active objects allocated on static storage
TaskBlink taskBlink("blink");

