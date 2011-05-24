/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_USART)

#include "hal/arch/avr32/uc3/devices/onchip/include/Usart.h"

namespace avr32
{
  namespace uc3
  {
    Usart::Usart()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Usart", this);
    }
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_USART) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
