/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MEMORYCARDMCI_H_
#define AVR32_UC3_MEMORYCARDMCI_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceMemoryCard.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    class MemoryCardMci : public OSDeviceMemoryCard
    {
    public:

      // ---- Implementation class --------------------------------------------

      class Implementation : public OSDeviceMemoryCard::Implementation
      {
      public:
        Implementation(mci::CardSlot_t cardSlot, gpio::PinNumber_t data0gpio);
        virtual
        ~Implementation();

        friend class OSDeviceMemoryCard;

      private:
        virtual OSReturn_t
        initialise();

        virtual OSReturn_t
        sendCommand(CommandCode_t commandCode, CommandArgument_t commandArg);

        virtual OSDeviceMemoryCard::Response_t
        readResponse(void);

        virtual bool
        isBusy(void);

        virtual bool
        isTransferDone(void);

        virtual bool
        isDataLineBusy(void);

        virtual OSReturn_t
        configureBusWidth(BusWidth_t busWidth);

        virtual OSReturn_t
        configureBlockLengthBytes(BlockLength_t length);

        virtual OSReturn_t
        configureBlockCount(BlockCount_t count);

        virtual OSReturn_t
        configureClockFrequencyHz(ClockFrequencyHz_t frequency);

        virtual OSReturn_t
        selectCard(void);

        virtual void
        configureHighSpeedMode(void);

        virtual bool
        isCrcError(void);

        void
        transferIncommingBytes(void *pBuf, size_t bytes);

        virtual void
        transferOutgoingBytes(void *pBuf, size_t bytes);

        virtual void
        transferIncommingBytesWithoutDma(void *pBuf, size_t bytes);

      private:

        // ---- Local methods -------------------------------------------------

        bool
        isRxReady(void);

        uint32_t
        readData(void);

        bool
        isTxReady(void);

        void
        writeData(uint32_t data);

        avr32::uc3::mci::BusWidth_t
        convertBusWidth(BusWidth_t busWidth);

        void
        waitRxReady(void);

        void
        waitTxReady(void);

      public:

        avr32::uc3::Mci m_mci;

        Mci::ClockFrequencyHz_t m_speed;
        mci::BusWidth_t m_busWidth;
        mci::CardSlot_t m_cardSlot;

        avr32::uc3::Gpio m_dataLine;

      };

    public:

      // ----- Constructors & Destructors -------------------------------------

      MemoryCardMci(mci::CardSlot_t cardSlot, gpio::PinNumber_t data0gpio);

      virtual
      ~MemoryCardMci();

    public:

      // ----- Public members -------------------------------------------------

      Implementation implementation;

      // ----------------------------------------------------------------------
    };

    inline avr32::uc3::mci::BusWidth_t
    MemoryCardMci::Implementation::convertBusWidth(BusWidth_t busWidth)
    {
      // The AVR32 bus width uses the same encoding, so nothing to do
      return busWidth;
    }
  }
}

#endif /* AVR32_UC3_MEMORYCARDMCI_H_ */
