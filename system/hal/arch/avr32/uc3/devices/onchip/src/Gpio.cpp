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
#if AVR32_UC3_GPIO_PIN_INTERRUPT
    volatile gpio::PortRegisters* Gpio::portRegistersArray[4];

    avr32::uc3::intc::InterruptHandler_t
    Gpio::registeredCallbacks[AVR32_GPIO_NUMBER_OF_PINS];
#endif

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
#if AVR32_UC3_GPIO_PIN_INTERRUPT
      Gpio::portRegistersArray[0] = (gpio::PortRegisters*)(gpio::PortRegisters::MEMORY_ADDRESS);
      Gpio::portRegistersArray[1] = (gpio::PortRegisters*)(gpio::PortRegisters::MEMORY_ADDRESS +
          gpio::PortRegisters::MEMORY_OFFSET);
      Gpio::portRegistersArray[2] = (gpio::PortRegisters*)(gpio::PortRegisters::MEMORY_ADDRESS +
          2 * gpio::PortRegisters::MEMORY_OFFSET);
      Gpio::portRegistersArray[3] = (gpio::PortRegisters*)(gpio::PortRegisters::MEMORY_ADDRESS +
          3 * gpio::PortRegisters::MEMORY_OFFSET);

      // register an interrupt handler for each line
      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler0,
          gpio::INTERRUPT_BASE + 0,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler1,
          gpio::INTERRUPT_BASE + 1,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler2,
          gpio::INTERRUPT_BASE + 2,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler3,
          gpio::INTERRUPT_BASE + 3,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler4,
          gpio::INTERRUPT_BASE + 4,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler5,
          gpio::INTERRUPT_BASE + 5,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler6,
          gpio::INTERRUPT_BASE + 6,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler7,
          gpio::INTERRUPT_BASE + 7,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler8,
          gpio::INTERRUPT_BASE + 8,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler9,
          gpio::INTERRUPT_BASE + 9,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler10,
          gpio::INTERRUPT_BASE + 10,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler11,
          gpio::INTERRUPT_BASE + 11,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler12,
          gpio::INTERRUPT_BASE + 12,
          avr32::uc3::intc::GroupPriorities::GROUP_02);

      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) lineHandler13,
          gpio::INTERRUPT_BASE + 13,
          avr32::uc3::intc::GroupPriorities::GROUP_02);
#endif // AVR32_UC3_GPIO_PIN_INTERRUPT
    }

#if AVR32_UC3_GPIO_PIN_INTERRUPT
    void
    Gpio::lineHandler0(void)
    {
      lineHandler(0);
    }

    void
    Gpio::lineHandler1(void)
    {
      lineHandler(1);
    }

    void
    Gpio::lineHandler2(void)
    {
      lineHandler(2);
    }

    void
    Gpio::lineHandler3(void)
    {
      lineHandler(3);
    }

    void
    Gpio::lineHandler4(void)
    {
      lineHandler(4);
    }

    void
    Gpio::lineHandler5(void)
    {
      lineHandler(5);
    }

    void
    Gpio::lineHandler6(void)
    {
      lineHandler(6);
    }

    void
    Gpio::lineHandler7(void)
    {
      lineHandler(7);
    }

    void
    Gpio::lineHandler8(void)
    {
      lineHandler(8);
    }

    void
    Gpio::lineHandler9(void)
    {
      lineHandler(9);
    }

    void
    Gpio::lineHandler10(void)
    {
      lineHandler(10);
    }

    void
    Gpio::lineHandler11(void)
    {
      lineHandler(11);
    }

    void
    Gpio::lineHandler12(void)
    {
      lineHandler(12);
    }

    void
    Gpio::lineHandler13(void)
    {
      lineHandler(13);
    }

    void
    Gpio::lineHandler(uint8_t line)
    {
      uint32_t pin, pinMask, pinBase, i, port, intrPin;

      port = line/4;
      pinBase = line * AVR32_GPIO_IRQS_PER_GROUP;

      //gpio::PortRegisters& gpioPort = Gpio::portRegistersArray[port];

      for(i = 0; i < AVR32_GPIO_IRQS_PER_GROUP; i++)
      {
          // check if an interrupt was generated for this pin
          pin = pinBase + i;
          pinMask = 1 << (pin & 0x1F);

          intrPin = Gpio::portRegistersArray[port]->ifr & Gpio::portRegistersArray[port]->ier & pinMask;

          if(intrPin && registeredCallbacks[pin])
          {
              registeredCallbacks[pin]( );

              // acknowledge the interrupt
              Gpio::portRegistersArray[port]->ifrc = pinMask;
          }
      }
    }
#endif // AVR32_UC3_GPIO_PIN_INTERRUPT

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

    // Warning: if AVR32_UC3_GPIO_PIN_INTERRUPT is not activated, than
    // it is not possible to set handlers of each individual interrupt,
    // but only for groups of 8 (there are 14 groups (0-13) of 8 interrupts)
    void
    Gpio::registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler)
    {
#if AVR32_UC3_GPIO_PIN_INTERRUPT
      registeredCallbacks[m_pin] = handler;
#else
      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) handler,
          gpio::INTERRUPT_BASE + (m_pin / 8),
          avr32::uc3::intc::GroupPriorities::GROUP_02);
#endif // AVR32_UC3_GPIO_PIN_INTERRUPT
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
