/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "portable/devices/character/include/DeviceCharacterUsart.h"
#include "portable/devices/character/include/DeviceCharacterMultiUsart1.h"
#include "portable/tasks/include/TaskBlink.h"

#include "TaskCli.h"

// ------------------------------------------------------------------------------
// Cli Tasks
//#define OS_INCLUDE_USB_CDC_DUAL_INTERFACE
// USB device
DeviceCharacterUsb devUsb0;
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
DeviceCharacterUsb devUsb1(1);
#endif


#if defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART1)

// USART buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERMULTIUSART1_RXBUF_SIZE];

// USART device
DeviceCharacterMultiUsart1 devUsart(tx_store, sizeof(tx_store), rx_store,
    sizeof(rx_store));

#else

// USART buffers
unsigned char tx_store[OS_CFGINT_DEVICECHARACTERUSART_TXBUF_SIZE];
unsigned char rx_store[OS_CFGINT_DEVICECHARACTERUSART_RXBUF_SIZE];

// USART device
DeviceCharacterUsart devUsart(tx_store, sizeof(tx_store),
    rx_store, sizeof(rx_store));

#endif

// tasks allocated on static storage
TaskCli taskCli0("usart", devUsart);

#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
TaskCli taskCli1("usb1", devUsb0 );
TaskCli taskCli2("usb2", devUsb1 );
#else
TaskCli taskCli1("usb", devUsb0);
#endif

// ------------------------------------------------------------------------------
// Blink Task

// task allocated on static storage
TaskBlink taskBlink("blink");

// ------------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
// and in the CLI session at open
const char greeting[]= "AVI cli v1.2";

