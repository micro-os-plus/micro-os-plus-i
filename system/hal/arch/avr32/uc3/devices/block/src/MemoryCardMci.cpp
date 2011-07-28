/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI)

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"
//#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
//#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardMci::MemoryCardMci(mci::CardSlot_t cardSlot) :
      OSDeviceMemoryCard(implementation), implementation(cardSlot)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardMci", this);
    }

    MemoryCardMci::~MemoryCardMci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci", this);
    }

    // ===== Implementation ===================================================

    MemoryCardMci::Implementation::Implementation(mci::CardSlot_t cardSlot)
    {
      OSDeviceDebug::putConstructor(
          "avr32::uc3::MemoryCardMci::Implementation", this);

      m_cardSlot = cardSlot;
    }

    MemoryCardMci::Implementation::~Implementation()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci::Implementation",
          this);
    }

    // ----- Public methods ---------------------------------------------------

    //    void
    //    MemoryCardMci::Implementation::setOpenParameters(void)
    //    {
    //    }

    // ---- Private virtuals Methods ------------------------------------------

    OSReturn_t
    MemoryCardMci::Implementation::initialise(void)
    {
      initGpio();

      // TODO: check if we need a special HMATRIX config, as in Atmel framework

#if false
      avr32::uc3::Pm pm;
      OSDeviceDebug::putString("PBB Mask=");
      OSDeviceDebug::putHex(pm.moduleRegisters.readPbbClockMask());
      OSDeviceDebug::putNewLine();
#endif

      // initialise MCI module
      m_mci.initialise(m_cardSlot);

      // Wait for 1ms, then wait for 74 more clock cycles (see MMC norms)
      if (m_mci.sendCommand(mci::CommandWord::SD_MMC_INIT_STATE_CMD, 0xFFFFFFFF)
          != MCI_SUCCESS)
        return OSReturn::OS_NOT_INITIALIZED;

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

      case CommandCode::READ_SINGLE_BLOCK:
        commandWord = mci::CommandWord::SD_MMC_READ_SINGLE_BLOCK_CMD;
        break;

      case CommandCode::READ_MULTIPLE_BLOCK:
        commandWord = mci::CommandWord::SD_MMC_READ_MULTIPLE_BLOCK_CMD;
        break;

      case CommandCode::STOP_TRANSMISSION:
        commandWord = mci::CommandWord::SD_MMC_STOP_READ_TRANSMISSION_CMD;
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
        OSDeviceDebug::putString("unknown CMD");
        OSDeviceDebug::putDec((uint16_t) commandCode);
        OSDeviceDebug::putNewLine();

        return OSReturn::OS_NOT_IMPLEMENTED;
        }

      // -----
#if defined(DEBUG)
      OSDeviceDebug::putString("MC ");
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
#endif /* defined(DEBUG) */

      if (m_mci.sendCommand(commandWord, commnadArg) != MCI_SUCCESS)
        return OSReturn::OS_BAD_COMMAND;
      else
        return OSReturn::OS_OK;
    }

    OSDeviceMemoryCard::Response_t
    MemoryCardMci::Implementation::readResponse(void)
    {
      OSDeviceMemoryCard::Response_t ret;
      ret = m_mci.moduleRegisters.readResponse(0);

      OSDeviceDebug::putString("MC R1=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

    OSReturn_t
    MemoryCardMci::Implementation::selectCard()
    {
      m_mci.configureSdCardBusWidthAndSlot(m_busWidth, m_cardSlot);

      OSDeviceDebug::putString("MC sdc=");
      OSDeviceDebug::putHex(m_mci.moduleRegisters.readSdCard());
      OSDeviceDebug::putNewLine();

      return OSReturn::OS_OK;
    }

    bool
    MemoryCardMci::Implementation::isBusy(void)
    {
      return m_mci.isBusy();
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureBusWidth(BusWidth_t busWidth)
    {
      m_busWidth = convertBusWidth(busWidth);
      m_mci.configureBusWidth(m_busWidth);
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureBlockLengthBytes(BlockLength_t length)
    {
      m_mci.configureBlockLengthBytes(length);
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureBlockCount(BlockCount_t count)
    {
      m_mci.configureBlockCount(count);
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureClockFrequencyHz(
        ClockFrequencyHz_t frequency)
    {
      m_mci.configureClockFrequencyHz(frequency);
      return OSReturn::OS_OK;
    }

    bool
    MemoryCardMci::Implementation::isRxReady(void)
    {
      return m_mci.isRxReady();
    }

    uint32_t
    MemoryCardMci::Implementation::readData(void)
    {
      return m_mci.readData();
    }

    bool
    MemoryCardMci::Implementation::isCrcError(void)
    {
      return m_mci.isCrcError();
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

    bool
    MemoryCardMci::Implementation::isTxReady(void)
    {
      return m_mci.isTxReady();
    }

    void
    MemoryCardMci::Implementation::writeData(uint32_t value)
    {
      m_mci.writeData(value);
    }

    // ----- Private methods --------------------------------------------------
    void
    MemoryCardMci::Implementation::initGpio(void)
    {
      OSDeviceDebug::putString("avr32::uc3::MemoryCardMci::initGpio()");
      OSDeviceDebug::putNewLine();

#if false
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
#endif

    }

  // --------------------------------------------------------------------------
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
