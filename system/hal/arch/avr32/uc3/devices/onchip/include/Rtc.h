/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_RTC_H_
#define AVR32_UC3_RTC_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace rtc
    {
      class ModuleRegisters
      {
      public:

        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::RTC;

        // ----- Memory Map ---------------------------------------------------

        regReadWrite_t ctrl;
        regReadWrite_t val;
        regReadWrite_t top;
        regWriteOnly_t ier;
        regWriteOnly_t idr;
        regReadOnly_t imr;
        regReadOnly_t isr;
        regWriteOnly_t icr;

        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint32_t v);
        uint32_t
        readControl(void);

        void
        writeValue(uint_t v);
        uint_t
        readValue(void);

        void
        writeTop(uint_t v);
        uint_t
        readTop(void);

        void
        interruptEnable(void);
        void
        interruptDisable(void);

        bool
        isInterruptEnabled(void);
        bool
        isInterruptRequested(void);
        void
        clearInterrupt(void);
      };


      // ----- Inline implementation ------------------------------------------

      inline void
      ModuleRegisters::writeControl(uint32_t v)
      {
        this->ctrl = v;
      }

      inline uint32_t
      ModuleRegisters::readControl(void)
      {
        return this->ctrl;
      }

      inline void
      ModuleRegisters::writeValue(uint_t v)
      {
        this->val = v;
      }

      inline uint_t
      ModuleRegisters::readValue(void)
      {
        return this->val;
      }

      inline void
      ModuleRegisters::writeTop(uint_t v)
      {
        this->top = v;
      }

      inline uint_t
      ModuleRegisters::readTop(void)
      {
        return this->top;
      }

      inline void
      ModuleRegisters::interruptEnable(void)
      {
        this->ier = 1;
      }

      inline void
      ModuleRegisters::interruptDisable(void)
      {
        this->idr = 1;
      }

      inline bool
      ModuleRegisters::isInterruptEnabled(void)
      {
        return ((this->imr & 1) != 0);
      }

      inline bool
      ModuleRegisters::isInterruptRequested(void)
      {
        return ((this->isr & 1) != 0);
      }

      inline void
      ModuleRegisters::clearInterrupt(void)
      {
        this->icr = 1;
      }
    }

    // ------------------------------------------------------------------------

    class Rtc
    {
    public:
      Rtc();

      rtc::ModuleRegisters& registers;

      void
      resetCounter(void);
    };
  }
}

#endif /* AVR32_UC3_RTC_H_ */
