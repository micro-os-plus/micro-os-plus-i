/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_SPI_H_
#define AVR32_UC3_SPI_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace spi
    {
      // ----- Type definitions -----------------------------------------------

      typedef enum ModuleId_e
      {
        MODULE_0 = 0, MODULE_1 = 1
      } ModuleId_t;

      typedef enum BitsPerTransfer_e
      {
        BITS_8 = 0, BITS_16 = 8
      } BitsPerTransfer_t;

      typedef uint8_t BaudRateFactor_t;
      typedef uint32_t BaudRate_t;

      // ----- Module memory mapped registers ---------------------------------

      class Registers
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS = 0xFFFF2400;
        const static uint32_t MEMORY_OFFSET = 0x0400;

        // ----- Memory map ---------------------------------------------------
        regWriteOnly_t cr; // 0x00
        regReadWrite_t mr; // 0x04
        regReadOnly_t rdr; // 0x08
        regWriteOnly_t tdr; // 0x0C
        regReadOnly_t sr; // 0x10
        regWriteOnly_t ier; // 0x14
        regWriteOnly_t idr; // 0x18
        regReadOnly_t imr; // 0x1C
        regReadOnly_t dummy20; // 0x20
        regReadOnly_t dummy24; // 0x24
        regReadOnly_t dummy28; // 0x28
        regReadOnly_t dummy2C; // 0x2C
        regReadWrite_t csr0; // 0x30
        regReadWrite_t csr1; // 0x34
        regReadWrite_t csr2; // 0x38
        regReadWrite_t csr3; // 0x3C
        // regReadWrite_t wpcr; // 0xE4
        // regReadOnly_t wpsr; // 0xE8
        // regReadOnly_t version; // 0xFC

        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint32_t mask);

        void
        writeMode(uint32_t value);

        uint32_t
        readMode(void);

        uint32_t
        readReceiveData(void);
        void
        writeTransmitData(uint32_t value);

        uint32_t
        readStatus(void);

        void
        writeInterruptsEnable(uint32_t mask);
        void
        writeInterruptsDisable(uint32_t mask);

        uint32_t
        readInterruptMask(void);

        void
        writeChipSelect0(uint32_t value);
        uint32_t
        readChipSelect0(void);
        void
        writeChipSelect1(uint32_t value);
        uint32_t
        readChipSelect1(void);
        void
        writeChipSelect2(uint32_t value);
        uint32_t
        readChipSelect2(void);
        void
        writeChipSelect3(uint32_t value);
        uint32_t
        readChipSelect3(void);

      };

      inline void
      Registers::writeControl(uint32_t mask)
      {
        this->cr = mask;
      }

      inline void
      Registers::writeMode(uint32_t value)
      {
        this->mr = value;
      }

      inline uint32_t
      Registers::readMode(void)
      {
        return this->mr;
      }

      inline uint32_t
      Registers::readReceiveData(void)
      {
        return this->rdr;
      }

      inline void
      Registers::writeTransmitData(uint32_t value)
      {
        this->tdr = value;
      }

      inline uint32_t
      Registers::readStatus(void)
      {
        return this->sr;
      }

      inline void
      Registers::writeInterruptsEnable(uint32_t mask)
      {
        this->ier = mask;
      }

      inline void
      Registers::writeInterruptsDisable(uint32_t mask)
      {
        this->idr = mask;
      }

      inline uint32_t
      Registers::readInterruptMask(void)
      {
        return this->imr;
      }

      inline void
      Registers::writeChipSelect0(uint32_t value)
      {
        this->csr0 = value;
      }

      inline uint32_t
      Registers::readChipSelect0(void)
      {
        return this->csr0;
      }

      inline void
      Registers::writeChipSelect1(uint32_t value)
      {
        this->csr1 = value;
      }

      inline uint32_t
      Registers::readChipSelect1(void)
      {
        return this->csr1;
      }

      inline void
      Registers::writeChipSelect2(uint32_t value)
      {
        this->csr2 = value;
      }

      inline uint32_t
      Registers::readChipSelect2(void)
      {
        return this->csr2;
      }

      inline void
      Registers::writeChipSelect3(uint32_t value)
      {
        this->csr3 = value;
      }

      inline uint32_t
      Registers::readChipSelect3(void)
      {
        return this->csr3;
      }

    }

    // Spi in master mode
    class Spim
    {
    public:
      Spim(spi::ModuleId_t module);
      ~Spim();

      // Init Spi in master mode
      void
      init(void);

      // configure baud rate and other communication parameters
      void
      configChipSelect(spi::BaudRateFactor_t baudRateFactor, uint8_t delayBCT,
          spi::BitsPerTransfer_t bitsPerTransfer);

      void
      configChipSelect(spi::BaudRateFactor_t baudRateFactor, uint8_t delayBCT, uint8_t delayBS,
          spi::BitsPerTransfer_t bitsPerTransfer, uint8_t configBits);

      void
      configBaudRateFactor(spi::BaudRateFactor_t baudRateFactor);

      void
      configBaudRate(spi::BaudRate_t baudRate);

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

    public:
      spi::Registers& registers;
    };

    inline uint32_t
    Spim::getInputClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBA_FREQUENCY_HZ;
    }

    inline void
    Spim::configChipSelect(spi::BaudRateFactor_t baudRateFactor, uint8_t delayBCT,
        spi::BitsPerTransfer_t bitsPerTransfer)
    {
      registers.writeChipSelect0(
          ((uint32_t) ((delayBCT & 0xFF) << AVR32_SPI_CSR0_DLYBCT_OFFSET) & AVR32_SPI_CSR0_DLYBCT_MASK) |
          ((uint32_t) ((baudRateFactor & 0xFF) << AVR32_SPI_CSR0_SCBR_OFFSET) & AVR32_SPI_CSR0_SCBR_MASK) |
          ((uint32_t) ((bitsPerTransfer & 0xF) << AVR32_SPI_CSR0_BITS_OFFSET) & AVR32_SPI_CSR0_BITS_MASK) |
          2);// hard-coded values for radio modem, i.e. NCPHA=1 and CPOL=0
    }

    inline void
    Spim::configChipSelect(spi::BaudRateFactor_t baudRateFactor, uint8_t delayBCT, uint8_t delayBS,
        spi::BitsPerTransfer_t bitsPerTransfer, uint8_t configBits)
    {
      registers.writeChipSelect0(
          ((uint32_t) ((delayBCT) << AVR32_SPI_CSR0_DLYBCT_OFFSET) & AVR32_SPI_CSR0_DLYBCT_MASK) |
          ((uint32_t) ((delayBS) << AVR32_SPI_CSR0_DLYBS_OFFSET) & AVR32_SPI_CSR0_DLYBS_MASK) |
          ((uint32_t) ((baudRateFactor) << AVR32_SPI_CSR0_SCBR_OFFSET) & AVR32_SPI_CSR0_SCBR_MASK) |
          ((uint32_t) ((bitsPerTransfer) << AVR32_SPI_CSR0_BITS_OFFSET) & AVR32_SPI_CSR0_BITS_MASK) |
          (configBits & 0xF));// hard-coded values for radio modem, i.e. NCPHA=1 and CPOL=0
    }

    inline void
    Spim::configBaudRateFactor(spi::BaudRateFactor_t baudRateFactor)
    {
      uint32_t val;

      val = registers.readChipSelect0();
      val &= ~AVR32_SPI_CSR0_SCBR_MASK;
      val |= ((uint32_t) ((baudRateFactor & 0xFF) << AVR32_SPI_CSR0_SCBR_OFFSET) & AVR32_SPI_CSR0_SCBR_MASK);
      registers.writeChipSelect0(val);
    }

    inline void
    Spim::configBaudRate(spi::BaudRate_t baudRate)
    {
      if ((getInputClockFrequencyHz()/baudRate) > 255)
        {
          OSDeviceDebug::putString("Spim::configBaudRate() out of range");
          OSDeviceDebug::putNewLine();
        }
      configBaudRateFactor(getInputClockFrequencyHz()/baudRate);
    }

    inline void
    Spim::enable(void)
    {
      registers.writeControl(AVR32_SPI_CR_SPIEN_MASK);
    }

    inline void
    Spim::disable(void)
    {
      registers.writeControl(AVR32_SPI_CR_SPIDIS_MASK);
    }

    inline void
    Spim::transmitByte(uint8_t value)
    {
      registers.writeTransmitData(value);
    }

    inline void
    Spim::transmitWord(uint16_t value)
    {
      registers.writeTransmitData(value);
    }

    inline bool
    Spim::isTransmitDataRegisterEmpty(void)
    {
      return (registers.readStatus() & AVR32_SPI_SR_TDRE_MASK) != 0;
    }

    inline bool
    Spim::isReceiveDataRegisterFull(void)
    {
      return (registers.readStatus() & AVR32_SPI_SR_RDRF_MASK) != 0;
    }

    inline bool
    Spim::isTransmittedAndReceived(void)
    {
      return (registers.readStatus() &
          (AVR32_SPI_SR_TXEMPTY_MASK | AVR32_SPI_SR_RDRF_MASK)) != 0;
    }

    inline uint8_t
    Spim::receiveByte(void)
    {
      return registers.readReceiveData();
    }

    inline uint16_t
    Spim::receiveWord(void)
    {
      return registers.readReceiveData();
    }

    inline void
    Spim::enableLocalLoopback()
    {
      registers.writeMode(registers.readMode() | AVR32_SPI_MR_LLB_MASK);
    }

    inline void
    Spim::disableLocalLoopback()
    {
      registers.writeMode(registers.readMode() & (~AVR32_SPI_MR_LLB_MASK));
    }
  }
}

#endif /* AVR32_UC3_SPI_H_ */
