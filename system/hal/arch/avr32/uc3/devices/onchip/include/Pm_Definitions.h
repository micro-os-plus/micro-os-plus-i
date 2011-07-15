/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_PM_DEFINITIONS_H_
#define AVR32_UC3_PM_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace pm
    {
      // ----- Type definitions -----------------------------------------------

      //      typedef uint32_t Command_t;

      //      class BusWidth
      //      {
      //      public:
      //        const static BusWidth_t _1bit = 0;
      //      };

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::PM;

        // ----- Memory map ---------------------------------------------------

        regReadWrite_t mcctrl; //0x0000
        regReadWrite_t cksel; //0x0004
        regReadWrite_t cpumask; //0x0008
        regReadWrite_t hsbmask; //0x000C

        regReadWrite_t pbamask; //0x0010
        regReadWrite_t pbbmask; //0x0014
        regNotAllocated_t
            dummy18[(0x0020 - 0x0018) / sizeof(regNotAllocated_t)]; //0x0018-0x001C

        regReadWrite_t pll0; //0x0020
        regReadWrite_t pll1; //0x0024
        regReadWrite_t osctrl0; //0x0028
        regReadWrite_t osctrl1; //0x002C

        regReadWrite_t osctrl32; //0x0030
        regNotAllocated_t
            dummy34[(0x0040 - 0x0034) / sizeof(regNotAllocated_t)]; //0x0034-0x003C

        regWriteOnly_t ier; //0x0040
        regWriteOnly_t idr; //0x0044
        regReadOnly_t imr; //0x0048
        regReadOnly_t isr; //0x004C

        regWriteOnly_t icr; //0x0050
        regReadWrite_t poscsr; //0x0054
        regNotAllocated_t
            dummy58[(0x0060 - 0x0058) / sizeof(regNotAllocated_t)]; //0x0058-0x005C

        regReadWrite_t gcctrl[6]; //0x0060-0x0074
        regNotAllocated_t
            dummy78[(0x00C0 - 0x0078) / sizeof(regNotAllocated_t)]; //0x0078-0x00BC

        regReadWrite_t rccr; //0x00C0
        regReadWrite_t bgcr; //0x00C4
        regReadWrite_t vregcr; //0x00C8
        regNotAllocated_t dummyCC; //0x00CC

        regReadWrite_t bod; //0x00D0
        regReadWrite_t bod33; //0x00D4
        regNotAllocated_t
            dummyD8[(0x0140 - 0x00D8) / sizeof(regNotAllocated_t)]; //0x00D8-0x013C

        regReadWrite_t rcause; //0x0140
        regReadWrite_t awen; //0x0144
        regNotAllocated_t dummy148[(0x0200 - 0x0148)
            / sizeof(regNotAllocated_t)]; //0x0148-0x01FC

        regReadWrite_t gplp; //0x0200

      public:

        // ----- Methods ------------------------------------------------------

        uint32_t
        readCpuClockMask(void);

        void
        writeCpuClockMask(uint32_t mask);

        uint32_t
        readHsbClockMask(void);

        void
        writeHsbClockMask(uint32_t mask);

        uint32_t
        readPbaClockMask(void);

        void
        writePbaClockMask(uint32_t mask);

        uint32_t
        readPbbClockMask(void);

        void
        writePbbClockMask(uint32_t mask);

      };

      // ----- Inline methods -------------------------------------------------

      // TODO: fill-in, as needed

      inline uint32_t
      ModuleRegisters::readCpuClockMask(void)
      {
        return this->cpumask;
      }

      inline void
      ModuleRegisters::writeCpuClockMask(uint32_t mask)
      {
        this->cpumask = mask;
      }

      inline uint32_t
      ModuleRegisters::readHsbClockMask(void)
      {
        return this->hsbmask;
      }

      inline void
      ModuleRegisters::writeHsbClockMask(uint32_t mask)
      {
        this->hsbmask = mask;
      }

      inline uint32_t
      ModuleRegisters::readPbaClockMask(void)
      {
        return this->pbamask;
      }

      inline void
      ModuleRegisters::writePbaClockMask(uint32_t mask)
      {
        this->pbamask = mask;
      }

      inline uint32_t
      ModuleRegisters::readPbbClockMask(void)
      {
        return this->pbbmask;
      }

      inline void
      ModuleRegisters::writePbbClockMask(uint32_t mask)
      {
        this->pbbmask = mask;
      }

    }
  }
}

#endif /* AVR32_UC3_PM_DEFINITIONS_H_ */
