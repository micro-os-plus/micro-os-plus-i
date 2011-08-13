/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_TWIM_DEFINITIONS_H_
#define AVR32_UC3_TWIM_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace twim
    {

      // ----- Type definitions -----------------------------------------------

      typedef uint8_t ModuleId_t;

      class ModuleId // Preserve the 'Id'
      {
      public:
        const static ModuleId_t TWIM0 = 0;
        const static ModuleId_t TWIM1 = 1;
      };

      typedef uint32_t Status_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:

        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::TWIM0;
        const static uint32_t MEMORY_OFFSET = 0x0400;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t cwgr; //0x0004
        regReadWrite_t smbtr; //0x0008
        regReadWrite_t cmdr; //0x000C

        regReadWrite_t ncmdr; //0x0010
        regReadOnly_t rhr; // 0x0014
        regWriteOnly_t thr; //0x0018
        regReadOnly_t sr; // 0x001C

        regWriteOnly_t ier; //0x0020
        regWriteOnly_t idr; //0x0024
        regReadOnly_t imr; // 0x0028
        regWriteOnly_t scr; //0x002C

        regReadOnly_t pr; // 0x0030
        regReadOnly_t vr; // 0x0034

        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint16_t mask);

        void
        writeClockWaveformGenerator(uint32_t value);

        uint32_t
        readClockWaveformGenerator(void);

        void
        writeSmbusTiming(uint32_t value);

        uint32_t
        readSmbusTiming(void);

        void
        writeCommand(uint32_t value);

        uint32_t
        readCommand(void);

        void
        writeNextCommand(uint32_t value);

        uint32_t
        readNextCommand(void);

        uint8_t
        readReceiveHolding(void);

        void
        writeTransmitHolding(uint8_t value);

        Status_t
        readStatus(void);

        // Writing 1 to a bit will set IMR; writing 0 will be ignored
        void
        writeInterruptEnable(uint16_t value);

        // Writing 1 to a bit will clear IMR; writing 0 will be ignored
        void
        writeInterruptDisable(uint16_t value);

        uint16_t
        readInterruptMask(void);

        void
        writeStatusClear(uint16_t value);

        uint32_t
        readParameter(void);

        uint32_t
        readVersion(void);

      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeControl(uint16_t mask)
      {
        this->cr = mask;
      }

      inline void
      ModuleRegisters::writeClockWaveformGenerator(uint32_t value)
      {
        this->cwgr = value;
      }

      inline uint32_t
      ModuleRegisters::readClockWaveformGenerator(void)
      {
        return this->cwgr;
      }

      inline void
      ModuleRegisters::writeSmbusTiming(uint32_t value)
      {
        this->smbtr = value;
      }

      inline uint32_t
      ModuleRegisters::readSmbusTiming(void)
      {
        return this->smbtr;
      }

      inline void
      ModuleRegisters::writeCommand(uint32_t value)
      {
        this->cmdr = value;
      }

      inline uint32_t
      ModuleRegisters::readCommand(void)
      {
        return this->cmdr;
      }

      inline void
      ModuleRegisters::writeNextCommand(uint32_t value)
      {
        this->ncmdr = value;
      }

      inline uint32_t
      ModuleRegisters::readNextCommand(void)
      {
        return this->ncmdr;
      }

      inline uint8_t
      ModuleRegisters::readReceiveHolding(void)
      {
        return this->rhr;
      }

      inline void
      ModuleRegisters::writeTransmitHolding(uint8_t value)
      {
        this->thr = value;
      }

      inline Status_t
      ModuleRegisters::readStatus(void)
      {
        return this->sr;
      }

      // Writing 1 to a bit will set IMR; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptEnable(uint16_t value)
      {
        this->ier = value;
      }

      // Writing 1 to a bit will clear IMR; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptDisable(uint16_t value)
      {
        this->idr = value;
      }

      inline uint16_t
      ModuleRegisters::readInterruptMask(void)
      {
        return this->imr;
      }

      inline void
      ModuleRegisters::writeStatusClear(uint16_t value)
      {
        this->scr = value;
      }

      inline uint32_t
      ModuleRegisters::readParameter(void)
      {
        return this->pr;
      }

      inline uint32_t
      ModuleRegisters::readVersion(void)
      {
        return this->vr;
      }

    // ----------------------------------------------------------------------

    }
  }
}

#endif /* AVR32_UC3_TWIM_DEFINITIONS_H_ */
