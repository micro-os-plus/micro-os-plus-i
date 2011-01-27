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

extern unsigned long _evba;

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
