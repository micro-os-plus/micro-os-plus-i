/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_template_DEFINITIONS_H_
#define AVR32_UC3_template_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace TEMPLATE
    {
      // ----- Type definitions -----------------------------------------------

      typedef uint8_t ModuleId_t;

      class ModuleId      // Preserve the 'Id'
      {
      public:
        const static ModuleId_t USART0 = 0;
        const static ModuleId_t USART1 = 1;
        const static ModuleId_t USART2 = 2;
        const static ModuleId_t USART3 = 3;
      };

      typedef uint32_t StatusRegister_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:

        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::TEMPLATE0;
        const static uint32_t MEMORY_OFFSET = 0x0400;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regWriteOnly_t ier; //0x0008
        regWriteOnly_t idr; //0x000C

        regReadOnly_t imr; // 0x0010


        regNotAllocated_t
            dummy38[(0x0040 - 0x0038) / sizeof(regNotAllocated_t)]; //0x0038-0x003C


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
        readVersion(void);

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
      ModuleRegisters::readVersion(void)
      {
        return this->version;
      }

      // ----------------------------------------------------------------------

    }
  }
}

#endif /* AVR32_UC3_template_DEFINITIONS_H_ */
