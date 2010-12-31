/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEBUGLED1_H_
#define OSDEBUGLED1_H_

#include "portable/kernel/include/OS.h"

class OSDebugLed1
  {
public:
  OSDebugLed1();

  static void toggle(void);
  static void on(void);
  static void off(void);
  static void init(void);

  unsigned char bitNumber(void);

private:
  };

inline OSDebugLed1::OSDebugLed1()
  {
  }

#if defined(OS_CONFIG_ARCH_AVR)
#include "hal/arch/avr/devices/debug/include/OSDebugLed1_Inlines.h"
#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/devices/debug/include/OSDebugLed1_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*OSDEBUGLED1_H_*/
