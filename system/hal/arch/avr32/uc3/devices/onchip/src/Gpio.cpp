/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_GPIO)

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    Gpio::Gpio(gpio::PinNumber_t pin) :
          portRegisters(
              *reinterpret_cast<gpio::PortRegisters*> (gpio::PortRegisters::MEMORY_ADDRESS
                  + ((pin / 32) * gpio::PortRegisters::MEMORY_OFFSET)))

    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Gpio", pin, this);

      m_mask = 1 << (pin & 0x1F);
      m_pin = pin;
    }

    void
    Gpio::init(void)
    {
      ;
    }

    void
    Gpio::configPeripheralFunction(gpio::PeripheralFunction_t func)
    {
      if (func & 0x1)
        portRegisters.pmr0s = m_mask;
      else
        portRegisters.pmr0c = m_mask;

      if (func & 0x2)
        portRegisters.pmr1s = m_mask;
      else
        portRegisters.pmr1c = m_mask;
    }

    void
    Gpio::configInterruptMode(gpio::InterruptMode_t mode)
    {
      // Configure the edge detector.
      if (mode & 0x1)
        portRegisters.imr0s = m_mask;
      else
        portRegisters.imr0c = m_mask;

      if (mode & 0x2)
        portRegisters.imr1s = m_mask;
      else
        portRegisters.imr1c = m_mask;
    }

    // Warning: due to the AVR32 architecture,
    // it is not possible to set handlers of each individual interrupt,
    // but only for groups of 8 (there are 14 groups (0-13) of 8 interrupts)
    void
    Gpio::registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler)
    {
      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) handler,
          gpio::INTERRUPT_BASE + (m_pin / 8),
          avr32::uc3::intc::GroupPriorities::GROUP_02);
    }

    // Static method
    void
    Gpio::configPeripheralModeAndFunction(gpio::PinNumber_t pin,
        gpio::PeripheralFunction_t func)
    {
      // Construct a local object here and use it
      // It will be deleted when exiting this function
      Gpio gpio(pin);

      gpio.setModePeripheral();
      gpio.configPeripheralFunction(func);
    }

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_GPIO) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
