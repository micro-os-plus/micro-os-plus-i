/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_PDCA)

#include "hal/arch/avr32/uc3/devices/onchip/include/Pdca.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"

namespace avr32
{
  namespace uc3
  {
    // ----- Pdca base --------------------------------------------------------

    Pdca::Pdca(pdca::ChannelId_t id) :
          registers(
              *reinterpret_cast<pdca::ChannelRegisters*> (pdca::ChannelRegisters::MEMORY_ADDRESS
                  + ((int) id) * pdca::ChannelRegisters::MEMORY_OFFSET))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Pdca", id, this);

      // The trick here was to map the object reference over the actual
      // processor memory mapped register.

      m_channelId = id;

#if false
      OSDeviceDebug::putPtr(reinterpret_cast<void*> (&registers));
      OSDeviceDebug::putNewLine();
#endif
      m_regionsArraySize = 0;
    }

    void
    Pdca::setRegionsArray(pdca::Region_t* pRegionsArray,
        uint_t regionsArraySize, bool isCircular)
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
      m_currentRegionIndex = 0;

      registers.writeInterruptEnable(
          AVR32_PDCA_IER_TRC_MASK | AVR32_PDCA_IER_TERR_MASK);

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
          m_reloadedRegionIndex = -1;
          return;
        }
      registers.writeMemoryAddressReload(m_pRegionsArray[nextRegion].address);
      registers.writeTransferCountReload(m_pRegionsArray[nextRegion].size);
      m_reloadedRegionIndex = nextRegion;
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
          nextRegionIndex = (m_currentRegionIndex + 1) % m_regionsArraySize;
        }
      else
        {
          // get consecutive index, if there are more regions available
          if (m_regionsArraySize > m_currentRegionIndex)
            {
              nextRegionIndex = m_currentRegionIndex + 1;
            }
          else
            {
              // no further regions available
              nextRegionIndex = -1;
            }
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
    Pdca::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      avr32::uc3::Intc::registerInterruptHandler(
          (intc::InterruptHandler_t) handler,
          pdca::INTERRUPT_BASE + m_channelId,
          avr32::uc3::intc::GroupPriorities::GROUP_03);

    }

    // ----- PdcaTransmit -----------------------------------------------------

    PdcaTransmit::PdcaTransmit(pdca::ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaTransmit", this);
    }

    // TODO: add the other functions

    // ----- PdcaReceive ------------------------------------------------------

    PdcaReceive::PdcaReceive(pdca::ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaReceive", this);
    }

    OSReturn_t
    PdcaReceive::readRegion(pdca::RegionAddress_t& region, bool doNotBlock)
    {

      return 1;
    }

    void
    PdcaReceive::stopTransfer(void)
    {
      // disable all interrupt sources
      // disable PDMA channel
      // update remaining numbers of words
      // reset counters
      // reset reload counters
      // clear error
    }

    void
    PdcaReceive::interruptServiceRoutine(void)
    {
      uint32_t interruptFlag;

      // find the interrupt source
      interruptFlag = registers.readInterruptMask() &
          registers.readInterruptStatus();

      if (interruptFlag & AVR32_PDCA_IER_TERR_MASK) // transfer error
        {
          // disable all interrupt sources
          // TODO: log registers MAR, TCR, MARR and TCRR
          //update remaining numbers of words
          // disable PDMA channel
          // reset counters
          // reset reload counters
          // clear error
        }
      if (interruptFlag & AVR32_PDCA_IER_RCZ_MASK) // reload counter zero
        {
          // if last transfer
            // disable RCZ interrupt
          // load next transfer
        }
      if (interruptFlag & AVR32_PDCA_IER_TRC_MASK) // transfer complete
      {
          // disable PDMA channel
          // disable all interrupt sources
      }
    }
  // TODO: add the other functions

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_PDCA) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
