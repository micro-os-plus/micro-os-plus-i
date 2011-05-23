/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_SPI)

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"

namespace avr32
{
  namespace uc3
  {
    Spi::Spi(ModuleId_t id)
    {
      if (id == zero)
        {
          m_channelAddress = (void*) 0xFFFF2400;
        }
      else if (id == one)
        {
          m_channelAddress = (void*) 0xFFFF2800;
        }
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_SPI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
