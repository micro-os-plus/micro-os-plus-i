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
      class Registers
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS = 0xFFFF0D00;

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

      // ----------------------------------------------------------------------
#if false
      // in this context 'static' means visible only within this file
      static Registers& registers =
          *reinterpret_cast<Registers*> (Registers::MEMORY_ADDRESS);
#endif

      // ----- Inline implementation ------------------------------------------

      inline void
      Registers::writeControl(uint32_t v)
      {
        this->ctrl = v;
      }

      inline uint32_t
      Registers::readControl(void)
      {
        return this->ctrl;
      }

      inline void
      Registers::writeValue(uint_t v)
      {
        this->val = v;
      }

      inline uint_t
      Registers::readValue(void)
      {
        return this->val;
      }

      inline void
      Registers::writeTop(uint_t v)
      {
        this->top = v;
      }

      inline uint_t
      Registers::readTop(void)
      {
        return this->top;
      }

      inline void
      Registers::interruptEnable(void)
      {
        this->ier = 1;
      }

      inline void
      Registers::interruptDisable(void)
      {
        this->idr = 1;
      }

      inline bool
      Registers::isInterruptEnabled(void)
      {
        return ((this->imr & 1) != 0);
      }

      inline bool
      Registers::isInterruptRequested(void)
      {
        return ((this->isr & 1) != 0);
      }

      inline void
      Registers::clearInterrupt(void)
      {
        this->icr = 1;
      }
    }

    // ------------------------------------------------------------------------

    class Rtc
    {
    public:
      Rtc();

      rtc::Registers& registers;

    };
  }
}

#endif /* AVR32_UC3_RTC_H_ */
