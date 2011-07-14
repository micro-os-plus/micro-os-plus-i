/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MEMORYCARDMCI_H_
#define AVR32_UC3_MEMORYCARDMCI_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressable.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"

namespace avr32
{
  namespace uc3
  {
    class MemoryCardMci : public OSDeviceAddressable
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

      // Prepare the device for operations.
      virtual OSReturn_t
      open(void);

      // Read exactly count bytes, in the given buffer.
      // Offset need not be aligned.
      virtual OSReturn_t
      readBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
          OSDeviceAddressable::Count_t count);

      // Write the given buffer. Offset need to be aligned with
      // getWriteAlignement() and count should be multiple of it.
      virtual OSReturn_t
      writeBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
          OSDeviceAddressable::Count_t count);

      // Prepare device for entering sleep
      virtual OSReturn_t
      close(void);

      // Erase part of the device, aligned to page if needed.
      virtual OSReturn_t
      erase(OSDeviceAddressable::Offset_t offset,
          OSDeviceAddressable::Count_t count);

      // Erase the entire device
      virtual OSReturn_t
      eraseEntireDevice(void);

      // Return the full size, in bytes, of the device.
      virtual OSDeviceAddressable::Offset_t
      getDeviceSize(void);

      // Return the write block size, to be used as alignment.
      virtual OSDeviceAddressable::Alignnment_t
      getWriteAlignment(void);

      // Return the erase page size, to be used as alignment.
      virtual OSDeviceAddressable::Alignnment_t
      getEraseAlignment(void);

    private:

      // ----- Private methods ------------------------------------------------

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
