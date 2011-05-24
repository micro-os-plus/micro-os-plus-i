/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_GPIO)

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    Gpio::Gpio()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Gpio", this);
    }
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_GPIO) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
