/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_INTC_H_
#define AVR32_UC3_INTC_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace intc
    {
      typedef enum Priority_e
      {
        PRIORITY_0 = 0, PRIORITY_1, PRIORITY_2, PRIORITY_3 = 3
      } Priority_t;

      typedef uint_t InterruptIndex_t;

      typedef void
      (*InterruptHandler_t)(void);

      class GroupPriorities
      {
      public:
        // CPU
        static const Priority_t GROUP_00 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP00_PRIORITY;
        // EINT, RTC, PWR
        static const Priority_t GROUP_01 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP01_PRIORITY;
        // GPIO00-13
        static const Priority_t GROUP_02 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP02_PRIORITY;
        // PDMA0-7
        static const Priority_t GROUP_03 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP03_PRIORITY;
        static const Priority_t GROUP_04 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP04_PRIORITY;
        static const Priority_t GROUP_05 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP05_PRIORITY;
        static const Priority_t GROUP_06 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP06_PRIORITY;
        static const Priority_t GROUP_07 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP07_PRIORITY;
        static const Priority_t GROUP_08 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP08_PRIORITY;
        static const Priority_t GROUP_09 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP09_PRIORITY;
        static const Priority_t GROUP_10 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP10_PRIORITY;
        static const Priority_t GROUP_11 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP11_PRIORITY;
        static const Priority_t GROUP_12 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP12_PRIORITY;
        static const Priority_t GROUP_13 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP13_PRIORITY;
        static const Priority_t GROUP_14 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP14_PRIORITY;
        static const Priority_t GROUP_15 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP15_PRIORITY;
        static const Priority_t GROUP_16 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP16_PRIORITY;
        static const Priority_t GROUP_17 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP17_PRIORITY;
        static const Priority_t GROUP_18 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP18_PRIORITY;
        static const Priority_t GROUP_19 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP19_PRIORITY;
        static const Priority_t GROUP_20 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP20_PRIORITY;
        static const Priority_t GROUP_21 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP21_PRIORITY;
        static const Priority_t GROUP_22 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP22_PRIORITY;
        static const Priority_t GROUP_23 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP23_PRIORITY;
        static const Priority_t GROUP_24 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP24_PRIORITY;
        static const Priority_t GROUP_25 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP25_PRIORITY;
        static const Priority_t GROUP_26 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP26_PRIORITY;
        static const Priority_t GROUP_27 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP27_PRIORITY;
        static const Priority_t GROUP_28 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP28_PRIORITY;
        static const Priority_t GROUP_29 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP29_PRIORITY;
      };
    }
    class Intc
    {
    public:
      Intc();

      static void
      registerInterruptHandler(intc::InterruptHandler_t handler,
          intc::InterruptIndex_t index, intc::Priority_t priority);

    };
  }
}

#endif /* AVR32_UC3_INTC_H_ */
