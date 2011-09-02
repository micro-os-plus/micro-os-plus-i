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

    // ----- Constructors & Destructors ---------------------------------------

    SpiMaster::SpiMaster(spi::Module_t module) :
      registers(
          *reinterpret_cast<spi::Registers*> (spi::Registers::MEMORY_ADDRESS
              + ((module % 2) * spi::Registers::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Spim", module, this);
    }

    SpiMaster::~SpiMaster()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Spim", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    SpiMaster::powerUp(void)
    {
      uint32_t regValue;

      // software reset SPI module
      registers.writeControl(AVR32_SPI_CR_SWRST_MASK);

      // disable all interrupts
      registers.writeInterruptsDisable(0xFFFFFFFF);

#if 0 // Not works if optimized level is -Os. It seems that the compiler call
      // readMode() prior to the first writeMode(). Check the *.lst file.

      // Set master mode, enable RX FIFO and Disable Mode Fault
      registers.writeMode(
          AVR32_SPI_MSTR_MASK | AVR32_SPI_MR_MODFDIS_MASK
          | AVR32_SPI_MR_RXFIFOEN_MASK);

      // initialize CSR0
      registers.writeChipSelect0(0);

      // set fixed peripheral mode for CS0
      uint32_t chipSelectValue = 0;

      registers.writeMode(
          registers.readMode() | ((~(1 << (chipSelectValue
                          + AVR32_SPI_MR_PCS_OFFSET))) & AVR32_SPI_MR_PCS_MASK));
#else
      // set fixed peripheral mode for CS0
      uint32_t chipSelectValue = 0;

      regValue = (AVR32_SPI_MSTR_MASK | AVR32_SPI_MR_MODFDIS_MASK
          | AVR32_SPI_MR_RXFIFOEN_MASK) | ((~(1 << (chipSelectValue
          + AVR32_SPI_MR_PCS_OFFSET))) & AVR32_SPI_MR_PCS_MASK);

      // TODO: check this. It seems if -Os is used the while loop is necessary.
      while (registers.readMode() != regValue)
        {
          registers.writeMode(regValue);
        }

      // initialize CSR0
      registers.writeChipSelect0(0);
#endif
    }

    // ----- Public Methods ---------------------------------------------------

    // Busy wait version of a full Spi byte access
    uint8_t
    SpiMaster::writeWaitReadByte(uint8_t value)
    {
      transmitByte(value);
      //OSDeviceDebug::putHex(value);

      while (!isTransmittedAndReceived())
        ; // TODO: Watchdog?

      uint8_t b;
      b = receiveByte();

      //OSDeviceDebug::putHex(b);
      //OSDeviceDebug::putChar(' ');

      return b;
    }

    // Busy wait version of a full Spi word access
    uint16_t
    SpiMaster::writeWaitReadWord(uint16_t value)
    {
      transmitWord(value);

      while (!isTransmittedAndReceived())
        ; // TODO: Watchdog?

      return receiveWord();
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_SPI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
