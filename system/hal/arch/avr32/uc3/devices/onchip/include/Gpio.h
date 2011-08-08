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

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    class Gpio
    {
    public:
      Gpio(gpio::PinNumber_t pin);
      ~Gpio();

      void
      initialise(gpio::PinNumber_t pin);

      void
      configureModeGpio(void);
      void
      configureModePeripheral(void);
      void
      toggleMode(void);
      bool
      isModeGpio(void);

      void
      configurePeripheralFunction(gpio::PeripheralFunction_t f);

      void
      setPinHigh(void);
      void
      setPinLow(void);
      void
      togglePin(void);

      bool
      isPinHigh(void);

      void
      configureDirectionOutput(void);
      void
      configureDirectionInput(void);
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
      clearInterruptRequest(void);

      void
      configureInterruptMode(gpio::InterruptMode_t mode);

      void
      registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler);

      // ----- Static methods -------------------------------------------------

      // Static method to be used at system initialisation to enable local bus
      // (mandatory, otherwise all local bus peripheral calls will fail).

      static void
      configureLocalBus(void);

      // Static method to be used without a dedicated object,
      // for example when setting pins for another terminal

      static void
      configPeripheralModeAndFunction(gpio::PinNumber_t pin,
          gpio::PeripheralFunction_t func);

      static void
      configGpioModeInput(gpio::PinNumber_t pin);

      static void
      configPeripheralModeAndFunction(
          gpio::PinPeripheralFunction_t* pPinFunctionArray);

      static void
      configGpioModeInput(
          gpio::PinPeripheralFunction_t* pPinFunctionArray);

    public:
      volatile gpio::PortRegisters& portRegisters;

    private:

      // private data
      uint32_t m_mask;
      gpio::PinNumber_t m_pin;

    };

    inline void
    Gpio::configureModeGpio(void)
    {
      portRegisters.gpers = m_mask;
    }

    inline void
    Gpio::configureModePeripheral(void)
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
      return ((portRegisters.gper & m_mask) != 0);
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
      return ((portRegisters.pvr & m_mask) != 0);
    }

    inline void
    Gpio::configureDirectionOutput(void)
    {
      portRegisters.oders = m_mask;
    }

    inline void
    Gpio::configureDirectionInput(void)
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
      return ((portRegisters.oder & m_mask) != 0);
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
      return ((portRegisters.puer & m_mask) != 0);
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
      return ((portRegisters.ier & m_mask) != 0);
    }

    inline bool
    Gpio::isInterruptRequested(void)
    {
      return ((portRegisters.ifr & m_mask) != 0);
    }

    inline void
    Gpio::clearInterruptRequest(void)
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
      return ((portRegisters.gfer & m_mask) != 0);
    }

    inline void
    configLocalBus(void)
    {
      // TODO: rewrite this
      Set_system_register(AVR32_CPUCR,
          Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
    }
  }
}

#endif /* AVR32_UC3_GPIO_H_ */
