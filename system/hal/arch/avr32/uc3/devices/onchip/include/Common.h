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
    // Register types
    typedef uint32_t volatile regReadWrite_t;
    typedef uint32_t const volatile regReadOnly_t;
    typedef uint32_t volatile regWriteOnly_t;
    typedef uint32_t regNotAllocated_t;

    // Table 5-2, page 36-37
    class PeripheralAddressMap
    {
    public:
      const static uint32_t DMACA = 0xFF100000;
      const static uint32_t USB = 0xFFFE0000;
      const static uint32_t HMATRIX = 0xFFFE1000;
      const static uint32_t FLASHC = 0xFFFE1400;
      const static uint32_t SMC = 0xFFFE1C00;
      const static uint32_t SDRAMC = 0xFFFE2000;
      const static uint32_t ECCHRS = 0xFFFE2400;
      const static uint32_t BUSMON = 0xFFFE2800;
      const static uint32_t MCI = 0xFFFE4000;
      const static uint32_t MSI = 0xFFFE8000;
      const static uint32_t PDCA = 0xFFFF0000;
      const static uint32_t INTC = 0xFFFF0800;
      const static uint32_t PM = 0xFFFF0C00;
      const static uint32_t RTC = 0xFFFF0D00;
      const static uint32_t WDT = 0xFFFF0D30;
      const static uint32_t EIC = 0xFFFF0D80;
      const static uint32_t GPIO = 0xFFFF1000;
      const static uint32_t USART0 = 0xFFFF1400;
      const static uint32_t USART1 = 0xFFFF1800;
      const static uint32_t USART2 = 0xFFFF1C00;
      const static uint32_t USART3 = 0xFFFF2000;
      const static uint32_t SPI0 = 0xFFFF2400;
      const static uint32_t SPI1 = 0xFFFF2800;
      const static uint32_t TWIM0 = 0xFFFF2C00;
      const static uint32_t TWIM1 = 0xFFFF3000;
      const static uint32_t TC0 = 0xFFFF3800;
      const static uint32_t ADC = 0xFFFF3C00;
      const static uint32_t TC1 = 0xFFFF4400;
      const static uint32_t TWIS0 = 0xFFFF5000;
      const static uint32_t TWIS1 = 0xFFFF5400;
    };

    /*
     * For clocks see Table 7-7, page 58.
     */
  }
}

#endif /* AVR32_UC3_COMMON_H_ */
