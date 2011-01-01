/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMCP2510SELECT_H_
#define DEVICEMCP2510SELECT_H_

#include "portable/devices/misc/include/DeviceChipSelect.h"

class DeviceMCP2510Select : public DeviceChipSelect
  {
public:
  static void implSelectInit(void);
  static void implSelectEnable(void);
  static void implSelectDisable(void);
  };

#if defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceMCP2510Select_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR)  
#include "hal/arch/avr8/devices/misc/include/DeviceMCP2510Select_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*DEVICEMCP2510SELECT_H_*/
