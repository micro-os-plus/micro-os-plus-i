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
      m_regionsArraySize = 0;
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
      // make sure no transfer is ongoing
      registers.writeControl(AVR32_PDCA_CR_TDIS_MASK); // disable channel
      registers.writeControl(AVR32_PDCA_CR_ECLR_MASK); // clear errors
      // set zero count for the current transfer
      registers.writeTransferCount(0);
      // set zero count for the next transfer
      registers.writeTransferCountReload(0);

      // set the associated peripheral
      registers.writePeripheralSelect(m_peripheralId);

      if (m_regionsArraySize == 0)
        // no regions configured, so nothing to be done
        return -1;

      // set first region to be used in transfer
      registers.writeMemoryAddress(m_pRegionsArray[0].address);
      registers.writeTransferCount(m_pRegionsArray[0].size);
      currentRegionIndex = 0;

      registers.writeInterruptEnable(AVR32_PDCA_IER_TRC_MASK |
          AVR32_PDCA_IER_TERR_MASK);

      setupReloadMechanism();

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
    Pdca::setupReloadMechanism()
    {
      int nextRegion;
      nextRegion = getNextRegionIndex();
      if (nextRegion < 0)
        {
          // no region to be set next
          reloadedRegionIndex = -1;
          return;
        }
      registers.writeMemoryAddressReload(m_pRegionsArray[nextRegion].address);
      registers.writeTransferCountReload(m_pRegionsArray[nextRegion].size);
      reloadedRegionIndex = nextRegion;
      registers.writeInterruptEnable(AVR32_PDCA_IER_RCZ_MASK);
    }

    // returns the index of the next region which must be used
    int
    Pdca::getNextRegionIndex()
    {
      int nextRegionIndex = 0;

      if (m_isCircular)
        {
          // get consecutive index, modulo total regions number
          nextRegionIndex = (currentRegionIndex + 1) % m_regionsArraySize;
        }
      else
        {
          // get consecutive index, if there are more regions available
          if (m_regionsArraySize > currentRegionIndex )
            nextRegionIndex = currentRegionIndex + 1;
          else
            // no further regions available
            nextRegionIndex = -1;
        }
      return nextRegionIndex;
    }

    void
    Pdca::startTransfer(void)
    {
      // enable channel and transfer
      registers.writeControl(AVR32_PDCA_CR_TDIS_MASK);
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
