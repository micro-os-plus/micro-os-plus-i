/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_GPIO_H_
#define AVR32_UC3_GPIO_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    namespace gpio
    {
      // ----- Type definitions -----------------------------------------------

      typedef uint_t PinNumber_t;
      typedef enum PeripheralFunction_e
      {
        FUNCTION_A = 0, FUNCTION_B = 1, FUNCTION_C = 2, FUNCTION_D = 3
      } PeripheralFunction_t;

      // ----- Port memory mapped registers -----------------------------------

      class PortRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS = 0xFFFF1000;
        const static uint32_t MEMORY_OFFSET = 0x100;

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

      };

    }
    class Gpio
    {
    public:
      Gpio(gpio::PinNumber_t pin);

      void
      configModeGpio(void);
      void
      configModePeripheral(void);

      void
      configPeripheralFunction(gpio::PeripheralFunction_t f);

      void
      setPinHigh(void);
      void
      setPinLow(void);
      void
      togglePin(void);

      bool
      isPinHigh(void);

      // Static methods to be used without a dedicated object,
      // for example when setting pins for another terminal

      static void
      configPeripheralModeAndFunction(gpio::PinNumber_t pin,
          gpio::PeripheralFunction_t f);

    public:
      gpio::PortRegisters& portRegisters;

    private:
      uint32_t m_mask;
    };

    inline void
    Gpio::configModeGpio(void)
    {
      portRegisters.gpers = m_mask;
    }

    inline void
    Gpio::configModePeripheral(void)
    {
      portRegisters.gperc = m_mask;
    }

    inline void
    Gpio::setPinHigh(void)
    {
      portRegisters.ovrs = m_mask;
    }

    inline void
    Gpio::setPinLow(void)
    {
      portRegisters.ovrc = m_mask;
    }

    inline void
    Gpio::togglePin(void)
    {
      portRegisters.ovrt = m_mask;
    }

    inline bool
    Gpio::isPinHigh(void)
    {
      return (portRegisters.pvr & m_mask) != 0;
    }
  }
}

#endif /* AVR32_UC3_GPIO_H_ */
