/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_GPIO_H_
#define AVR32_UC3_GPIO_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/lib/include/compiler.h"

#define AVR32_UC3_GPIO_PIN_INTERRUPT 0

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

      typedef enum InterruptMode_e
      {
        PIN_CHANGE = 0, RISING_EDGE = 1, FAILING_EDGE = 2
      } InterruptMode_t;

      const static uint_t INTERRUPT_BASE = 64;

      // ----- Port memory mapped registers -----------------------------------

      class PortRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::GPIO;
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

      static void
      init(void);

      void
      setModeGpio(void);
      void
      setModePeripheral(void);
      void
      toggleMode(void);
      bool
      isModeGpio(void);

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

      void
      setDirectionOutput(void);
      void
      setDirectionInput(void);
      void
      toggleDirection(void);
      bool
      isDirectionOutputEnabled(void);

      void
      enablePullup(void);
      void
      disablePullup(void);
      void
      togglePullup(void);
      bool
      isPullupEnabled(void);

      void
      enableInterrupt(void);
      void
      disableInterrupt(void);
      void
      toggleInterrupt(void);
      bool
      isInterruptEnabled(void);

      void
      enableGlitchFilter(void);
      void
      disableGlitchFilter(void);
      void
      toggleGlitchFilter(void);
      bool
      isGlitchFilterEnabled(void);

      bool
      isInterruptRequested(void);
      void
      clearInterruptRequested(void);

      void
      configInterruptMode(gpio::InterruptMode_t mode);

      void
      registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler);

      // Static method to be used at system init to enable local bus
      // (mandatory, otherwise all local bus peripheral calls will fail).
      static void
      configLocalBus(void);

      // Static methods to be used without a dedicated object,
      // for example when setting pins for another terminal

      static void
      configPeripheralModeAndFunction(gpio::PinNumber_t pin,
          gpio::PeripheralFunction_t func);

    public:
      volatile gpio::PortRegisters& portRegisters;

    private:

#if AVR32_UC3_GPIO_PIN_INTERRUPT
      static volatile gpio::PortRegisters* portRegistersArray[4];

      // private methods
      static void lineHandler(uint8_t line);
      static __attribute__((__interrupt__)) void lineHandler0(void);
      static __attribute__((__interrupt__)) void lineHandler1(void);
      static __attribute__((__interrupt__)) void lineHandler2(void);
      static __attribute__((__interrupt__)) void lineHandler3(void);
      static __attribute__((__interrupt__)) void lineHandler4(void);
      static __attribute__((__interrupt__)) void lineHandler5(void);
      static __attribute__((__interrupt__)) void lineHandler6(void);
      static __attribute__((__interrupt__)) void lineHandler7(void);
      static __attribute__((__interrupt__)) void lineHandler8(void);
      static __attribute__((__interrupt__)) void lineHandler9(void);
      static __attribute__((__interrupt__)) void lineHandler10(void);
      static __attribute__((__interrupt__)) void lineHandler11(void);
      static __attribute__((__interrupt__)) void lineHandler12(void);
      static __attribute__((__interrupt__)) void lineHandler13(void);
#endif // AVR32_UC3_GPIO_PIN_INTERRUPT

      // private data
      uint32_t m_mask;
      uint16_t m_pin;

#if AVR32_UC3_GPIO_PIN_INTERRUPT
      static avr32::uc3::intc::InterruptHandler_t registeredCallbacks[];
#endif // AVR32_UC3_GPIO_PIN_INTERRUPT
    };

    inline void
    Gpio::setModeGpio(void)
    {
      portRegisters.gpers = m_mask;
    }

    inline void
    Gpio::setModePeripheral(void)
    {
      portRegisters.gperc = m_mask;
    }

    inline void
    Gpio::toggleMode(void)
    {
      portRegisters.gpert = m_mask;
    }

    inline bool
    Gpio::isModeGpio(void)
    {
      return (bool)(portRegisters.gper & m_mask);
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
      return (bool)(portRegisters.pvr & m_mask) != 0;
    }

    inline void
    Gpio::setDirectionOutput(void)
    {
      portRegisters.oders = m_mask;
    }

    inline void
    Gpio::setDirectionInput(void)
    {
      portRegisters.oderc = m_mask;
    }

    inline void
    Gpio::toggleDirection(void)
    {
      portRegisters.odert = m_mask;
    }

    inline bool
    Gpio::isDirectionOutputEnabled(void)
    {
      return (bool)(portRegisters.oder & m_mask);
    }

    inline void
    Gpio::enablePullup(void)
    {
      portRegisters.puers = m_mask;
    }

    inline void
    Gpio::disablePullup(void)
    {
      portRegisters.puerc = m_mask;
    }

    inline void
    Gpio::togglePullup(void)
    {
      portRegisters.puert = m_mask;
    }

    inline bool
    Gpio::isPullupEnabled(void)
    {
      return (bool)(portRegisters.puer & m_mask);
    }

    inline void
    Gpio::enableInterrupt(void)
    {
      portRegisters.iers = m_mask;
    }
    inline void
    Gpio::disableInterrupt(void)
    {
      portRegisters.ierc = m_mask;
    }
    inline void
    Gpio::toggleInterrupt(void)
    {
      portRegisters.iert = m_mask;
    }
    inline bool
    Gpio::isInterruptEnabled(void)
    {
      return (bool)(portRegisters.ier & m_mask);
    }

    inline bool
    Gpio::isInterruptRequested(void)
    {
      return (bool)(portRegisters.ifr & m_mask);
    }
    inline void
    Gpio::clearInterruptRequested(void)
    {
      portRegisters.ifrc = m_mask;
    }

    inline void
    Gpio::enableGlitchFilter(void)
    {
      portRegisters.gfers = m_mask;
    }
    inline void
    Gpio::disableGlitchFilter(void)
    {
      portRegisters.gferc = m_mask;
    }
    inline void
    Gpio::toggleGlitchFilter(void)
    {
      portRegisters.gfert = m_mask;
    }
    inline bool
    Gpio::isGlitchFilterEnabled(void)
    {
      return (bool)(portRegisters.gfer & m_mask);
    }

    inline void
    configLocalBus(void)
    {
      Set_system_register(AVR32_CPUCR,
          Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
    }
  }
}

#endif /* AVR32_UC3_GPIO_H_ */
