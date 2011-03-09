/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/tasks/include/TaskBlink.h"
#include "TaskCli.h"

// ------------------------------------------------------------------------------

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart0.h"

// USART buffers
unsigned char tx_store0[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_TXBUF_ARRAY_SIZE];
unsigned char rx_store0[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_RXBUF_ARRAY_SIZE];

DeviceCharacterBufferedUsart0 devUsart0(tx_store0, sizeof(tx_store0), rx_store0,
    sizeof(rx_store0));

TaskCli taskCli0("usart0", devUsart0);

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) */

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"

// USART buffers
unsigned char tx_store1[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_TXBUF_ARRAY_SIZE];
unsigned char rx_store1[OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_RXBUF_ARRAY_SIZE];

DeviceCharacterBufferedUsart1 devUsart1(tx_store1, sizeof(tx_store1), rx_store1,
    sizeof(rx_store1));

TaskCli taskCli1("usart1", devUsart1);

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1) */


// ------------------------------------------------------------------------------
// Blink Task

TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------
