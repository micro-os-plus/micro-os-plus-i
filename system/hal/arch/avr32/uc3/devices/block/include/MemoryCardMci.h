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
    private:

      // ---- Implementation class --------------------------------------------

      class Implementation : public OSDeviceMemoryCard::Implementation
      {
      public:
        Implementation();
        virtual
        ~Implementation();

        void
        setOpenParameters(Mci::Speed_t speed, mci::BusWidth_t busWidth,
            mci::CardSlot_t cardSlot);

        friend class OSDeviceMemoryCard;

      private:
        virtual OSReturn_t
        init();

        virtual OSReturn_t
        sendCommand(CommandCode_t commandCode, CommandArgument_t commandArg);

        virtual OSDeviceMemoryCard::Response_t
        readResponse(void);

        virtual OSReturn_t
        waitBusySignal(void);

        virtual OSReturn_t
        setBusWidth(BusWidth_t busWidth);

        virtual OSReturn_t
        setBlockLength(BlockLength_t length);

        virtual OSReturn_t
        setBlockCount(BlockCount_t count);

        virtual OSReturn_t
        mci_set_speed(uint32_t speed);

        virtual OSReturn_t
        mci_select_card(BusWidth_t busWidth);

        virtual bool
        mci_rx_ready(void);

        virtual uint32_t
        mci_rd_data(void);

        virtual void
        setHighSpeedMode(void);

      private:

        // ---- Local methods -------------------------------------------------

        void
        initGpio(void);

      public:
        avr32::uc3::Mci m_mci;

        Mci::Speed_t m_speed;
        mci::BusWidth_t m_busWidth;
        mci::CardSlot_t m_cardSlot;

      };

    public:

      // ----- Constructors & Destructors -------------------------------------

      MemoryCardMci();

      virtual
      ~MemoryCardMci();

    public:

      // ----- public members ------------------------------------------------

      Implementation implementation;

      // ----------------------------------------------------------------------
    };

  }
}

#endif /* AVR32_UC3_MEMORYCARDMCI_H_ */
