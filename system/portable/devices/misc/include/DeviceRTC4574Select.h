/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICERTC4574SELECT_H_
#define DEVICERTC4574SELECT_H_

#include "portable/devices/misc/include/DeviceChipSelect.h"

class DeviceRTC4574Select : public DeviceChipSelect
  {
public:
  static void implSelectInit(void);
  static void implSelectEnable(void);
  static void implSelectDisable(void);
  };

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceRTC4574Select_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR)  
#include "hal/arch/avr/devices/misc/include/DeviceRTC4574Select_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*DEVICERTC4574SELECT_H_*/
