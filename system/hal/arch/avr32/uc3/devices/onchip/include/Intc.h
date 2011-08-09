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
      typedef uint8_t Priority_t;

      class Priority
      {
      public:
        const static Priority_t _0 = 0;
        const static Priority_t _1 = 1;
        const static Priority_t _2 = 2;
        const static Priority_t _3 = 3;
      };

      typedef void
      (*InterruptHandler_t)(void);

      class GroupPriority
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

      typedef uint_t InterruptIndex_t;

      class InterruptIndex
      {
      public:
        // Interrupt index is (Group * 32 + Line) in Table 10-2, page 104
        const static InterruptIndex_t EIC0 = 1 * 32 + 0;
        const static InterruptIndex_t EIC1 = 1 * 32 + 1;
        const static InterruptIndex_t EIC2 = 1 * 32 + 2;
        const static InterruptIndex_t EIC3 = 1 * 32 + 3;
      };

      typedef uint8_t Group_t;

      class Group
      {
      public:
        const static Group_t CPU = 0;
        const static Group_t EIC = 1;
        const static Group_t GPIO = 2;
        const static Group_t PDCA = 3;
        const static Group_t FLASHC = 4;
        const static Group_t USART0 = 5;
        const static Group_t USART1 = 6;
        const static Group_t USART2 = 7;
        const static Group_t USART3 = 8;
        const static Group_t SPI0 = 9;
        const static Group_t SPI1 = 10;
        const static Group_t TWIM0 = 11;
        const static Group_t TWIM1 = 12;
        const static Group_t SSC = 13;
        const static Group_t TC0 = 14;
        const static Group_t ADC = 15;
        const static Group_t TC1 = 16;
        const static Group_t USB = 17;
        const static Group_t SDRAMC = 18;
        const static Group_t ABDAC = 19;
        const static Group_t MCI = 20;
        const static Group_t AES = 21;
        const static Group_t DMACA = 22;
        const static Group_t MSI = 26;
        const static Group_t TWIS0 = 27;
        const static Group_t TWIS1 = 28;
        const static Group_t ECCHRS = 29;
      };

      typedef uint8_t Line_t;

      const static uint_t LINES_PER_GROUP = 32;
    }

    class Intc
    {
    public:
      Intc();

      static void
      registerInterruptHandler(intc::InterruptHandler_t handler,
          intc::InterruptIndex_t index, intc::Priority_t priority);

      static intc::InterruptIndex_t
      computeInterruptIndex(intc::Group_t group, intc::Line_t line);
    };

    inline intc::InterruptIndex_t
    Intc::computeInterruptIndex(intc::Group_t group, intc::Line_t line)
    {
      return ((group * intc::LINES_PER_GROUP) + line);
    }
  }
}

#endif /* AVR32_UC3_INTC_H_ */
