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
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardMci::MemoryCardMci() :
      OSDeviceMemoryCard(implementation)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardMci", this);
    }

    MemoryCardMci::~MemoryCardMci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci", this);
    }

     // ===== Implementation ===================================================

    MemoryCardMci::Implementation::Implementation()
    {
      OSDeviceDebug::putConstructor(
          "avr32::uc3::MemoryCardMci::Implementation", this);
    }

    MemoryCardMci::Implementation::~Implementation()
    {
      OSDeviceDebug::putDestructor(
          "avr32::uc3::MemoryCardMci::Implementation", this);
    }

    // ----- Private virtual methods ------------------------------------------

    void
    MemoryCardMci::Implementation::setOpenParameters(Mci::Speed_t speed,
        mci::BusWidth_t busWidth, mci::CardSlot_t cardSlot)
    {
      m_speed = speed;
      m_busWidth = busWidth;
      m_cardSlot = cardSlot;
    }

    // ---- Virtuals

    OSReturn_t
    MemoryCardMci::Implementation::init()
    {
      initGpio();

      // TODO: check if we need a special HMATRIX config, as in Atmel framework

#if false
      avr32::uc3::Pm pm;
      OSDeviceDebug::putString("PBB Mask=");
      OSDeviceDebug::putHex(pm.moduleRegisters.readPbbClockMask());
      OSDeviceDebug::putNewLine();
#endif

      // init MCI module
      m_mci.init(m_speed, m_busWidth, m_cardSlot);

      // Wait for 1ms, then wait for 74 more clock cycles (see MMC norms)
      if (m_mci.sendCommand(SD_MMC_INIT_STATE_CMD, 0xFFFFFFFF) != MCI_SUCCESS)
        return OSReturn::OS_ERROR;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::sendCommand(
        OSDeviceMemoryCard::CommandCode_t code,
        OSDeviceMemoryCard::CommandArgument_t arg)
    {
      avr32::uc3::mci::CommandCode_t mciCode;

      switch (code)
        {
      case CommandCode::GO_IDLE_STATE:
        mciCode = SD_MMC_GO_IDLE_STATE_CMD;
        break;

      case CommandCode::SEND_OP_COND:
        mciCode = SD_MMC_MMC_SEND_OP_COND_CMD;
        break;

      case CommandCode::SEND_IF_COND:
        mciCode = SD_MMC_SD_SEND_IF_COND_CMD;
        break;

      case CommandCode::APP_CMD:
        mciCode = SD_MMC_APP_CMD;
        break;

      case ApplicationCommandCode::SD_SEND_OP_COND:
        mciCode = SD_MMC_SDCARD_APP_OP_COND_CMD;
        break;

      default:
        OSDeviceDebug::putString("unknown cmd=");
        OSDeviceDebug::putDec((uint16_t) code);
        OSDeviceDebug::putNewLine();

        return OSReturn::OS_NOT_IMPLEMENTED;
        }
      //
      OSDeviceDebug::putString("SS cmd=");
      if ((code >> 8) != 0)
        {
          OSDeviceDebug::putDec((uint16_t) (code >> 8));
          OSDeviceDebug::putChar(' ');
        }
      OSDeviceDebug::putDec((uint16_t) (code & 0xFF));
      OSDeviceDebug::putNewLine();
      return m_mci.sendCommand(mciCode, arg);
    }

    OSDeviceMemoryCard::Response_t
    MemoryCardMci::Implementation::readResponse(void)
    {
      OSDeviceMemoryCard::Response_t ret;
      ret = m_mci.moduleRegisters.readResponse(0);

      OSDeviceDebug::putString("rsp=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

    OSReturn_t
    MemoryCardMci::Implementation::waitBusySignal(void)
    {
      // mci_wait_busy_signal()

      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::setBusWidth(BusWidth_t busWidth __attribute__((unused)))
    {
      // mci_set_bus_size()
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::setBlockSize(BlockSize_t size)
    {
      // mci_set_block_size
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::setBlockCount(BlockCount_t count)
    {
      // mci_set_block_count
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::mci_set_speed(void)
    {
      return OSReturn::OS_OK;
    }

    // ----- Private methods --------------------------------------------------
    void
    MemoryCardMci::Implementation::initGpio(void)
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
