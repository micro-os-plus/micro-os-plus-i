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

      m_mask = pin % 32;
    }

    void
    Gpio::configPeripheralFunction(gpio::PeripheralFunction_t f)
    {
      if (f & 0x1)
        portRegisters.pmr0s = m_mask;
      else
        portRegisters.pmr0c = m_mask;

      if (f & 0x2)
        portRegisters.pmr1s = m_mask;
      else
        portRegisters.pmr1c = m_mask;
    }

    void
    Gpio::configPeripheralFunction(gpio::PinNumber_t pin,
        gpio::PeripheralFunction_t f)
    {
      // Construct a local object here and use it
      // It will be deleted when exiting this function
      Gpio gpio(pin);

      gpio.configModePeripheral();
      gpio.configPeripheralFunction(f);
    }

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_GPIO) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
