/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEBUGLED2_H_
#define OSDEBUGLED2_H_

#include "portable/kernel/include/OS.h"

class OSDebugLed2
{
public:
  OSDebugLed2();

  static void
  toggle(void);
  static void
  on(void);
  static void
  off(void);
  static void
  init(void);

  unsigned char
  bitNumber(void);

private:
};

inline
OSDebugLed2::OSDebugLed2()
{
}

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/devices/debug/include/OSDebugLed2_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/devices/debug/include/OSDebugLed2_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)
#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/devices/debug/include/OSDebugLed2_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif
#endif /* OSDEBUGLED2_H_ */
