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
    }

    Usart::~Usart()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Template", this);
    }

    // ----- Public Methods ---------------------------------------------------

    // --------------------------------------------------------------------------

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_OS_INCLUDE_AVR32_UC3_template) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
