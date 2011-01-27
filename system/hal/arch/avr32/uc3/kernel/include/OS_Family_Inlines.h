/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

inline void
OSImpl::CPUstackInit(void)
{
  asm volatile (
      " lda.w   sp, _estack \n"
      :::
  );
}

extern unsigned long _evba;

inline void
OSImpl::CPUinit(void)
{
  // Switch to external Oscillator 0
  pm_switch_to_osc0(&AVR32_PM, OS_CFGLONG_OSCILLATOR_HZ,
      AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);

  // initialise local bus; without it GPIO does not work
  Set_system_register(AVR32_CPUCR,
      Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);

  // Set up EVBA so interrupts can be enabled later.
  Set_system_register(AVR32_EVBA, (int)&_evba);
}


inline void
OSImpl::returnFromInterrupt(void)
{
  asm volatile (
      " rete \n"
      :::
  );
  for (;;)
    ; // noreturn
}

inline void
OSImpl::returnFromSubroutine(void)
{
  asm volatile
  (
      " mov pc, lr \n"
      :::
  );
  for (;;)
    ; // noreturn
}

inline void
OSImpl::NOP(void)
{
  asm volatile
  (
      " nop \n"
      :::
  );
}

inline void
OSImpl::interruptsEnable(void)
{
  Enable_global_interrupt();
}

inline void
OSImpl::interruptsDisable(void)
{
  Disable_global_interrupt();
}

inline void
OSImpl::interruptsClearMask(void)
{
  ; // TODO: implement it
}

inline void
OSImpl::interruptsSetMask(void)
{
  ; // TODO: implement it
}

inline void
OSImpl::CPUidle(void)
{
  asm volatile
  (
      " sleep 0 \n"
      :::
  );
}

inline void
OSImpl::CPUsleep(void)
{
  asm volatile
  (
      " sleep 0 \n"     /* To be updated !!! */
      :::
  );
}

inline void
OSImpl::CPUdeepSleep(void)
{
  asm volatile
  (
      " sleep 0 \n"     /* To be updated !!! */
      :::
  );
}

inline void
OSImpl::WDTreset(void)
{
  ; // TODO: fill it in
}

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void
OSImpl::SOFTreset(void)
{
#if false
  wdt_enable(WDTO_15MS);
  cli();
#endif
  for (;;)
    ; // trigger WD
}

#endif /*HAL_FAMILY_OS_INLINES_H_*/
