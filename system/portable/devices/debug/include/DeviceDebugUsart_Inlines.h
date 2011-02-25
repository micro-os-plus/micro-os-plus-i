/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICEDEBUGUSART_INLINES_H_
#define DEVICEDEBUGUSART_INLINES_H_

#if defined(OS_CONFIG_BOARD_ATMEL_STK525)
#include "hal/boards/Atmel/stk525/include/DeviceDebugUsart_Defines.h"
#elif defined(OS_CONFIG_BOARD_METRILOG_M512)
#include "hal/boards/Metrilog/m512/include/DeviceDebugUsart_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/devices/debug/include/DeviceDebugUsart_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

inline int
OSDeviceDebug::implPutBytes(const char *s, unsigned int n)
{
  unsigned int i;
  for (i = 0; i < n; ++i)
    implPutByte(s[i]);

  return n;
}

#endif /*DEVICEDEBUGUSART_INLINES_H_*/
