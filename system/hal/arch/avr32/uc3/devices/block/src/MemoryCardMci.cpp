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
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci::Implementation",
          this);
    }

    // ----- Public methods ---------------------------------------------------

    void
    MemoryCardMci::Implementation::setOpenParameters(Mci::Speed_t speed,
        mci::BusWidth_t busWidth, mci::CardSlot_t cardSlot)
    {
      m_speed = speed;
      m_busWidth = busWidth;
      m_cardSlot = cardSlot;
    }

    // ---- Private virtuals Methods ------------------------------------------

    OSReturn_t
    MemoryCardMci::Implementation::init(void)
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
      if (m_mci.sendCommand(mci::CommandWord::SD_MMC_INIT_STATE_CMD, 0xFFFFFFFF)
          != MCI_SUCCESS)
        return OSReturn::OS_ERROR;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::sendCommand(
        OSDeviceMemoryCard::CommandCode_t commandCode,
        OSDeviceMemoryCard::CommandArgument_t commnadArg)
    {
      avr32::uc3::mci::CommandWord_t commandWord;

      switch (commandCode)
        {
      case CommandCode::GO_IDLE_STATE:
        commandWord = mci::CommandWord::SD_MMC_GO_IDLE_STATE_CMD;
        break;

      case CommandCode::SEND_OP_COND:
        commandWord = mci::CommandWord::SD_MMC_MMC_SEND_OP_COND_CMD;
        break;

      case CommandCode::SEND_IF_COND:
        commandWord = mci::CommandWord::SD_MMC_SD_SEND_IF_COND_CMD;
        break;

      case CommandCode::APP_CMD:
        commandWord = mci::CommandWord::SD_MMC_APP_CMD;
        break;

      case CommandCode::ALL_SEND_CID:
        commandWord = mci::CommandWord::SD_MMC_ALL_SEND_CID_CMD;
        break;

      case CommandCode::SEND_RELATIVE_ADDR:
        commandWord = mci::CommandWord::SD_MMC_SET_RELATIVE_ADDR_CMD;
        break;

      case CommandCode::SEND_CSD:
        commandWord = mci::CommandWord::SD_MMC_SEND_CSD_CMD;
        break;

      case CommandCode::SELECT_DESELECT_CARD:
        commandWord = mci::CommandWord::SD_MMC_SEL_DESEL_CARD_CMD;
        break;

      case CommandCode::SET_BLOCKLEN:
        commandWord = mci::CommandWord::SD_MMC_SET_BLOCKLEN_CMD;
        break;

      case CommandCode::SEND_STATUS:
        commandWord = mci::CommandWord::SD_MMC_SEND_STATUS_CMD;
        break;

        // ----- Application commands -----------------------------------------
      case ApplicationCommandCode::SD_SEND_OP_COND:
        commandWord = mci::CommandWord::SD_MMC_SDCARD_APP_OP_COND_CMD;
        break;

      case ApplicationCommandCode::SEND_EXT_CSD:
        commandWord = mci::CommandWord::SD_MMC_SEND_EXT_CSD_CMD;
        break;

      case ApplicationCommandCode::SET_BUS_WIDTH:
        commandWord = mci::CommandWord::SD_MMC_SDCARD_SET_BUS_WIDTH_CMD;
        break;

        // ----- Switch commands ----------------------------------------------
      case SwitchCommandCode::SD_SWITCH_FUNC:
        commandWord = mci::CommandWord::SD_SWITCH_FUNC;
        break;

      case SwitchCommandCode::MMC_SWITCH_FUNC:
        commandWord = mci::CommandWord::MMC_SWITCH_CMD;
        break;

      default:
        OSDeviceDebug::putString("unknown cmd=");
        OSDeviceDebug::putDec((uint16_t) commandCode);
        OSDeviceDebug::putNewLine();

        return OSReturn::OS_NOT_IMPLEMENTED;
        }

      // -----

      OSDeviceMemoryCard::CommandClass_t cmsClass;
      cmsClass = (commandCode >> OSDeviceMemoryCard::COMMAND_CLASS_SHIFT);
      if (cmsClass == OSDeviceMemoryCard::CommandClass::APPLICATION_8)
        OSDeviceDebug::putString("ACMD");
      else
        {
          if (cmsClass == OSDeviceMemoryCard::CommandClass::SWITCH_10)
            {
              OSDeviceDebug::putString("SW_");
            }
          else if (cmsClass != 0)
            {
              OSDeviceDebug::putString("GRP");
              OSDeviceDebug::putDec((uint16_t) cmsClass);
              OSDeviceDebug::putChar('_');
            }
          OSDeviceDebug::putString("CMD");
        }
      OSDeviceDebug::putDec(
          (uint16_t) (commandCode & OSDeviceMemoryCard::COMMAND_CODE_MASK));
      OSDeviceDebug::putNewLine();

      return m_mci.sendCommand(commandWord, commnadArg);
    }

    OSDeviceMemoryCard::Response_t
    MemoryCardMci::Implementation::readResponse(void)
    {
      OSDeviceMemoryCard::Response_t ret;
      ret = m_mci.moduleRegisters.readResponse(0);

      OSDeviceDebug::putString("R1=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

    OSReturn_t
    MemoryCardMci::Implementation::mci_select_card(BusWidth_t busWidth)
    {
      m_mci.initSdCardBusWidthAndSlot(busWidth, m_cardSlot);

      return OSReturn::OS_OK;
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
    MemoryCardMci::Implementation::setBlockLength(BlockLength_t length __attribute__((unused)))
    {
      // mci_set_block_size
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::setBlockCount(BlockCount_t count __attribute__((unused)))
    {
      // mci_set_block_count
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::mci_set_speed(uint32_t speed)
    {
      m_mci.initSpeed(speed);
      return OSReturn::OS_OK;
    }

    bool
    MemoryCardMci::Implementation::mci_rx_ready(void)
    {
      return m_mci.mci_rx_ready();
    }

    uint32_t
    MemoryCardMci::Implementation::mci_rd_data(void)
    {
      return (m_mci.getStatusRegister() & AVR32_MCI_SR_RXRDY_MASK) != 0;
    }

    void
    MemoryCardMci::Implementation::setHighSpeedMode(void)
    {
      union u_cfg
      {
        unsigned long cfg;
        avr32_mci_cfg_t CFG;
      };
      union u_cfg val;
      val.cfg = m_mci.moduleRegisters.readConfiguration();
      val.CFG.hsmode = 1;
      m_mci.moduleRegisters.writeConfiguration(val.cfg);
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
