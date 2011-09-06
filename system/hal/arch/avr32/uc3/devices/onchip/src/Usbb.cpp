/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_USBB)

#include "hal/arch/avr32/uc3/devices/onchip/include/Usbb.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Usbb::Usbb() :
          moduleRegisters(
              *reinterpret_cast<usbb::ModuleRegisters*> (usbb::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Usbb", this);
    }

    Usbb::~Usbb()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Usbb", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    Usbb::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Usbb::PowerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction(
              m_pGpioConfigurationArray);
        }
    }

    void
    Usbb::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Usbb::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
        }
    }

    void
    Usbb::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      OSCriticalSection::enter();
        {
          Intc::registerInterruptHandler(handler,
              Intc::computeInterruptIndex(intc::Group::USB, 0),
              intc::GroupPriority::USB);
        }
      OSCriticalSection::exit();
    }

    // May be called several times, if conditions change
    OSReturn_t
    Usbb::initialise(void)
    {
      return OSReturn::OS_OK;
    }

    OSReturn_t
    Usbb::configureClockFrequencyHz(ClockFrequencyHz_t speed)
    {
      speed = speed;
      return OSReturn::OS_OK;
    }

    void
    Usbb::interruptServiceRoutine(void)
    {
      ;
    }

  // --------------------------------------------------------------------------

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_USBB) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
