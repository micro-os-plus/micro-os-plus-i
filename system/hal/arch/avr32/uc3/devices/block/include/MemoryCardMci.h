/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MEMORYCARDMCI_H_
#define AVR32_UC3_MEMORYCARDMCI_H_

#include "portable/kernel/include/OS.h"
#include "portable/devices/block/include/OSDeviceAddressable.h"

namespace avr32
{
  namespace uc3
  {
    class MemoryCardMci : public OSDeviceAddressable
    {
    public:
      MemoryCardMci();
      virtual
      ~MemoryCardMci();
    };
  }
}

#endif /* AVR32_UC3_MEMORYCARDMCI_H_ */
