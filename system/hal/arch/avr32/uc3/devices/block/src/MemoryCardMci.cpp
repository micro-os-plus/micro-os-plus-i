/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI)

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardMci.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardMci::MemoryCardMci(mci::CardSlot_t cardSlot,
        gpio::PinNumber_t data0gpio,
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioArray) :
      OSDeviceMemoryCard(implementation),
          implementation(cardSlot, data0gpio, pGpioArray)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardMci", this);
    }

    MemoryCardMci::~MemoryCardMci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci", this);
    }

    // ===== Implementation ===================================================

    MemoryCardMci::Implementation::Implementation(mci::CardSlot_t cardSlot,
        gpio::PinNumber_t data0,
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioArray) :
      m_dataLine(data0)
    {
      OSDeviceDebug::putConstructor(
          "avr32::uc3::MemoryCardMci::Implementation", this);

      m_cardSlot = cardSlot;
      m_pGpioArray = pGpioArray;
    }

    MemoryCardMci::Implementation::~Implementation()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardMci::Implementation",
          this);
    }

    // ---- Private virtuals methods ------------------------------------------

    void
    MemoryCardMci::Implementation::powerUp(void)
    {
      OSDeviceDebug::putString("MemoryCardMci::Implementation::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction( m_pGpioArray);
        }
    }

    void
    MemoryCardMci::Implementation::powerDown(void)
    {
      OSDeviceDebug::putString("MemoryCardMci::Implementation::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput( m_pGpioArray);
        }
    }

    OSReturn_t
    MemoryCardMci::Implementation::initialise(void)
    {
      //initGpio();

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
        return OSReturn::OS_NOT_INITIALISED;

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

      case CommandCode::WRITE_BLOCK:
        commandWord = mci::CommandWord::SD_MMC_WRITE_BLOCK_CMD;
        break;

      case CommandCode::ERASE_WR_BLK_START:
        commandWord = mci::CommandWord::SD_MMC_TAG_SECTOR_START_CMD;
        break;

      case CommandCode::ERASE_WR_BLK_END:
        commandWord = mci::CommandWord::SD_MMC_TAG_SECTOR_END_CMD;
        break;

      case CommandCode::ERASE:
        commandWord = mci::CommandWord::SD_MMC_ERASE_CMD;
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
#if defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SENDCOMMAND)
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
#endif /* defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SENDCOMMAND) */

      mci::StatusRegister_t status;
      status = m_mci.sendCommand(commandWord, commnadArg);

#if false && defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SENDCOMMAND)
      OSDeviceDebug::putString("MC stat=");
      OSDeviceDebug::putHex(status);
      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SENDCOMMAND) */

      if (status == MCI_SUCCESS)
        return OSReturn::OS_OK;

      if ((status & (AVR32_MCI_SR_CSTOE_MASK | AVR32_MCI_SR_RTOE_MASK)) != 0)
        return OSReturn::OS_TIMEOUT;

      // TODO: maybe other return codes are also interesting
      if ((status & MCI_SR_ERROR) != 0)
        return OSReturn::OS_BAD_COMMAND;

      return OSReturn::OS_ERROR;
    }

    OSDeviceMemoryCard::Response_t
    MemoryCardMci::Implementation::readResponse(void)
    {
      OSDeviceMemoryCard::Response_t ret;
      ret = m_mci.moduleRegisters.readResponse(0);

#if defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_READRESPONSE)
      OSDeviceDebug::putString("MC R1=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_READRESPONSE) */

      return ret;
    }

    OSReturn_t
    MemoryCardMci::Implementation::selectCard()
    {
      m_mci.configureSdCardBusWidthAndSlot(m_busWidth, m_cardSlot);

#if defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SELECTCARD)
      OSDeviceDebug::putString("MC sdc=");
      OSDeviceDebug::putHex(m_mci.moduleRegisters.readSdCard());
      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SELECTCARD) */

      return OSReturn::OS_OK;
    }

    bool
    MemoryCardMci::Implementation::isBusy(void)
    {
      return m_mci.isBusy();
    }

    bool
    MemoryCardMci::Implementation::isTransferDone(void)
    {
      return m_mci.isTransferDone();
    }

    bool
    MemoryCardMci::Implementation::isDataLineBusy(void)
    {
      bool isPinHigh;
      m_dataLine.configureModeGpio();
      m_dataLine.configureDirectionInput();
      isPinHigh = m_dataLine.isPinHigh();
      
      // TODO: use the pin array (pass an index into this array, not a pin number)
      m_dataLine.configurePeripheralFunction(
          avr32::uc3::gpio::PeripheralFunction::A);

      return !isPinHigh;
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureBusWidth(BusWidth_t busWidth)
    {
      m_busWidth = convertBusWidth(busWidth);
      m_mci.configureBusWidth(m_busWidth);
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardMci::Implementation::configureBlockLengthBytes(
        BlockLength_t length)
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

    void
    MemoryCardMci::Implementation::waitRxReady(void)
    {
      while (!isRxReady())
        {
          OSDeviceDebug::putChar('R');
          OSScheduler::timerTicks.sleep(1);
          //OSScheduler::yield();
        }
    }

    void
    MemoryCardMci::Implementation::waitTxReady(void)
    {
      while (!isTxReady())
        {
          OSDeviceDebug::putChar('T');
          OSScheduler::timerTicks.sleep(1);
          //OSScheduler::yield();
        }
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
    MemoryCardMci::Implementation::configureHighSpeedMode(void)
    {
      m_mci.configureHighSpeedMode();
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

    void
    MemoryCardMci::Implementation::transferIncommingBytes(void *pBuf,
        size_t bytes)
    {

#if false
      OSDeviceDebug::putString("ptr=");
      OSDeviceDebug::putPtr(pLong);
      OSDeviceDebug::putString(", bytes=");
      OSDeviceDebug::putDec(bytes);
      OSDeviceDebug::putNewLine();
#endif

      uint32_t wordsToRead;

      // Warning: buffer must be word aligned
      uint32_t* pLong = (uint32_t*) pBuf;

      wordsToRead = (bytes / sizeof(*pLong));

      while (wordsToRead > 0)
        {
          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          waitRxReady();
          *pLong++ = readData();

          wordsToRead -= 8;
        }
      //OSDeviceDebug::putString("done");
      //OSDeviceDebug::putNewLine();
    }

    void
    MemoryCardMci::Implementation::transferIncommingBytesWithoutDma(void *pBuf,
        size_t bytes)
    {
      uint32_t wordsToRead;

      // Warning: buffer must be word aligned
      uint32_t* pLong = (uint32_t*) pBuf;

      wordsToRead = (bytes / sizeof(*pLong));

      while (wordsToRead > 0)
        {
          waitRxReady();
          *pLong++ = readData();

          wordsToRead--;
        }
    }

    void
    MemoryCardMci::Implementation::transferOutgoingBytes(void *pBuf,
        size_t bytes)
    {
      uint32_t wordsToWrite;
      const uint32_t *pLong = (const uint32_t *) pBuf;

      wordsToWrite = (bytes / sizeof(*pLong));

      while (wordsToWrite > 0)
        {
          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          waitTxReady();
          writeData(*pLong++);

          wordsToWrite -= 8;
        }
    }

  // --------------------------------------------------------------------------
  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
