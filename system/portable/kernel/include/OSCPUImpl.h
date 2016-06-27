/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSCPUIMPL_H_
#define OSCPUIMPL_H_

// ----------------------------------------------------------------------------

class OSCPUImpl
{
public:
  // Initialise the SP register in order to point to the stack section.
  inline static void
  stackInit(void) __attribute__((always_inline));
  // Some CPU initialisations.
  static void
  earlyInit(void);
  // Put MCU to the idle sleep mode.
  inline static void
  idle(void) __attribute__((always_inline));
  // Put MCU to sleep.
  inline static void
  sleep(void) __attribute__((always_inline));
  // Put MCU to deep sleep.
  inline static void
  deepSleep(void) __attribute__((always_inline));
  // Returns the reset bits used to know the reset reason.
  inline static OSResetBits_t
  fetchResetBits(void) __attribute__((always_inline));
  // Reset the WDT.
  inline static void
  watchdogReset(void) __attribute__((always_inline));
  // No operation performed. Embeds the nop assembly command.
  inline static void
  nop(void) __attribute__((always_inline));
  // Generate a soft reset.
  inline static void
  softReset(void) __attribute__((always_inline));
  // Wrapper for the rete instruction.
  inline static void
  returnFromInterrupt(void) __attribute__((always_inline,noreturn));
  // Copy link register's (LR) value into the PC. This will restart execution
  // of the previously executed routine.
  inline static void
  returnFromSubroutine(void) __attribute__((always_inline,noreturn));

#if defined(OS_INLINE_INTERRUPTS_ENABLE_DISABLE)
  // Enable interrupts (global interrupt mask bit - GM from SR).
  inline static void
  interruptsEnable(void) __attribute__((always_inline));
  // Disable all interrupts (global interrupt mask bit - GM from SR).
  inline static void
  interruptsDisable(void) __attribute__((always_inline));
#else
  // Enable interrupts (global interrupt mask bit - GM from SR).
  inline static void
  interruptsEnable(void);
  // Disable all interrupts (global interrupt mask bit - GM from SR).
  inline static void
  interruptsDisable(void);
#endif

#if false
  // Used for ARM architecture.
  inline static void interruptsClearMask(void) __attribute__((always_inline));
#endif

  // support for critical sections
  inline static void
  setInterruptsMask(OSInterruptsMask_t) __attribute__((always_inline));
  inline static OSInterruptsMask_t
  getInterruptsMask(void) __attribute__((always_inline));

  inline static void
  stackPush(OSStack_t) __attribute__((always_inline));
  inline static OSStack_t
  stackPop(void) __attribute__((always_inline));

  inline static bool
  isInInterruptMode(void) __attribute__((always_inline));

  inline static bool
  isInCriticalSection(void) __attribute__((always_inline));

private:

};

#if defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/kernel/include/OSCPUImpl_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR8)

#include "hal/arch/avr8/kernel/include/OSCPUImpl_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)

#include "hal/arch/arm_cortex_m3/kernel/include/OSCPUImpl_Inlines.h"

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif
