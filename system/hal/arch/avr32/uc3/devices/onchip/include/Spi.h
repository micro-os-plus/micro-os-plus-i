/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_SPI_H_
#define AVR32_UC3_SPI_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    // Spi in master mode
    class SpiMaster
    {
    public:
      SpiMaster(spi::Module_t module);
      ~SpiMaster();

      // Init Spi in master mode
      void
      powerUp(void);

      // configure baud rate and other communication parameters
      void
      configureChipSelect(spi::BaudRateFactor_t baudRateFactor,
          uint8_t delayBCT, spi::BitsPerTransfer_t bitsPerTransfer);

      void
      configureChipSelect(spi::BaudRateFactor_t baudRateFactor,
          uint8_t delayBCT, uint8_t delayBS,
          spi::BitsPerTransfer_t bitsPerTransfer, uint8_t configBits);

      void
      configureBaudRateFactor(spi::BaudRateFactor_t baudRateFactor);

      void
      configureBaudRate(spi::BaudRate_t baudRate);

      void
      enable(void);
      void
      disable(void);

      void
      transmitByte(uint8_t value);
      void
      transmitWord(uint16_t value);
      bool
      isTransmitDataRegisterEmpty(void);
      bool
      isReceiveDataRegisterFull(void);
      bool
      isTransmittedAndReceived(void);
      uint8_t
      receiveByte(void);
      uint16_t
      receiveWord(void);

      uint8_t
      writeWaitReadByte(uint8_t value);
      uint16_t
      writeWaitReadWord(uint16_t value);
      void
      enableLocalLoopback();
      void
      disableLocalLoopback();

      uint32_t
      getInputClockFrequencyHz(void);

      void
      flushRxFifo(void);
    public:
      spi::Registers& registers;
    };

    inline uint32_t
    SpiMaster::getInputClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBA_FREQUENCY_HZ;
    }

    inline void
    SpiMaster::configureChipSelect(spi::BaudRateFactor_t baudRateFactor,
        uint8_t delayBCT, spi::BitsPerTransfer_t bitsPerTransfer)
    {
      registers.writeChipSelect0(((uint32_t) ((delayBCT & 0xFF)
          << AVR32_SPI_CSR0_DLYBCT_OFFSET) & AVR32_SPI_CSR0_DLYBCT_MASK)
          | ((uint32_t) ((baudRateFactor & 0xFF) << AVR32_SPI_CSR0_SCBR_OFFSET)
              & AVR32_SPI_CSR0_SCBR_MASK)
          | ((uint32_t) ((bitsPerTransfer & 0xF) << AVR32_SPI_CSR0_BITS_OFFSET)
              & AVR32_SPI_CSR0_BITS_MASK) | 2);// hard-coded values for radio modem, i.e. NCPHA=1 and CPOL=0
    }

    inline void
    SpiMaster::configureChipSelect(spi::BaudRateFactor_t baudRateFactor,
        uint8_t delayBCT, uint8_t delayBS,
        spi::BitsPerTransfer_t bitsPerTransfer, uint8_t configBits)
    {
      registers.writeChipSelect0(((uint32_t) ((delayBCT)
          << AVR32_SPI_CSR0_DLYBCT_OFFSET) & AVR32_SPI_CSR0_DLYBCT_MASK)
          | ((uint32_t) ((delayBS) << AVR32_SPI_CSR0_DLYBS_OFFSET)
              & AVR32_SPI_CSR0_DLYBS_MASK) | ((uint32_t) ((baudRateFactor)
          << AVR32_SPI_CSR0_SCBR_OFFSET) & AVR32_SPI_CSR0_SCBR_MASK)
          | ((uint32_t) ((bitsPerTransfer) << AVR32_SPI_CSR0_BITS_OFFSET)
              & AVR32_SPI_CSR0_BITS_MASK) | (configBits & 0xF));// hard-coded values for radio modem, i.e. NCPHA=1 and CPOL=0
    }

    inline void
    SpiMaster::configureBaudRateFactor(spi::BaudRateFactor_t baudRateFactor)
    {
      uint32_t val;

      val = registers.readChipSelect0();
      val &= ~AVR32_SPI_CSR0_SCBR_MASK;
      val
          |= ((uint32_t) ((baudRateFactor & 0xFF) << AVR32_SPI_CSR0_SCBR_OFFSET)
              & AVR32_SPI_CSR0_SCBR_MASK);
      registers.writeChipSelect0(val);
    }

    inline void
    SpiMaster::configureBaudRate(spi::BaudRate_t baudRate)
    {
      if ((getInputClockFrequencyHz() / baudRate) > 255)
        {
          OSDeviceDebug::putString("Spim::configBaudRate() out of range");
          OSDeviceDebug::putNewLine();
        }
      configureBaudRateFactor(getInputClockFrequencyHz() / baudRate);
    }

    inline void
    SpiMaster::enable(void)
    {
      registers.writeControl(AVR32_SPI_CR_SPIEN_MASK);
    }

    inline void
    SpiMaster::disable(void)
    {
      registers.writeControl(AVR32_SPI_CR_SPIDIS_MASK);
    }

    inline void
    SpiMaster::transmitByte(uint8_t value)
    {
      registers.writeTransmitData(value);
    }

    inline void
    SpiMaster::transmitWord(uint16_t value)
    {
      registers.writeTransmitData(value);
    }

    inline bool
    SpiMaster::isTransmitDataRegisterEmpty(void)
    {
      return (registers.readStatus() & AVR32_SPI_SR_TDRE_MASK) != 0;
    }

    inline bool
    SpiMaster::isReceiveDataRegisterFull(void)
    {
      return (registers.readStatus() & AVR32_SPI_SR_RDRF_MASK) != 0;
    }

    inline bool
    SpiMaster::isTransmittedAndReceived(void)
    {
      return (registers.readStatus() & (AVR32_SPI_SR_TXEMPTY_MASK
          | AVR32_SPI_SR_RDRF_MASK)) != 0;
    }

    inline uint8_t
    SpiMaster::receiveByte(void)
    {
      return registers.readReceiveData();
    }

    inline uint16_t
    SpiMaster::receiveWord(void)
    {
      return registers.readReceiveData();
    }

    inline void
    SpiMaster::enableLocalLoopback()
    {
      registers.writeMode(registers.readMode() | AVR32_SPI_MR_LLB_MASK);
    }

    inline void
    SpiMaster::disableLocalLoopback()
    {
      registers.writeMode(registers.readMode() & (~AVR32_SPI_MR_LLB_MASK));
    }

    inline void
    SpiMaster::flushRxFifo(void)
    {
      registers.writeControl(AVR32_SPI_CR_FLUSHFIFO_MASK);
    }
  }
}

#endif /* AVR32_UC3_SPI_H_ */
