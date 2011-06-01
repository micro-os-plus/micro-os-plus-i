/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_COMMON_H_
#define AVR32_UC3_COMMON_H_

//#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    typedef uint32_t volatile regReadWrite_t;
    typedef uint32_t const volatile regReadOnly_t;
    typedef uint32_t regWriteOnly_t;
    typedef uint32_t regNotAllocated_t;

    class PeripheralAddressMap
    {
    public:
      static const uint32_t DMACA = 0xFF100000;
      static const uint32_t USB = 0xFFFE0000;
      static const uint32_t MCI = 0xFFFE4000;
      static const uint32_t PDCA = 0xFFFF0000;
      static const uint32_t INTC = 0xFFFF0800;
      static const uint32_t PM = 0xFFFF0C00;
      static const uint32_t RTC = 0xFFFF0D00;
      static const uint32_t WDT = 0xFFFF0D30;
      static const uint32_t GPIO = 0xFFFF1000;
      static const uint32_t USART0 = 0xFFFF1400;
      static const uint32_t USART1 = 0xFFFF1800;
      static const uint32_t USART2 = 0xFFFF1C00;
      static const uint32_t USART3 = 0xFFFF2000;
      static const uint32_t SPI0 = 0xFFFF2400;
      static const uint32_t SPI1 = 0xFFFF2800;
      static const uint32_t TWIM0 = 0xFFFF2C00;
      static const uint32_t TWIM1 = 0xFFFF3000;
      static const uint32_t TC0 = 0xFFFF3800;
      static const uint32_t ADC = 0xFFFF3C00;
      static const uint32_t TC1 = 0xFFFF4400;
    };
  }
}

#endif /* AVR32_UC3_COMMON_H_ */
