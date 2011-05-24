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
          registers(
              *reinterpret_cast<ChannelRegisters*> (0xFFFF0000 + ((int) id)
                  * 0x040))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Pdca", this);

      // The trick here was to map the object reference over the actual
      // processor memory mapped register.

#if true
      OSDeviceDebug::putPtr(reinterpret_cast<void*> (&registers));
      OSDeviceDebug::putNewLine();
#endif
    }

    void
    Pdca::setRegionsArray(Region_t* pRegionsArray, uint_t regionsArraySize,
        bool isCircular)
    {
      m_pRegionsArray = pRegionsArray;
      m_regionsArraySize = regionsArraySize;
      m_isCircular = isCircular;
    }

    OSReturn_t
    Pdca::prepareTransfer(void)
    {
#if true
      // First set the associated peripheral
      registers.writePeripheralSelect(m_peripheralId);

      registers.writeMemoryAddress(m_pRegionsArray[0].address);
      registers.writeTransferCount(m_pRegionsArray[0].size);
#else
      // test to check compiled code
      // First set the associated peripheral
      registers.writePeripheralSelect(spi1rx);

      registers.writeMemoryAddress((void*)32);
      registers.writeTransferCount(64);
#endif
      // TODO: add more code

      return OSReturn::OS_OK;
    }

    void
    Pdca::registerInterruptHandler(void* handler __attribute__((unused)))
    {
      // TODO register the given interrupt in the m_peripheralId position
    }

    // ----- PdcaTransmit -----------------------------------------------------

    PdcaTransmit::PdcaTransmit(ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaTransmit", this);
    }

    // TODO: add the other functions

    // ----- PdcaReceive ------------------------------------------------------

    PdcaReceive::PdcaReceive(ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaReceive", this);
    }

  // TODO: add the other functions

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_PDCA) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
