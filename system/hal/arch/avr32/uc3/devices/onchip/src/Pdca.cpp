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
      m_event = (OSEvent_t) this;
      m_eventRet = OSEventWaitReturn::OS_NONE;
      m_candidateNotif = -1; // no candidate to be notified, yet
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
      // set zero count for the current transfer
      registers.writeTransferCount(0);
      // set zero count for the next transfer
      registers.writeTransferCountReload(0);

      registers.writeControl(AVR32_PDCA_CR_ECLR_MASK); // clear errors

      // set the associated peripheral
      registers.writePeripheralSelect(m_peripheralId);

      if (m_regionsArraySize == 0)
        {
          // no regions configured, so nothing to be done
          #if OS_DEBUG_PDCA
                OSDeviceDebug::putString("Pdca::prepareTransfer no region");
                OSDeviceDebug::putNewLine();
          #endif
          return OSReturn::OS_BAD_COMMAND;
        }
      // set first region to be used in transfer
      registers.writeMemoryAddress(m_pRegionsArray[0].address);
      registers.writeTransferCount(m_pRegionsArray[0].size);
      //while (registers.readStatus())
      m_currentRegionIndex = 0;
      m_candidateNotif = 0;

      registers.writeInterruptEnable(
          AVR32_PDCA_IER_TRC_MASK | AVR32_PDCA_IER_TERR_MASK);
      // enable channel and transfer

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

    // configures the reload mechanism with the next region
    OSReturn_t
    Pdca::setupReloadMechanism()
    {
      int nextRegion;
      nextRegion = getNextRegionIndex(m_currentRegionIndex);
      if (nextRegion < 0)
        {
          // no region to be set next
          m_reloadedRegionIndex = -1;
#if OS_DEBUG_PDCA
            OSDeviceDebug::putString("Pdca::setupReloadMechanism no next region");
            OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_ITEM_NOT_FOUND;
        }
      registers.writeMemoryAddressReload(m_pRegionsArray[nextRegion].address);
      registers.writeTransferCountReload(m_pRegionsArray[nextRegion].size);
      m_reloadedRegionIndex = nextRegion;
      registers.writeInterruptEnable(AVR32_PDCA_IER_RCZ_MASK);
      return OSReturn::OS_OK;
    }

    // returns the index of the next region which must be used
    int
    Pdca::getNextRegionIndex(int actualRegion)//implicit should be m_currentRegionIndex
    {
      int nextRegionIndex = 0;

      if (m_isCircular)
        {
          // get consecutive index, modulo total regions number
          nextRegionIndex = (actualRegion + 1) % m_regionsArraySize;
        }
      else
        {
          // get consecutive index, if there are more regions available
          if (m_regionsArraySize > (uint_t)(actualRegion + 1))
            {
              nextRegionIndex = actualRegion + 1;
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
      m_status = avr32::uc3::pdca::STATUS_BUSY;
      // enable channel and transfer
      registers.writeControl(AVR32_PDCA_CR_TEN_MASK);
    }

    void
    Pdca::abortTransfer(void)
    {
      // disable all interrupt sources
      registers.writeInterruptDisable(AVR32_PDCA_IDR_RCZ_MASK |
          AVR32_PDCA_IDR_TERR_MASK | AVR32_PDCA_IDR_TRC_MASK);

      // disable PDMA channel
      registers.writeControl(AVR32_PDCA_CR_TDIS_MASK);

      // update remaining numbers of words
      m_wordsRemaining = registers.readTransferCounter() +
          registers.readTransferCounterReload();

      // reset counters
      registers.writeTransferCount(0);

      // reset reload counters
      registers.writeTransferCountReload(0);

      // clear error
      registers.writeControl(AVR32_PDCA_CR_ECLR_MASK);
    }

    void
    Pdca::registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler)
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

    OSReturn_t
    PdcaTransmit::waitWriteRegions(bool doNotBlock)
    {
      // if status is not busy return immediately
      if (m_status != avr32::uc3::pdca::STATUS_BUSY)
        {
          return OSReturn::OS_OK;
        }

      // status busy and doNotBlock return WOULD_BLOCK
      if (doNotBlock == true)
        return OSReturn::OS_WOULD_BLOCK;

      // wait
      OSScheduler::eventWait(m_event);
      return OSReturn::OS_OK;
    }

    void
    PdcaTransmit::stopTransfer(void)
    {
      abortTransfer();
      m_status = avr32::uc3::pdca::STATUS_STOPPED;
      // notify
      OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
    }

    void
    PdcaTransmit::interruptServiceRoutine(void)
    {
      uint32_t interruptFlag;

      // find the interrupt source
      interruptFlag = registers.readInterruptMask() &
          registers.readInterruptStatus();

      if ((interruptFlag & AVR32_PDCA_IER_TERR_MASK) != 0) // transfer error
        {

          // TODO: log registers MAR, TCR, MARR and TCRR
          // the only source of error is an invalid address in MAR or MARR

          abortTransfer();
          m_status = avr32::uc3::pdca::STATUS_ERROR;
        }
      if ((interruptFlag & AVR32_PDCA_IER_RCZ_MASK) != 0) // reload counter zero
        {
          m_currentRegionIndex = m_reloadedRegionIndex;

          OSReturn_t ret = setupReloadMechanism();
          if (ret != OSReturn::OS_OK)
            {
              // means no next region so this is last transfer
              // disable RCZ interrupt
              registers.writeInterruptDisable(AVR32_PDCA_IDR_RCZ_MASK);
            }
        }
      if ((interruptFlag & AVR32_PDCA_IER_TRC_MASK) != 0) // transfer complete
      {
          // disable PDMA channel
          registers.writeControl(AVR32_PDCA_CR_TDIS_MASK);

          // disable all interrupt sources
          registers.writeInterruptDisable(AVR32_PDCA_IDR_RCZ_MASK |
              AVR32_PDCA_IDR_TERR_MASK | AVR32_PDCA_IDR_TRC_MASK);

          m_status = avr32::uc3::pdca::STATUS_OK;
          // notify
          OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
      }
    }

    // ----- PdcaReceive ------------------------------------------------------

    PdcaReceive::PdcaReceive(pdca::ChannelId_t id) :
      Pdca(id)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::PdcaReceive", this);
    }

    OSReturn_t
    PdcaReceive::readRegion(pdca::RegionAddress_t& region, bool doNotBlock)
    {
      int nextRegion;

      // if nothing to be notified
      if (m_candidateNotif < 0)
        {
#if OS_DEBUG_PDCA
          OSDeviceDebug::putString("PdcaReceive::readRegion no candid. region");
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_ITEM_NOT_FOUND;
        }

      // check if the candidate(to be notified) is already transfered
      if (m_pRegionsArray[m_candidateNotif].status ==
          avr32::uc3::pdca::IS_TRANFERRED_MASK)
        {
          region = &(m_pRegionsArray[m_candidateNotif]);
          m_pRegionsArray[m_candidateNotif].status =
                    avr32::uc3::pdca::IS_SIGNALLED_MASK;
          nextRegion = getNextRegionIndex(m_candidateNotif);
          if (nextRegion != -1)
            m_candidateNotif = nextRegion;
          else // no next region
            m_candidateNotif = -1;
          return OSReturn::OS_OK;
        }

      // the candidate is not ready to be notified
      if (doNotBlock == true)
        return OSReturn::OS_WOULD_BLOCK;

      // now wait for the candidate to be ready
      //OSEventWaitReturn_t ret = OSScheduler::eventWait(m_event);
      OSScheduler::eventWait(m_event);

      // should check if m_candidateNotif is transferred
      if (m_pRegionsArray[m_candidateNotif].status !=
                avr32::uc3::pdca::IS_TRANFERRED_MASK)
        {
#if OS_DEBUG_PDCA
          OSDeviceDebug::putString("PdcaReceive::readRegion candid. reg NOT transf.");
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_ITEM_NOT_FOUND;
        }

      region = &(m_pRegionsArray[m_candidateNotif]);
      m_pRegionsArray[m_candidateNotif].status =
                avr32::uc3::pdca::IS_SIGNALLED_MASK;

      // set next candidate to be notified
      nextRegion = getNextRegionIndex(m_candidateNotif);
      if (nextRegion != -1)
        m_candidateNotif = nextRegion;
      else // no next region
        m_candidateNotif = -1;
      return OSReturn::OS_OK;

    }

    void
    PdcaReceive::stopTransfer(void)
    {
      abortTransfer();
      m_status = avr32::uc3::pdca::STATUS_STOPPED;
      m_pRegionsArray[m_candidateNotif].status =
                      avr32::uc3::pdca::IS_TRANFERRED_MASK;
      // notify
      OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
    }

    void
    PdcaReceive::interruptServiceRoutine(void)
    {
      uint32_t interruptFlag;

      // find the interrupt source
      interruptFlag = registers.readInterruptMask() &
          registers.readInterruptStatus();

      if ((interruptFlag & AVR32_PDCA_IER_TERR_MASK) != 0) // transfer error
        {

          // TODO: log registers MAR, TCR, MARR and TCRR
          // the only source of error is an invalid address in MAR or MARR

          abortTransfer();
          // set status to finished
          m_pRegionsArray[m_candidateNotif].status =
                              avr32::uc3::pdca::IS_TRANFERRED_MASK;
          m_status = avr32::uc3::pdca::STATUS_ERROR;
          OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
        }
      if ((interruptFlag & AVR32_PDCA_IER_RCZ_MASK) != 0) // reload counter zero
        {
          // set status to finished
          m_pRegionsArray[m_currentRegionIndex].status =
                              avr32::uc3::pdca::IS_TRANFERRED_MASK;
          // notify
          OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);

          m_currentRegionIndex = m_reloadedRegionIndex;
          OSReturn_t ret = setupReloadMechanism();
          if (ret != OSReturn::OS_OK)
            {
              // means no next region so this is last transfer
              // disable RCZ interrupt
              registers.writeInterruptDisable(AVR32_PDCA_IDR_RCZ_MASK);
            }
        }
      if ((interruptFlag & AVR32_PDCA_IER_TRC_MASK) != 0) // transfer complete
      {
          // disable PDMA channel
          registers.writeControl(AVR32_PDCA_CR_TDIS_MASK);

          // disable all interrupt sources
          registers.writeInterruptDisable(AVR32_PDCA_IDR_RCZ_MASK |
              AVR32_PDCA_IDR_TERR_MASK | AVR32_PDCA_IDR_TRC_MASK);

          // set status to finished
          m_pRegionsArray[m_currentRegionIndex].status =
                              avr32::uc3::pdca::IS_TRANFERRED_MASK;

          m_status = avr32::uc3::pdca::STATUS_OK;
          // notify
          OSScheduler::eventNotify(m_event, (OSEventWaitReturn_t) m_event);
      }
    }




  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_PDCA) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
