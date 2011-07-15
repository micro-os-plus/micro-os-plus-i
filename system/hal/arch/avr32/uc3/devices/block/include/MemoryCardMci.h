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

      // ----- Type definitions -----------------------------------------------


    public:

      // ----- Constructors & Destructors -------------------------------------

      MemoryCardMci();

      virtual
      ~MemoryCardMci();

    public:

      // ----- Public methods -------------------------------------------------

      void
      setOpenParameters(Mci::Speed_t speed, mci::BusWidth_t busWidth,
          mci::CardSlot_t cardSlot);

    private:

      // ----- Private methods ------------------------------------------------

      virtual OSReturn_t
      implInit();

      virtual OSReturn_t
      implSendCommand(CommandCode_t code, CommandArgument_t arg);

      virtual OSDeviceMemoryCard::Response_t
      implReadResponse(void);

      virtual OSReturn_t
      implWaitBusySignal(void);

      virtual OSReturn_t
      implSetBusWidth(BusWidth_t busWidth);

      virtual OSReturn_t
      implSetBlockSize(BlockSize_t size);

      virtual OSReturn_t
      implSetBlockCount(BlockCount_t count);

      virtual OSReturn_t
      mci_set_speed(void);

      // ----
      void
      initGpio(void);

    private:

      // ----- Private members ------------------------------------------------

      avr32::uc3::Mci m_mci;

      Mci::Speed_t m_speed;
      mci::BusWidth_t m_busWidth;
      mci::CardSlot_t m_cardSlot;

      // ----------------------------------------------------------------------
    };
  }
}

#endif /* AVR32_UC3_MEMORYCARDMCI_H_ */
