/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_RTC)

#include "hal/arch/avr32/uc3/devices/onchip/include/Rtc.h"

namespace avr32
{
  namespace uc3
  {
    Rtc::Rtc() :
          registers(
              *reinterpret_cast<rtc::ModuleRegisters*> (rtc::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Rtc", this);
    }

    void
    Rtc::resetCounter(void)
    {
      while (registers.readValue() > 0)
        {
          // If busy, wait, according to the manual it should last
          // 'a few cycles to propagate the values stored in CTRL, TOP and VAL'
          while((registers.readControl() & AVR32_RTC_BUSY_MASK) != 0)
            ;

          // Try to clear the value register
          registers.writeValue(0);

          // It still may not succeed, in case busy occurred
          // right after checking, so we check
        }
    }

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_RTC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
