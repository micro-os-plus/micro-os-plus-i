/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_PM)

#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{

  namespace uc3
  {

    Pm::Pm() :
          moduleRegisters(
              *reinterpret_cast<pm::ModuleRegisters*> (pm::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Pm", this);
    }

    Pm::~Pm()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Pm", this);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_PM) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
