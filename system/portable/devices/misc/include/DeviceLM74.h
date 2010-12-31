/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICELM74_H_
#define DEVICELM74_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/misc/include/DeviceLM74Select.h"

class DeviceLM74 : public DeviceLM74Select
  {
public:
  static void init(void);

  static signed short read(void);

private:
  static void clkInit(void);
  static void clkHigh(void);
  static void clkLow(void);

  static void dataIn(void);
  static void dataOut(void);
  static void dataHigh(void);
  static void dataLow(void);
  static bool dataIsLow(void);
  };

#if defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceLM74_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceLM74_Defines.h"
#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/DeviceLM74_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if defined(OS_CONFIG_ARCH_AVR)  
#include "hal/arch/avr/devices/misc/include/DeviceLM74_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*DEVICELM74_H_*/
