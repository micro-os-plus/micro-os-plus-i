/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_H_
#define OS_H_

#include "portable/kernel/include/OS_Defines.h"

class OSImpl
  {
public:
  inline static void returnFromInterrupt(void) __attribute__((always_inline,noreturn));
  inline static void returnFromSubroutine(void) __attribute__((always_inline,noreturn));
  // ---

  inline static void CPUidle(void) __attribute__((always_inline));
  inline static void CPUsleep(void) __attribute__((always_inline));
  inline static void CPUdeepSleep(void) __attribute__((always_inline));

  inline static void WDTreset(void) __attribute__((always_inline));
  inline static void NOP(void) __attribute__((always_inline));
  inline static void SOFTreset(void) __attribute__((always_inline));

#if defined(OS_INLINE_INTERRUPTS_ENABLE_DISABLE)
  inline static void interruptsEnable(void) __attribute__((always_inline));
  inline static void interruptsDisable(void) __attribute__((always_inline));
#else
  inline static void interruptsEnable(void);
  inline static void interruptsDisable(void);
#endif

  inline static void interruptsClearMask(void) __attribute__((always_inline));
  inline static void interruptsSetMask(void) __attribute__((always_inline));

  };

#include "portable/kernel/include/OSScheduler.h"

#include "portable/kernel/include/OSTask.h"

#include "portable/kernel/include/OSTimer.h"

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
#include "portable/kernel/include/OSTimerSeconds.h"
#endif

#include "portable/kernel/include/OSTimerTicks.h"

#include "portable/kernel/include/Timer.h"

#include "portable/devices/debug/include/OSDeviceDebug.h"

// ----------------------------------------------------------------------------

typedef int OSReturn_t;

class OSReturn
  {
public:
  static const OSReturn_t OS_OK = 0;
  static const OSReturn_t OS_TIMEOUT = -1;
  static const OSReturn_t OS_DISCONNECTED = -2;
  static const OSReturn_t OS_WOULD_BLOCK = -3;
  static const OSReturn_t OS_NOT_OWNER = -4;
  static const OSReturn_t OS_NOT_ACQUIRED = -5;
  static const OSReturn_t OS_ALREADY_OPENED = -6;
  static const OSReturn_t OS_NOT_OPENED = -7;
  static const OSReturn_t OS_TOO_LONG = -8;
  static const OSReturn_t OS_ILLEGAL_CHARACTER = -9;
  static const OSReturn_t OS_BAD_CHECKSUM = -10;
  static const OSReturn_t OS_NOT_INITIALIZED = -11;
  static const OSReturn_t OS_BAD_PARAMETER = -12;
  static const OSReturn_t OS_BUFFER_FULL = -13;

  };

class OSTimeout
  {
public:
  static const OSTimerTicks_t OS_IMMEDIATELY = 0;
  static const OSTimerTicks_t OS_NEVER = (0 - 1);
  };

class OS: public OSImpl
  {

public:

#if !defined(OS_EXCLUDE_MULTITASKING)
  OSScheduler sched;
#endif

#if !defined(OS_EXCLUDE_MULTITASKING)
#if defined(DEBUG)
  OS();
#endif
#else
  static void main(void) __attribute__( ( noreturn ) );
#endif

#if defined(OS_INCLUDE_NAKED_INIT)
  static void nakedEarlyInit(void) __attribute__( ( naked, section( ".init6" ) ) );
#endif

  static void earlyInit(void);

#if defined(DEBUG)

  //const static bool isDebug = true;
  inline static bool isDebug()
    {
      return true;
    }
  ;
#else

  //const static bool isDebug = false;
  inline static bool isDebug()
    {
      return false;
    }
  ;
#endif

#if defined(OS_INCLUDE_OS_BUSYWAITMILLIS)
  static void busyWaitMillis(schedTicks_t n);
#endif

  static unsigned char getResetBits(void);

private:
  static void saveResetBits(void);
  static unsigned char ms_resetBits;
  };

inline unsigned char OS::getResetBits(void)
  {
    return ms_resetBits;
  }

extern "C" OSStack_t **g_ppCurrentStack;

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/include/OS_Arch_Inlines.h"
#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#include "hal/arch/arm_cortex_m3/kernel/include/OS_Arch_Inlines.h"
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/kernel/include/OS_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif
#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*OS_H_ */
