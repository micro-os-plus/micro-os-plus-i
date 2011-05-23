/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_PDCA_H_
#define AVR32_UC3_PDCA_H_

#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    // ----- Pdca base class --------------------------------------------------

    class Pdca
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef enum ChannelId_e
      {
        // Define one of the eight channels
        zero = 0,
        one,
        two,
        three,
        four,
        five,
        six,
        seven = 7
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
        adcRx = 0,
        sscRx = 1,
        usart0rx = 2,
        twim0rx = 6,
        spi0rx = 10,
        spi1rx = 11,
        usart0tx = 13,
        twim0tx = 17,
        spi0tx = 21,
        spi1tx = 22,
        abdacTx = 23
      } PeripheralId_t;

      typedef enum TransferSize_e
      {
        // Define the transfer size
        oneByte = 0,
        twoBytes = 1,
        fourBytes = 2
      } TransferSize_t;

      // ----- Channel Memory Mapped Registers --------------------------------

      class ChannelRegisters
      {
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
        uint32_t
        readStatus(void);

      private:
        // TODO add volatile to fields that change by themselves
        uint32_t mar;
        uint32_t psr;
        uint32_t tcr;
        uint32_t marr;
        uint32_t tcrr;
        uint32_t cr;
        uint32_t mr;
        uint32_t sr;
        uint32_t ier;
        uint32_t idr;
        uint32_t imr;
        uint32_t isr;
      };

    public:
      // Constructor for the given channel
      Pdca(ChannelId_t id);

      void
      setPeripheralId(PeripheralId_t id);
      PeripheralId_t
      getPeripheralId(void);

      void
      setRegionsArray(Region_t* pRegionsArray, uint_t regionsArraySize,
          bool isCircular);

      OSReturn_t
      prepareTransfer(void);

      void
      startTransfer(void);

      void
      registerInterruptHandler(void* handler);

      // Not (yet) implemented:
      //        Performance Monitor Registers
      //        Version Register

    public:
      // A reference (instead of a pointer) since it is not only more
      // convenient to use, but it is the only one that guarantees
      // not to change.
      ChannelRegisters& registers;

    private:
      PeripheralId_t m_peripheralId;

      Region_t* m_pRegionsArray;
      uint_t m_regionsArraySize;
      bool m_isCircular;
    };

    // ----- PdcaTransmit -----------------------------------------------------

    class PdcaTransmit : public Pdca
    {
    public:
      PdcaTransmit(ChannelId_t id);

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
      PdcaReceive(ChannelId_t id);

      OSReturn_t
      readRegion(RegionAddress_t& region, bool doNotBlock);
      void
      stopTransfer(void);

      void
      interruptServiceRoutine(void);
    };

    // ----- Pdca base inlines ------------------------------------------------

    inline void
    Pdca::setPeripheralId(PeripheralId_t id)
    {
      m_peripheralId = id;
    }

    inline Pdca::PeripheralId_t
    Pdca::getPeripheralId(void)
    {
      return m_peripheralId;
    }

    inline void
    Pdca::ChannelRegisters::writePeripheralSelect(PeripheralId_t id)
    {
      psr = id;
    }

    inline void
    Pdca::ChannelRegisters::writeMemoryAddress(RegionAddress_t address)
    {
      mar = (uint32_t) address;
    }

    inline void
    Pdca::ChannelRegisters::writeTransferCount(RegionSize_t count)
    {
      tcr = (uint32_t) count;
    }

    inline uint32_t
    Pdca::ChannelRegisters::readStatus(void)
    {
      return sr;
    }

  // TODO: add the other
  }
}

#endif /* AVR32_UC3_PDCA_H_ */
