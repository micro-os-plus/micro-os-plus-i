/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_PDCA)

#include "hal/arch/avr32/uc3/devices/onchip/include/Pdca.h"

namespace avr32
{
  namespace uc3
  {
    // ----- Pdca base --------------------------------------------------------

    Pdca::Pdca(ChannelId_t id) :
          m_channelRegisters(
              *reinterpret_cast<ChannelRegisters*> (0xFFFF0000 + ((int) id)
                  * 0x040))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Pdca", this);

      // The trick here was to map the object reference over the actual
      // processor memory mapped register.

      //OSDeviceDebug::putPtr(reinterpret_cast<void*> (&m_channelRegisters));
      //OSDeviceDebug::putNewLine();
    }

    void
    Pdca::setRegionsArray(Region_t* pRegionsArray, uint_t regionsArraySize,
        bool isCircular)
    {
      m_pRegionsArray = pRegionsArray;
      m_regionsArraySize = regionsArraySize;
      m_isCircular = isCircular;
    }

    // ----- PdcaTransmit -----------------------------------------------------

    PdcaTransmit::PdcaTransmit(ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaTransmit", this);
    }

    // ----- PdcaReceive ------------------------------------------------------

    PdcaReceive::PdcaReceive(ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaReceive", this);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_PDCA) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
