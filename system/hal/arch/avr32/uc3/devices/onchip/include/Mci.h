/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MCI_H_
#define AVR32_UC3_MCI_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"

namespace avr32
{
  namespace uc3
  {
    namespace mci
    {
      // ----- Type definitions -----------------------------------------------

      // ----- Port memory mapped registers -----------------------------------

      class Registers
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::MCI;
        //const static uint32_t MEMORY_OFFSET = 0x100;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regReadWrite_t dtor; //0x0008
        regReadWrite_t sdcr; //0x000C

        regReadWrite_t argr; //0x0010
        regWriteOnly_t cmdr; //0x0014
        regReadWrite_t blkr; //0x0018
        regReadWrite_t cstor; //0x001C

        regReadOnly_t rspr[4]; //0x0020-0x002C

        regReadOnly_t rdr; //0x0030
        regWriteOnly_t tdr; //0x0034
        regNotAllocated_t dummy38[2]; //0x0038-0x003C

        regReadOnly_t sr; //0x0040
        regWriteOnly_t ier; //0x0044
        regWriteOnly_t idr; //0x0048
        regReadOnly_t imr; //0x004C

        regReadWrite_t dma; //0x0050
        regReadWrite_t cfg; //0x0054
        regNotAllocated_t dummy58[35]; //0x0058-0x00E0

        regReadWrite_t wpmr; //0x00E4
        regReadOnly_t wpsr; //0x00E8
        regNotAllocated_t dummyEC[4]; //0x00EC-0x00F8

        regReadOnly_t version; //0x00FC

        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint8_t mask);

        void
        writeMode(uint32_t value);

        uint32_t
        readMode(void);

        void
        writeDataTimeout(uint8_t value);

        uint8_t
        readDataTimeout(void);

        void
        writeSdCard(uint8_t value);

        uint8_t
        readSdCard(void);

        void
        writeArgument(uint32_t value);

        uint32_t
        readArgument(void);

        void
        writeCommand(uint32_t value);

        void
        writeBlock(uint32_t value);

        uint32_t
        readBlock(void);

        void
        writeCompletionSignalTimeout(uint8_t value);

        uint8_t
        readCompletionSignalTimeout(void);

        uint32_t
        readResponse(uint_t index);

        uint32_t
        readReceiveData(void);

        void
        writeTransmitData(uint32_t value);

        uint32_t
        readStatus(void);

        // Writing 1 to a bit will se IMRt; writing 0 will be ignored
        void
        writeInterruptEnable(uint32_t value);

        // Writing 1 to a bit will clear IMR; writing 0 will be ignored
        void
        writeInterruptDisable(uint32_t value);

        uint32_t
        readInterruptMask(void);

        void
        writeDmaConfiguration(uint16_t value);

        uint16_t
        readDmaConfiguration(void);

        void
        writeConfiguration(uint16_t value);

        uint16_t
        readConfiguration(void);

        void
        writeWriteProtectMode(uint32_t value);

        uint32_t
        readWriteProtectMode(void);

        uint32_t
        readWriteProtectStatus(void);

        uint32_t
        readVersion(void);

      };

      inline void
      Registers::writeControl(uint8_t mask)
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

      inline void
      Registers::writeDataTimeout(uint8_t value)
      {
        this->dtor = value;
      }

      inline uint8_t
      Registers::readDataTimeout(void)
      {
        return this->dtor;
      }

      inline void
      Registers::writeSdCard(uint8_t value)
      {
        this->sdcr = value;
      }

      inline uint8_t
      Registers::readSdCard(void)
      {
        return this->sdcr;
      }

      inline void
      Registers::writeArgument(uint32_t value)
      {
        this->argr = value;
      }

      inline uint32_t
      Registers::readArgument(void)
      {
        return this->argr;
      }

      inline void
      Registers::writeCommand(uint32_t value)
      {
        this->cmdr = value;
      }

      inline void
      Registers::writeBlock(uint32_t value)
      {
        this->cmdr = value;
      }

      inline uint32_t
      Registers::readBlock(void)
      {
        return this->blkr;
      }

      inline void
      Registers::writeCompletionSignalTimeout(uint8_t value)
      {
        this->cstor = value;
      }

      inline uint8_t
      Registers::readCompletionSignalTimeout(void)
      {
        return this->cstor;
      }

      inline uint32_t
      Registers::readResponse(uint_t index)
      {
        return this->rspr[index];
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

      // Writing 1 to a bit will se IMRt; writing 0 will be ignored
      inline void
      Registers::writeInterruptEnable(uint32_t value)
      {
        this->ier = value;
      }

      // Writing 1 to a bit will clear IMR; writing 0 will be ignored
      inline void
      Registers::writeInterruptDisable(uint32_t value)
      {
        this->idr = value;
      }

      inline uint32_t
      Registers::readInterruptMask(void)
      {
        return this->imr;
      }

      inline void
      Registers::writeDmaConfiguration(uint16_t value)
      {
        this->dma = value;
      }

      inline uint16_t
      Registers::readDmaConfiguration(void)
      {
        return this->dma;
      }

      inline void
      Registers::writeConfiguration(uint16_t value)
      {
        this->cfg = value;
      }

      inline uint16_t
      Registers::readConfiguration(void)
      {
        return this->cfg;
      }

      inline void
      Registers::writeWriteProtectMode(uint32_t value)
      {
        this->wpmr = value;
      }

      inline uint32_t
      Registers::readWriteProtectMode(void)
      {
        return this->wpmr;
      }

      inline uint32_t
      Registers::readWriteProtectStatus(void)
      {
        return this->wpsr;
      }

      inline uint32_t
      Registers::readVersion(void)
      {
        return this->version;
      }

    }

    class Mci
    {
    public:
      Mci();

    public:
      volatile mci::Registers& registers;

    };
  }
}
#endif /* AVR32_UC3_MCI_H_ */
