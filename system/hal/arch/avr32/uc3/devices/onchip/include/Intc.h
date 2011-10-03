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
        const static Priority_t CPU = OS_CFGINT_AVR32_UC3_INTC_GROUP00_PRIORITY;
        // EINT, RTC, PWR
        const static Priority_t EIC = OS_CFGINT_AVR32_UC3_INTC_GROUP01_PRIORITY;
        // GPIO00-13
        const static Priority_t GPIO =
            OS_CFGINT_AVR32_UC3_INTC_GROUP02_PRIORITY;
        // PDMA0-7
        const static Priority_t PDCA =
            OS_CFGINT_AVR32_UC3_INTC_GROUP03_PRIORITY;
        const static Priority_t FLASHC =
            OS_CFGINT_AVR32_UC3_INTC_GROUP04_PRIORITY;
        const static Priority_t USART0 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP05_PRIORITY;
        const static Priority_t USART1 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP06_PRIORITY;
        const static Priority_t USART2 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP07_PRIORITY;
        const static Priority_t USART3 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP08_PRIORITY;
        const static Priority_t SPI0 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP09_PRIORITY;
        const static Priority_t SPI1 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP10_PRIORITY;
        const static Priority_t TWIM0 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP11_PRIORITY;
        const static Priority_t TWIM1 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP12_PRIORITY;
        const static Priority_t SSC = OS_CFGINT_AVR32_UC3_INTC_GROUP13_PRIORITY;
        const static Priority_t TC0 = OS_CFGINT_AVR32_UC3_INTC_GROUP14_PRIORITY;
        const static Priority_t ADC = OS_CFGINT_AVR32_UC3_INTC_GROUP15_PRIORITY;
        const static Priority_t TC1 = OS_CFGINT_AVR32_UC3_INTC_GROUP16_PRIORITY;
        const static Priority_t USB = OS_CFGINT_AVR32_UC3_INTC_GROUP17_PRIORITY;
        const static Priority_t SDRAMC =
            OS_CFGINT_AVR32_UC3_INTC_GROUP18_PRIORITY;
        const static Priority_t ABDAC =
            OS_CFGINT_AVR32_UC3_INTC_GROUP19_PRIORITY;
        const static Priority_t MCI = OS_CFGINT_AVR32_UC3_INTC_GROUP20_PRIORITY;
        const static Priority_t AES = OS_CFGINT_AVR32_UC3_INTC_GROUP21_PRIORITY;
        const static Priority_t DMACA =
            OS_CFGINT_AVR32_UC3_INTC_GROUP22_PRIORITY;
        const static Priority_t GROUP_23 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP23_PRIORITY;
        const static Priority_t GROUP_24 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP24_PRIORITY;
        const static Priority_t GROUP_25 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP25_PRIORITY;
        const static Priority_t MSI = OS_CFGINT_AVR32_UC3_INTC_GROUP26_PRIORITY;
        const static Priority_t TWIS0 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP27_PRIORITY;
        const static Priority_t TWIS1 =
            OS_CFGINT_AVR32_UC3_INTC_GROUP28_PRIORITY;
        const static Priority_t ECCHRS =
            OS_CFGINT_AVR32_UC3_INTC_GROUP29_PRIORITY;
      };

      typedef uint_t InterruptIndex_t;

      // Interrupt index is (Group * 32 + Line) in Table 10-2, page 104

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

      static void
      registerInterruptHandler(intc::InterruptHandler_t handler,
          intc::Group_t group, intc::Line_t line, intc::Priority_t priority);

      static intc::InterruptIndex_t
      computeInterruptIndex(intc::Group_t group, intc::Line_t line);
    };

    inline intc::InterruptIndex_t
    Intc::computeInterruptIndex(intc::Group_t group, intc::Line_t line)
    {
      return ((group * intc::LINES_PER_GROUP) + line);
    }

    void
    inline
    Intc::registerInterruptHandler(intc::InterruptHandler_t handler,
        intc::Group_t group, intc::Line_t line, intc::Priority_t priority)
    {
      registerInterruptHandler(handler, computeInterruptIndex(group, line),
          priority);
    }
  }
}

#endif /* AVR32_UC3_INTC_H_ */
