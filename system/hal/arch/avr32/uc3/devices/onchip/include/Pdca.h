/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_PDCA_H_
#define AVR32_UC3_PDCA_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"

namespace avr32
{
  namespace uc3
  {
    namespace pdca
    {
      // ----- Type definitions -----------------------------------------------

      typedef enum ChannelId_e
      {
        // Define one of the eight channels
        CHANNEL_0 = 0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
        CHANNEL_6,
        CHANNEL_7 = 7
      } ChannelId_t;

      typedef void* RegionAddress_t;
      typedef uint16_t RegionSize_t;
      typedef struct Region_s
      {
        // A memory region is defined by an address and a size
        RegionAddress_t address;
        RegionSize_t size;
      } Region_t;

      typedef enum PeripheralId_e
      {
        // Define the peripheral which will do the actual transfer
        // Should be limited to a byte, currently no more than 23
        ADC_RX = 0,
        SSC_RX = 1,
        USART0_RX = 2,
        TWIM0_RX = 6,
        SPI0_RX = 10,
        SPI1_RX = 11,
        USART0_TX = 13,
        TWIM0_TX = 17,
        SPI0_TX = 21,
        SPI1_TX = 22,
        ABDAC_TX = 23
      } PeripheralId_t;

      typedef enum TransferSize_e
      {
        // Define the transfer size
        SIZE_1_BYTE = 0,
        SIZE_2_BYTES = 1,
        SIZE_4_BYTES = 2
      } TransferSize_t;

      const static uint_t INTERRUPT_BASE = 96;

      // ----- Channel Memory Mapped Registers --------------------------------

      class ChannelRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS = 0xFFFF0000;
        const static uint32_t MEMORY_OFFSET = 0x040;

        // ----- Memory Map ---------------------------------------------------
        regReadWrite_t mar;
        regReadWrite_t psr;
        regReadWrite_t tcr;
        regReadWrite_t marr;
        regReadWrite_t tcrr;
        regWriteOnly_t cr;
        regReadWrite_t mr;
        regReadOnly_t sr;
        regWriteOnly_t ier;
        regWriteOnly_t idr;
        regReadOnly_t imr;
        regReadOnly_t isr;

      public:
        ChannelRegisters();
        void
        writeMemoryAddress(RegionAddress_t address);
        void
        writePeripheralSelect(PeripheralId_t id);
        void
        writeTransferCount(RegionSize_t count);
        void
        writeMemoryAddressReload(RegionAddress_t address);
        void
        writeTransferCountReload(RegionSize_t count);
        void
        writeControl(uint32_t mask);
        void
        writeMode(TransferSize_t size);
        void
        writeInterruptEnable(uint32_t mask);
        void
        writeInterruptDisable(uint32_t mask);
        uint32_t
        readStatus(void);

      };

      // Not (yet) implemented:
      //        Performance Monitor Registers
      //        Version Register

      inline void
      ChannelRegisters::writePeripheralSelect(pdca::PeripheralId_t id)
      {
        this->psr = id;
      }

      inline void
      ChannelRegisters::writeMemoryAddress(pdca::RegionAddress_t address)
      {
        this->mar = (uint32_t) address;
      }

      inline void
      ChannelRegisters::writeTransferCount(pdca::RegionSize_t count)
      {
        this->tcr = (uint32_t) count;
      }

      inline void
      ChannelRegisters::writeMemoryAddressReload(pdca::RegionAddress_t address)
      {
        this->marr = (uint32_t) address;
      }

      inline void
      ChannelRegisters::writeTransferCountReload(pdca::RegionSize_t count)
      {
        this->tcrr = (uint32_t) count;
      }

      // mask is logic OR between following possible flags:
      //          AVR32_PDCA_IER_RCZ_MASK, for Reload Counter Zero
      //          AVR32_PDCA_IER_TERR_MASK, for Transfer Error
      //          AVR32_PDCA_IER_TRC_MASK, for Transfer Complete
      inline void
      ChannelRegisters::writeInterruptEnable(uint32_t mask)
      {
        this->ier = mask;
      }

      // mask is logic OR between following possible flags:
      //          AVR32_PDCA_IDR_RCZ_MASK, for Reload Counter Zero
      //          AVR32_PDCA_IDR_TERR_MASK, for Transfer Error
      //          AVR32_PDCA_IDR_TRC_MASK, for Transfer Complete
      inline void
      ChannelRegisters::writeInterruptDisable(uint32_t mask)
      {
        this->idr = mask;
      }

      inline uint32_t
      ChannelRegisters::readStatus(void)
      {
        return this->sr;
      }

      // mask is logic OR between following possible flags:
      //          AVR32_PDCA_CR_TEN_MASK, for enabling channel
      //          AVR32_PDCA_CR_TDIS_MASK, for disabling channel
      //          AVR32_PDCA_CR_ECLR_MASK, for clearing transfer error
      inline void
      ChannelRegisters::writeControl(uint32_t mask)
      {
        // Writing a zero to any bit(flag) has no effect
        this->cr = (uint32_t) mask;
      }

      inline void
      ChannelRegisters::writeMode(pdca::TransferSize_t size)
      {
        this->mr = (uint32_t) size;
      }

    }
    // ----- Pdca base class --------------------------------------------------

    class Pdca
    {
    public:
      // Constructor for the given channel
      Pdca(pdca::ChannelId_t id);

      void
      setPeripheralId(pdca::PeripheralId_t id);
      pdca::PeripheralId_t
      getPeripheralId(void);

      void
      setRegionsArray(pdca::Region_t* pRegionsArray, uint_t regionsArraySize,
          bool isCircular);

      OSReturn_t
      prepareTransfer(void);

      void
      startTransfer(void);

      void
      registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler);

    public:
      // A reference (instead of a pointer) since it is not only more
      // convenient to use, but it is the only one that guarantees
      // not to change.
      pdca::ChannelRegisters& registers;

    private:
      int
      getNextRegionIndex(void);

      void
      setupReloadMechanism(void);

      pdca::ChannelId_t m_channelId;
      pdca::PeripheralId_t m_peripheralId;

      pdca::Region_t* m_pRegionsArray;
      uint_t m_regionsArraySize;
      bool m_isCircular;
      uint_t m_currentRegionIndex;
      int m_reloadedRegionIndex;
    };

    // ----- PdcaTransmit -----------------------------------------------------

    class PdcaTransmit : public Pdca
    {
    public:
      PdcaTransmit(pdca::ChannelId_t id);

      OSReturn_t
      waitWriteRegions(bool doNotBlock);
      void
      stopTransfer(void);
      void
      interruptServiceRoutine(void);
    };

    // ----- PdcaReceive ------------------------------------------------------

    class PdcaReceive : public Pdca
    {
    public:
      PdcaReceive(pdca::ChannelId_t id);

      OSReturn_t
      readRegion(pdca::RegionAddress_t& region, bool doNotBlock);
      void
      stopTransfer(void);

      void
      interruptServiceRoutine(void);
    };

    // ----- Pdca base inlines ------------------------------------------------

    inline void
    Pdca::setPeripheralId(pdca::PeripheralId_t id)
    {
      m_peripheralId = id;
    }

    inline pdca::PeripheralId_t
    Pdca::getPeripheralId(void)
    {
      return m_peripheralId;
    }

  // TODO: add the other
  }
}

#endif /* AVR32_UC3_PDCA_H_ */
