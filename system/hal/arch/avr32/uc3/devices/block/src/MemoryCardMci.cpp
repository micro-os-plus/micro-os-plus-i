/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI)

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardMci::MemoryCardMci()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardMci", this);
    }

    MemoryCardMci::~MemoryCardMci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci", this);
    }
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
