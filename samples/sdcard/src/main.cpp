/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "TaskCli.h"

// ------------------------------------------------------------------------------
// Cli Tasks

// USB device
DeviceCharacterUsb devUsb;

// USART buffers
unsigned char tx_store[OS_CONFIG_TTYUSART_TXBUF_SIZE ];
unsigned char rx_store[OS_CONFIG_TTYUSART_RXBUF_SIZE ];

// USART device
DeviceCharacterUsart devUsart(tx_store, OS_CONFIG_TTYUSART_TXBUF_SIZE,
    rx_store, OS_CONFIG_TTYUSART_RXBUF_SIZE);

DeviceMemCard card;

// tasks allocated on static storage
TaskCli taskCli1("usb", devUsb, card);
//TaskCli taskCli2("usart", devUsart, card );

// ------------------------------------------------------------------------------
// Blink Task

// task allocated on static storage
TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
// and in the CLI session at open
const char greeting[]= "AVI sdcard v1.2";

