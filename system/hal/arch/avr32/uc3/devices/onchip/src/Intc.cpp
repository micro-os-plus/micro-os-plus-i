/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_INTC)

#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"

namespace avr32
{
  namespace uc3
  {
    Intc::Intc()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Intc", this);
    }

    void
    Intc::registerInterruptHandler(intc::InterruptHandler_t handler,
        intc::InterruptIndex_t index, intc::Priority_t priority)
    {
      OSDeviceDebug::putString("Intc::registerInterruptHandler(");
      OSDeviceDebug::putPtr((void*)handler);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(index/32);
      OSDeviceDebug::putString("*32+");
      OSDeviceDebug::putDec(index%32);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(priority);
      OSDeviceDebug::putChar(')');
      OSDeviceDebug::putNewLine();

      // TODO: implement locally
      // Until then, call the framework
      INTC_register_interrupt(handler, index, priority);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_INTC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
