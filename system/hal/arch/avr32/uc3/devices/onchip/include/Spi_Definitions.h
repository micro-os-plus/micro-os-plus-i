/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_SPI_DEFINITIONS_H_
#define AVR32_UC3_SPI_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace spi
    {
      // ----- Type definitions -----------------------------------------------

      typedef uint8_t Module_t;

      class Module
      {
      public:
        const static Module_t SPI0 = 0;
        const static Module_t SPI1 = 1;
      };

      typedef enum BitsPerTransfer_e
      {
        BITS_8 = 0, BITS_16 = 8
      } BitsPerTransfer_t;

      typedef uint8_t BaudRateFactor_t;
      typedef uint32_t BaudRate_t;

      // ----- ModuleId memory mapped registers ---------------------------------

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

      // ----- Inline methods -------------------------------------------------

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

      // ----------------------------------------------------------------------

    }
  }
}

#endif /* AVR32_UC3_SPI_DEFINITIONS_H_ */
