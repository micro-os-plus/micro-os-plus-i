/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICELM74SELECT_H_
#define DEVICELM74SELECT_H_

#include "portable/devices/misc/include/DeviceChipSelect.h"

class DeviceLM74Select : public DeviceChipSelect
  {
public:
  static void implSelectInit(void);
  static void implSelectEnable(void);
  static void implSelectDisable(void);
  };

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceLM74Select_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceLM74Select_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/DeviceLM74Select_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR8)  
#include "hal/arch/avr8/devices/misc/include/DeviceLM74Select_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*DEVICELM74SELECT_H_*/
