/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_GPIO_DEFINITIONS_H_
#define AVR32_UC3_GPIO_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

//#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
//#include "hal/arch/avr32/lib/include/compiler.h"

namespace avr32
{
  namespace uc3
  {
    namespace gpio
    {

      // ----- Type definitions -----------------------------------------------

      typedef uint8_t PinNumber_t;

      const static PinNumber_t NOT_A_PIN = 0xFF;

      typedef uint8_t PeripheralFunction_t;

      class PeripheralFunction
      {
      public:
        const static PeripheralFunction_t A = 0;
        const static PeripheralFunction_t B = 1;
        const static PeripheralFunction_t C = 2;
        const static PeripheralFunction_t D = 3;
      };

      typedef uint_t InterruptMode_t;

      class InterruptMode
      {
      public:
        const static InterruptMode_t PIN_CHANGE = 0;
        const static InterruptMode_t RISING_EDGE = 1;
        const static InterruptMode_t FAILING_EDGE = 2;
      };

      class PinPeripheralFunction
      {
      public:
        PinNumber_t pin;
        PeripheralFunction_t function;
      };

      typedef const class PinPeripheralFunction PinPeripheralFunction_t;

      //const static uint_t INTERRUPT_BASE = 64;

      // ----- Port memory mapped registers -----------------------------------

      class PortRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::GPIO;
        const static uint32_t MEMORY_OFFSET = 0x0100;

        // ----- Memory map ---------------------------------------------------

        regReadWrite_t gper; //0x0000
        regWriteOnly_t gpers; //0x0004
        regWriteOnly_t gperc; //0x0008
        regWriteOnly_t gpert; //0x000c

        regReadWrite_t pmr0; //0x0010
        regWriteOnly_t pmr0s; //0x0014
        regWriteOnly_t pmr0c; //0x0018
        regWriteOnly_t pmr0t; //0x001c

        regReadWrite_t pmr1; //0x0020
        regWriteOnly_t pmr1s; //0x0024
        regWriteOnly_t pmr1c; //0x0028
        regWriteOnly_t pmr1t; //0x002c

        regNotAllocated_t dummy30; //0x0030
        regNotAllocated_t dummy34; //0x0034
        regNotAllocated_t dummy38; //0x0038
        regNotAllocated_t dummy3C; //0x003c

        regReadWrite_t oder; //0x0040
        regWriteOnly_t oders; //0x0044
        regWriteOnly_t oderc; //0x0048
        regWriteOnly_t odert; //0x004c

        regReadWrite_t ovr; //0x0050
        regWriteOnly_t ovrs; //0x0054
        regWriteOnly_t ovrc; //0x0058
        regWriteOnly_t ovrt; //0x005c

        regReadOnly_t pvr; //0x0060
        regNotAllocated_t dummy64; //0x0064
        regNotAllocated_t dummy68; //0x0068
        regNotAllocated_t dummy6C; //0x006c

        regReadWrite_t puer; //0x0070
        regWriteOnly_t puers; //0x0074
        regWriteOnly_t puerc; //0x0078
        regWriteOnly_t puert; //0x007c

        regReadWrite_t odmer; //0x0080
        regWriteOnly_t odmers; //0x0084
        regWriteOnly_t odmerc; //0x0088
        regWriteOnly_t odmert; //0x008c

        regReadWrite_t ier; //0x0090
        regWriteOnly_t iers; //0x0094
        regWriteOnly_t ierc; //0x0098
        regWriteOnly_t iert; //0x009c

        regReadWrite_t imr0; //0x00a0
        regWriteOnly_t imr0s; //0x00a4
        regWriteOnly_t imr0c; //0x00a8
        regWriteOnly_t imr0t; //0x00ac

        regReadWrite_t imr1; //0x00b0
        regWriteOnly_t imr1s; //0x00b4
        regWriteOnly_t imr1c; //0x00b8
        regWriteOnly_t imr1t; //0x00bc

        regReadWrite_t gfer; //0x00c0
        regWriteOnly_t gfers; //0x00c4
        regWriteOnly_t gferc; //0x00c8
        regWriteOnly_t gfert; //0x00cc

        regReadOnly_t ifr; //0x00d0
        regNotAllocated_t dummyD4; //0x00d4
        regWriteOnly_t ifrc; //0x00d8
        regNotAllocated_t dummyDC; //0x00dc

        // ----- Methods ------------------------------------------------------

        // TODO: add methods, if needed

        // --------------------------------------------------------------------

      };
    }
  }
}

#endif /* AVR32_UC3_GPIO_DEFINITIONS_H_ */
