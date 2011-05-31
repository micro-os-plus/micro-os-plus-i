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

    Spim::~Spim()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Spim", this);
    }

    void
    Spim::init(void)
    {
      // software reset SPI module
      registers.writeControl(AVR32_SPI_CR_SWRST_MASK);
      // Set master mode and enable RX FIFO
      //AVR32_SPI_MR_RXFIFOEN_MASK
      registers.writeMode(AVR32_SPI_MSTR_MASK |
          AVR32_SPI_MR_MODFDIS_MASK | AVR32_SPI_MR_LLB_MASK);
      // initialize CSR0
      registers.writeChipSelect0(0);
      // set fixed peripheral mode for CS0
      // set PCS field from MR to 1110 value
      uint32_t chipSelectValue = 0;
      registers.writeMode(registers.readMode() |
          //((~(1 << (chipSelectValue + AVR32_SPI_MR_PCS_OFFSET));
          ( (uint32_t)0b1110 << AVR32_SPI_MR_PCS_OFFSET));

      // 1Mbps, 0, delay, 1 byte word size, CASST, ...
      registers.writeChipSelect0((uint32_t)8<<8 | (uint32_t)0x0A);
                //((1 << chipSelectValue) << AVR32_SPI_MR_PCS_OFFSET));
      // clear RDR
      receiveByte();
    }

    // Busy wait version of a full Spi byte access
    uint8_t
    Spim::writeWaitReadByte(uint8_t value)
    {
      transmitByte(value);
#if true
      //while (!isReceiveDataRegisterFull())
      // wait for sending
      while (registers.readStatus() & AVR32_SPI_SR_TXEMPTY_MASK == 0)
        ;
      // wait for receiving
      while (registers.readStatus() & AVR32_SPI_SR_RDRF_MASK == 0)
#else
      while (!isTransmitDataRegisterEmpty())
#endif
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
