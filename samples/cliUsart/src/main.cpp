/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"
#include "portable/tasks/include/TaskBlink.h"
#include "TaskCli.h"

// ------------------------------------------------------------------------------

// USART buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_RXBUF_SIZE];

DeviceCharacterBufferedUsart1 devUsart(tx_store, sizeof(tx_store), rx_store,
    sizeof(rx_store));

TaskCli taskCli("usart", devUsart);


// ------------------------------------------------------------------------------
// Blink Task

TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------
