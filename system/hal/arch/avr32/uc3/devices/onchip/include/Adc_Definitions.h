/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_ADC_DEFINITIONS_H_
#define AVR32_UC3_ADC_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace adc
    {
      static const uint32_t START_MASK = 0x00000002;
      static const uint32_t LOWRES_OFFSET = 4;
      static const uint32_t MR_PRESCAL_OFFSET = 8;
      static const uint32_t SHTIM_OFFSET = 24;
      static const uint32_t STARTUP_OFFSET = 16;
      static const uint32_t SLEEP_OFFSET = 5;
      static const uint32_t CR_SWRST_MASK = 0x00000001;
      static const uint32_t MAX_VALUE_10_BITS = 0x3FF;

      // ----- Type definitions -----------------------------------------------

      typedef uint8_t ChannelId_t;
      typedef uint8_t ChannelsMask_t;
      typedef uint32_t Mask_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS = 0xFFFF3C00;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regNotAllocated_t dummy08[2]; //0x0008-0x000c
        regWriteOnly_t cher; //0x0010
        regWriteOnly_t chdr; //0x0014
        regReadOnly_t chsr; //0x0018
        regReadOnly_t sr; //0x001c
        regReadOnly_t lcdr; //0x0020
        regWriteOnly_t ier; //0x0024
        regWriteOnly_t idr; //0x0028
        regReadOnly_t imr; //0x002c
        regReadOnly_t cdr[8]; //0x0030
        regNotAllocated_t dummy50[43]; //0x0050-0x00f8
        regReadOnly_t version;//0x00fc

        // ----- Methods ------------------------------------------------------

        void
        writeControl(Mask_t);

        void
        writeMode(Mask_t value);

        Mask_t
        readMode(void);

        void
        writeChannelEnable(Mask_t value);

        void
        writeChannelDisable(Mask_t value);

        Mask_t
        readChannelStatus(void);

        Mask_t
        readStatus(void);

        Mask_t
        readLastConvertedData(void);

        void
        writeInterruptEnable(Mask_t value);

        void
        writeInterruptDisable(Mask_t value);

        Mask_t
        readInterruptMask(void);

        Mask_t
        readChannelData(ChannelId_t channel);

        Mask_t
        readVersion(void);
      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeControl(Mask_t value)
      {
        cr = value;
      }

      inline void
      ModuleRegisters::writeMode(Mask_t value)
      {
        mr = value;
      }

      inline Mask_t
      ModuleRegisters::readMode(void)
      {
        return mr;
      }

      inline void
      ModuleRegisters::writeChannelEnable(Mask_t value)
      {
        cher = value;
      }

      inline void
      ModuleRegisters::writeChannelDisable(Mask_t value)
      {
        chdr = value;
      }

      inline Mask_t
      ModuleRegisters::readChannelStatus(void)
      {
        return chsr;
      }

      inline Mask_t
      ModuleRegisters::readStatus(void)
      {
        return sr;
      }

      inline Mask_t
      ModuleRegisters::readLastConvertedData(void)
      {
        return lcdr;
      }

      inline void
      ModuleRegisters::writeInterruptEnable(Mask_t value)
      {
        ier = value;
      }

      inline void
      ModuleRegisters::writeInterruptDisable(Mask_t value)
      {
        idr = value;
      }

      inline Mask_t
      ModuleRegisters::readInterruptMask(void)
      {
        return imr;
      }

      inline Mask_t
      ModuleRegisters::readChannelData(ChannelId_t channel)
      {
        return cdr[channel];
      }

      inline Mask_t
      ModuleRegisters::readVersion(void)
      {
        return version;
      }
    }
  }
}

#endif /* AVR32_UC3_ADC_DEFINITIONS_H_ */
