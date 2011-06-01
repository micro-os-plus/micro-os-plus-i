/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_ADC_H_
#define AVR32_UC3_ADC_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace adc
    {
      // ----- Type definitions -----------------------------------------------

      // ----- Port memory mapped registers -----------------------------------

      class Registers
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
        regReadOnly_t cdr0; //0x0030
        regReadOnly_t cdr1; //0x0034
        regReadOnly_t cdr2; //0x0038
        regReadOnly_t cdr3; //0x003c
        regReadOnly_t cdr4; //0x0040
        regReadOnly_t cdr5; //0x0044
        regReadOnly_t cdr6; //0x0048
        regReadOnly_t cdr7; //0x004c
        regNotAllocated_t dummy50[43]; //0x0050-0x00f8
        regReadOnly_t version;//0x00fc

        // ----- Methods ------------------------------------------------------

        // TODO: add methods, if needed

      };
    }

    class Adc
    {
    public:
      Adc();
    };
  }
}

#endif /* AVR32_UC3_ADC_H_ */
