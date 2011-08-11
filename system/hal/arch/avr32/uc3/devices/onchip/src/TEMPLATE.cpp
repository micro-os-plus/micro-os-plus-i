/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_template)

#include "hal/arch/avr32/uc3/devices/onchip/include/Usart.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Template::Template(template::ModuleId_t module) :
          moduleRegisters(
              *reinterpret_cast<template::ModuleRegisters*> (template::ModuleRegisters::MEMORY_ADDRESS
                  + ((module % 4) * usart::ModuleRegisters::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Template", module, this);

      setModule(module);
    }

    Template::~Template()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Template", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    Template::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Template::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction
              ( m_pGpioConfigurationArray);
        }
    }

    void
    Template::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Template::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput( m_pGpioConfigurationArray);
        }
    }

    void
    Template::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      OSCriticalSection::enter();
        {
          Intc::registerInterruptHandler(
              handler,
              Intc::computeInterruptIndex(
                  m_module == twim::ModuleId::TWIM0 ? intc::Group::TWIM0
                      : intc::Group::TWIM1, 0),
              m_module == twim::ModuleId::TWIM0 ? intc::GroupPriority::TWIM0
                  : intc::GroupPriority::TWIM1);
        }
      OSCriticalSection::exit();
    }

    // May be called several times, if conditions change
    OSReturn_t
    Template::initialise(void)
    {
      return OSReturn::OS_OK;
    }

    OSReturn_t
    Template:: configureClockFrequencyHz(ClockFrequencyHz_t speed)
    {
          return OSReturn::OS_OK;
        }

    // --------------------------------------------------------------------------

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_template) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
