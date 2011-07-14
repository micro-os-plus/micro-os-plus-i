/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI)

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardMci::MemoryCardMci()
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardMci", this);
    }

    MemoryCardMci::~MemoryCardMci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci", this);
    }

    // ----- Public methods ---------------------------------------------------

    void
    MemoryCardMci::setOpenParameters(Mci::Speed_t speed,
        mci::BusWidth_t busWidth, mci::CardSlot_t cardSlot)
    {
      m_speed = speed;
      m_busWidth = busWidth;
      m_cardSlot = cardSlot;
    }

    OSReturn_t
    MemoryCardMci::open(void)
    {
      OSDeviceDebug::putString_P(PSTR("avr32::uc3::MemoryCardMci::open()"));
      OSDeviceDebug::putNewLine();

      initGpio();

      // TODO: check if we need a special HMATRIX config, as in Atmel framework

      // init MCI module
      m_mci.init(m_speed, m_busWidth, m_cardSlot);

      // Wait for 1ms, then wait for 74 more clock cycles (see MMC norms)
      if (m_mci.sendCommand(SD_MMC_INIT_STATE_CMD, 0xFFFFFFFF) != MCI_SUCCESS)
        return OSReturn::OS_ERROR;

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    OSDeviceAddressable::Offset_t
    MemoryCardMci::getDeviceSize(void)
    {
      return 0; // No device
    }

    OSReturn_t
    MemoryCardMci::readBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
        uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
    {
      OSDeviceDebug::putString_P(PSTR("avr32::uc3::MemoryCardMci::readBytes()"));
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    OSReturn_t
    MemoryCardMci::writeBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
        uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
    {
      OSDeviceDebug::putString_P(
          PSTR("avr32::uc3::MemoryCardMci::writeBytes()"));
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    OSDeviceAddressable::Alignnment_t
    MemoryCardMci::getWriteAlignment(void)
    {
      return 512; // Default alignment size, from MMC specs
    }

    OSReturn_t
    MemoryCardMci::close(void)
    {
      OSDeviceDebug::putString_P(PSTR("avr32::uc3::MemoryCardMci::close()"));
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    OSReturn_t
    MemoryCardMci::erase(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
        OSDeviceAddressable::Count_t count __attribute__((unused)))
    {
      OSDeviceDebug::putString_P(PSTR("avr32::uc3::MemoryCardMci::erase()"));
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    OSDeviceAddressable::Alignnment_t
    MemoryCardMci::getEraseAlignment(void)
    {
      return 512; // Default alignment size, from MMC specs
    }

    OSReturn_t
    MemoryCardMci::eraseEntireDevice(void)
    {
      OSDeviceDebug::putString_P(
          PSTR("avr32::uc3::MemoryCardMci::eraseEntireDevice()"));
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_NOT_IMPLEMENTED;
    }

    // ----- Private methods --------------------------------------------------

    void
    MemoryCardMci::initGpio(void)
    {
      OSDeviceDebug::putString("avr32::uc3::MemoryCardMci::initGpio()");
      OSDeviceDebug::putNewLine();

      // CLK
      avr32::uc3::Gpio::configPeripheralModeAndFunction(27,
          avr32::uc3::gpio::PeripheralFunction::A);

      if (m_cardSlot == avr32::uc3::mci::CardSlot::A)
        {
          // CMD0 (connected to card 0 CMD)
          avr32::uc3::Gpio::configPeripheralModeAndFunction(28,
              avr32::uc3::gpio::PeripheralFunction::A);

          // DATA[0-3] (connected to card 0 DATA[0-3]
          avr32::uc3::Gpio::configPeripheralModeAndFunction(29,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(30,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(31,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(32,
              avr32::uc3::gpio::PeripheralFunction::A);
        }
      else
        {
          // CMD1 (connected to card 1 CMD)
          avr32::uc3::Gpio::configPeripheralModeAndFunction(15,
              avr32::uc3::gpio::PeripheralFunction::A);

          // DATA[4-7] (connected to card 0 DATA[0-3]
          avr32::uc3::Gpio::configPeripheralModeAndFunction(33,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(34,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(35,
              avr32::uc3::gpio::PeripheralFunction::A);
          avr32::uc3::Gpio::configPeripheralModeAndFunction(36,
              avr32::uc3::gpio::PeripheralFunction::A);
        }
    }

  // --------------------------------------------------------------------------
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
