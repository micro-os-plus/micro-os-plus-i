/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_EIC_DEFINITIONS_H_
#define AVR32_UC3_EIC_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace eic
    {
      // ----- Type definitions -----------------------------------------------

      typedef uint8_t InterruptId_t;

      class InterruptId
      {
      public:
        const static InterruptId_t INT0 = 0;
        const static InterruptId_t INT1 = 1;
        const static InterruptId_t INT2 = 2;
        const static InterruptId_t INT3 = 3;
        const static InterruptId_t INT4 = 4;
        const static InterruptId_t INT5 = 5;
        const static InterruptId_t INT6 = 6;
        const static InterruptId_t INT7 = 7;
        const static InterruptId_t NMI = 8;
      };
      typedef uint16_t Mask_t;

      typedef uint8_t Prescaller_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:

        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::EIC;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t ier; //0x0000
        regWriteOnly_t idr; //0x0004
        regReadOnly_t imr; // 0x0008
        regReadOnly_t isr; // 0x000C

        regWriteOnly_t icr; //0x0010
        regReadWrite_t mode; //0x0014
        regReadWrite_t edge; //0x0018
        regReadWrite_t level; //0x001C

        regReadWrite_t filter; //0x0020
        regReadWrite_t test; //0x0024
        regReadWrite_t async; //0x0028
        regReadWrite_t scan; //0x002C

        regWriteOnly_t en; //0x0030
        regWriteOnly_t dis; //0x0034
        regReadOnly_t ctrl; // 0x0038


        // ----- Methods ------------------------------------------------------

        void
        writeInterruptEnable(Mask_t value);

        void
        writeInterruptDisable(Mask_t value);

        Mask_t
        readInterruptMask(void);

        Mask_t
        readInterruptStatus(void);

        void
        writeInterruptClear(Mask_t value);

        void
        writeMode(Mask_t value);

        Mask_t
        readMode(void);

        void
        writeEdge(Mask_t value);

        Mask_t
        readEdge(void);

        void
        writeLevel(Mask_t value);

        Mask_t
        readLevel(void);

        void
        writeFilter(Mask_t value);

        Mask_t
        readFilter(void);

        void
        writeTest(Mask_t value);

        Mask_t
        readTest(void);

        void
        writeAsync(Mask_t value);

        Mask_t
        readAsync(void);

        void
        writeScan(uint32_t value);

        uint32_t
        readScan(void);

        void
        writeEnable(Mask_t value);

        void
        writeDisable(Mask_t value);

        Mask_t
        readControl(void);

      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeInterruptEnable(Mask_t value)
      {
        this->ier = value;
      }

      inline void
      ModuleRegisters::writeInterruptDisable(Mask_t value)
      {
        this->idr = value;
      }

      inline Mask_t
      ModuleRegisters::readInterruptMask(void)
      {
        return this->imr;
      }

      inline Mask_t
      ModuleRegisters::readInterruptStatus(void)
      {
        return this->isr;
      }

      inline void
      ModuleRegisters::writeInterruptClear(Mask_t value)
      {
        this->icr = value;
      }

      inline void
      ModuleRegisters::writeMode(Mask_t value)
      {
        this->mode = value;
      }

      inline Mask_t
      ModuleRegisters::readMode(void)
      {
        return this->mode;
      }

      inline void
      ModuleRegisters::writeEdge(Mask_t value)
      {
        this->edge = value;
      }

      inline Mask_t
      ModuleRegisters::readEdge(void)
      {
        return this->edge;
      }

      inline void
      ModuleRegisters::writeLevel(Mask_t value)
      {
        this->level = value;
      }

      inline Mask_t
      ModuleRegisters::readLevel(void)
      {
        return this->level;
      }

      inline void
      ModuleRegisters::writeFilter(Mask_t value)
      {
        this->filter = value;
      }

      inline Mask_t
      ModuleRegisters::readFilter(void)
      {
        return this->filter;
      }

      inline void
      ModuleRegisters::writeTest(Mask_t value)
      {
        this->test = value;
      }

      inline Mask_t
      ModuleRegisters::readTest(void)
      {
        return this->test;
      }

      inline void
      ModuleRegisters::writeAsync(Mask_t value)
      {
        this->async = value;
      }

      inline Mask_t
      ModuleRegisters::readAsync(void)
      {
        return this->async;
      }

      inline void
      ModuleRegisters::writeScan(uint32_t value)
      {
        this->scan = value;
      }

      inline uint32_t
      ModuleRegisters::readScan(void)
      {
        return this->scan;
      }

      inline void
      ModuleRegisters::writeEnable(Mask_t value)
      {
        this->en = value;
      }

      inline void
      ModuleRegisters::writeDisable(Mask_t value)
      {
        this->dis = value;
      }

      inline Mask_t
      ModuleRegisters::readControl(void)
      {
        return this->ctrl;
      }

    // ----------------------------------------------------------------------

    }
  }
}

#endif /* AVR32_UC3_template_DEFINITIONS_H_ */
