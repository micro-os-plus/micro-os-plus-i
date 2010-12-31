/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMEMCARDSELECT_H_
#define DEVICEMEMCARDSELECT_H_

#include "portable/devices/misc/include/DeviceChipSelect.h"

class DeviceMemCardSelect : public DeviceChipSelect
  {
public:
  static void implSelectInit(void);
  static void implSelectEnable(void);
  static void implSelectDisable(void);
  };

#if defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceMemCardSelect_Defines.h"
#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)
#include "hal/boards/Atmel/stk525/include/DeviceMemCardSelect_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR)  
#include "hal/arch/avr/devices/block/include/DeviceMemCardSelect_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*DEVICEMEMCARDSELECT_H_*/
