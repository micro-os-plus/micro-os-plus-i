/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_ADC)

#include "hal/arch/avr32/uc3/devices/onchip/include/Adc.h"

namespace avr32
{
  namespace uc3
  {
    Adc::Adc()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Adc", this);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_ADC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
