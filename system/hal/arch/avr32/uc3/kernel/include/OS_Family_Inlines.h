/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"

inline void OSImpl::CPUidle(void)
  {
    asm volatile("	":::);

#if false
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_cpu();
#endif
  }

inline void OSImpl::CPUsleep(void)
  {
    asm volatile("	":::);

#if false
    set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
    sleep_cpu();
#endif
  }

inline void OSImpl::CPUdeepSleep(void)
  {
    asm volatile("	":::);

#if false
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_cpu();
#endif
  }

inline void OSImpl::WDTreset(void)
  {
    ;
  }

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void OSImpl::SOFTreset(void)
  {
#if false
    wdt_enable(WDTO_15MS);
    cli();
#endif
    for (;;)
      ; // trigger WD
  }

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
extern __inline__ void gpio_local_init(void)
{
  Set_system_register(AVR32_CPUCR,
                      Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
}

#endif /*HAL_FAMILY_OS_INLINES_H_*/
