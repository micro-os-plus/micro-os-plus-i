/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECANTRANSCEIVER_H_
#define DEVICECANTRANSCEIVER_H_

#include "portable/kernel/include/OS.h"

class DeviceCANTransceiver
{
public:
  static void
  transceiverInit(void);
  static void
  transceiverHighSpeed(void);
  static void
  transceiverStandby(void);
};

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceCANTransceiver_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceCANTransceiver_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/DeviceCANTransceiver_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR8)  
#include "hal/arch/avr8/devices/misc/include/DeviceCANTransceiver_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /* DEVICECANTRANSCEIVER_H_ */
