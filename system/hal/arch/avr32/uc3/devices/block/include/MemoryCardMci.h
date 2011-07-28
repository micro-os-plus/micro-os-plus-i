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
        Implementation(mci::CardSlot_t cardSlot);
        virtual
        ~Implementation();

        //void
        //setOpenParameters(void);

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

        virtual bool
        isRxReady(void);

        virtual uint32_t
        readData(void);

        virtual void
        setHighSpeedMode(void);

        virtual bool
        isCrcError(void);

        virtual bool
        isTxReady(void);

        virtual void
        writeData(uint32_t data);

      private:

        // ---- Local methods -------------------------------------------------

        void
        initGpio(void);

        avr32::uc3::mci::BusWidth_t
        convertBusWidth(BusWidth_t busWidth);

      public:
        avr32::uc3::Mci m_mci;

        Mci::ClockFrequencyHz_t m_speed;
        mci::BusWidth_t m_busWidth;
        mci::CardSlot_t m_cardSlot;

      };

    public:

      // ----- Constructors & Destructors -------------------------------------

      MemoryCardMci(mci::CardSlot_t cardSlot);

      virtual
      ~MemoryCardMci();

    public:

      // ----- public members -------------------------------------------------

      Implementation implementation;

      // ----------------------------------------------------------------------
    };

    inline avr32::uc3::mci::BusWidth_t
    MemoryCardMci::Implementation::convertBusWidth(BusWidth_t busWidth)
    {
      // The AVR32 bus width uses the same values, nothing to do
      return busWidth;
    }
  }
}

#endif /* AVR32_UC3_MEMORYCARDMCI_H_ */
