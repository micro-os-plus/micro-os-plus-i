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
    Spim::Spim(spi::ModuleId_t module) :
          registers(
              *reinterpret_cast<spi::Registers*> (spi::Registers::MEMORY_ADDRESS
                  + ((module % 2) * spi::Registers::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Spim", this);
    }

    void
    Spim::init(void)
    {
      // TODO: implement it
    }

    // Busy wait version of a full Spi byte access
    uint8_t
    Spim::writeWaitReadByte(uint8_t value)
    {
      transmitByte(value);
      while (!isTransmitDataRegisterEmpty())
        // TODO: check logic
        ;
      return receiveByte();
    }

    // Busy wait version of a full Spi word access
    uint16_t
    Spim::writeWaitReadWord(uint16_t value)
    {
      transmitWord(value);
      while (!isTransmitDataRegisterEmpty())
        // TODO: check logic
        ;
      return receiveWord();
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_SPI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
