/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_USART_DEFINITIONS_H_
#define AVR32_UC3_USART_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace usart
    {
      // ----- Type definitions -----------------------------------------------

      typedef uint8_t ModuleId_t;

      class ModuleId
      {
      public:
        const static ModuleId_t USART0 = 0;
        const static ModuleId_t USART1 = 1;
        const static ModuleId_t USART2 = 2;
        const static ModuleId_t USART3 = 3;
      };

      typedef uint32_t ClockFrequencyHz_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::USART0;
        const static uint32_t MEMORY_OFFSET = 0x0400;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regWriteOnly_t ier; //0x0008
        regWriteOnly_t idr; //0x000C

        regReadOnly_t imr; // 0x0010
        regReadOnly_t csr; // 0x0014
        regReadOnly_t rhr; // 0x0018
        regWriteOnly_t thr; //0x001C

        regReadWrite_t brgr; //0x0020
        regReadWrite_t rtor; //0x0024
        regReadWrite_t ttgr; //0x0028
        regNotAllocated_t
            dummy2C[(0x0040 - 0x002C) / sizeof(regNotAllocated_t)]; //0x002C-0x003C

        regReadWrite_t fidi; //0x0040
        regWriteOnly_t ner; //0x0044
        regNotAllocated_t dummy48; // 0x0048
        regReadWrite_t ifr; //0x004C

        regReadWrite_t man; //0x0050
        regReadWrite_t linmr; //0x0054
        regReadWrite_t linir; //0x0058
        regNotAllocated_t
            dummy5C[(0x00E4 - 0x005C) / sizeof(regNotAllocated_t)]; //0x005C-0x00E0

        regReadWrite_t wpmr; //0x00E4
        regReadOnly_t wpsr; // 0x00E8
        regNotAllocated_t
            dummyEC[(0x00FC - 0x00EC) / sizeof(regNotAllocated_t)]; //0x00EC-0x00F8

        regReadOnly_t version; // 0x00FC


        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint32_t mask);

        void
        writeMode(uint32_t value);

        uint32_t
        readMode(void);

        // Writing 1 to a bit will set IMR; writing 0 will be ignored
        void
        writeInterruptEnable(uint32_t value);

        // Writing 1 to a bit will clear IMR; writing 0 will be ignored
        void
        writeInterruptDisable(uint32_t value);

        uint32_t
        readInterruptMask(void);

        uint32_t
        readChannelStatus(void);

        uint16_t
        readReceiveHolding(void);

        void
        writeTransmitHolding(uint16_t value);

        void
        writeBaudRateGenerator(uint32_t value);

        uint32_t
        readBaudRateGenerator(void);

        void
        writeReceiveTimeOut(uint32_t value);

        uint32_t
        readReceiveTimeOut(void);

        void
        writeTransmitterTimeGuard(uint8_t value);

        uint8_t
        readTransmitterTimeGuard(void);

        void
        writeFiDiRatio(uint16_t value);

        uint16_t
        readFiDiRatio(void);

        uint8_t
        readNumberOfErrors(void);

        void
        writeIrdaFilter(uint8_t value);

        uint8_t
        readIrdaFilter(void);

        void
        writeManchesterConfiguration(uint32_t value);

        uint32_t
        readManchesterConfiguration(void);

        void
        writeLinMode(uint32_t value);

        uint32_t
        readLinMode(void);

        void
        writeLinIdentifier(uint8_t value);

        uint8_t
        readLinIdentifier(void);

        void
        writeWriteProtect(uint32_t value);

        uint32_t
        readWriteProtect(void);

        uint32_t
        readWriteProtectStatus(void);

        uint32_t
        readVersion(void);

        // --------------------------

        bool
        isTransmitterReady(void);

        bool
        isReceiverReady(void);

      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeControl(uint32_t mask)
      {
        this->cr = mask;
      }

      inline void
      ModuleRegisters::writeMode(uint32_t value)
      {
        this->mr = value;
      }

      inline uint32_t
      ModuleRegisters::readMode(void)
      {
        return this->mr;
      }

      // Writing 1 to a bit will set IMR; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptEnable(uint32_t value)
      {
        this->ier = value;
      }

      // Writing 1 to a bit will clear IMR; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptDisable(uint32_t value)
      {
        this->idr = value;
      }

      inline uint32_t
      ModuleRegisters::readInterruptMask(void)
      {
        return this->imr;
      }

      inline uint32_t
      ModuleRegisters::readChannelStatus(void)
      {
        return this->csr;
      }

      inline uint16_t
      ModuleRegisters::readReceiveHolding(void)
      {
        return this->rhr;
      }

      inline void
      ModuleRegisters::writeTransmitHolding(uint16_t value)
      {
        this->thr = value;
      }

      inline void
      ModuleRegisters::writeBaudRateGenerator(uint32_t value)
      {
        this->brgr = value;
      }

      inline uint32_t
      ModuleRegisters::readBaudRateGenerator(void)
      {
        return this->brgr;
      }

      inline void
      ModuleRegisters::writeReceiveTimeOut(uint32_t value)
      {
        this->rtor = value;
      }

      inline uint32_t
      ModuleRegisters::readReceiveTimeOut(void)
      {
        return this->rtor;
      }

      inline void
      ModuleRegisters::writeTransmitterTimeGuard(uint8_t value)
      {
        this->ttgr = value;
      }

      inline uint8_t
      ModuleRegisters::readTransmitterTimeGuard(void)
      {
        return this->ttgr;
      }

      inline void
      ModuleRegisters::writeFiDiRatio(uint16_t value)
      {
        this->fidi = value;
      }

      inline uint16_t
      ModuleRegisters::readFiDiRatio(void)
      {
        return this->fidi;
      }

      inline uint8_t
      ModuleRegisters::readNumberOfErrors(void)
      {
        return this->ner;
      }

      inline void
      ModuleRegisters::writeIrdaFilter(uint8_t value)
      {
        this->ifr = value;
      }

      inline uint8_t
      ModuleRegisters::readIrdaFilter(void)
      {
        return this->ifr;
      }

      inline void
      ModuleRegisters::writeManchesterConfiguration(uint32_t value)
      {
        this->man = value;
      }

      inline uint32_t
      ModuleRegisters::readManchesterConfiguration(void)
      {
        return this->man;
      }

      inline void
      ModuleRegisters::writeLinMode(uint32_t value)
      {
        this->linmr = value;
      }

      inline uint32_t
      ModuleRegisters::readLinMode(void)
      {
        return this->linmr;
      }

      inline void
      ModuleRegisters::writeLinIdentifier(uint8_t value)
      {
        this->linir = value;
      }

      inline uint8_t
      ModuleRegisters::readLinIdentifier(void)
      {
        return this->linir;
      }

      inline void
      ModuleRegisters::writeWriteProtect(uint32_t value)
      {
        this->wpmr = value;
      }

      inline uint32_t
      ModuleRegisters::readWriteProtect(void)
      {
        return this->wpmr;
      }

      inline uint32_t
      ModuleRegisters::readWriteProtectStatus(void)
      {
        return this->wpsr;
      }

      inline uint32_t
      ModuleRegisters::readVersion(void)
      {
        return this->version;
      }

      // ----------

      inline bool
      ModuleRegisters::isTransmitterReady(void)
      {
        return ((readChannelStatus() & AVR32_USART_CSR_TXRDY_MASK) != 0);
      }

      inline bool
      ModuleRegisters::isReceiverReady(void)
      {
        return ((readChannelStatus() & AVR32_USART_CSR_RXRDY_MASK) != 0);
      }

    }

  // --------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_USART_DEFINITIONS_H_ */
