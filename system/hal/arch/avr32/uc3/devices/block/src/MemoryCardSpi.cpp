/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDSPI)

#include "hal/arch/avr32/uc3/devices/block/include/MemoryCardSpi.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{
  namespace uc3
  {
    MemoryCardSpi::MemoryCardSpi(avr32::uc3::spi::ModuleId_t spiModuleId,
        avr32::uc3::gpio::PinNumber_t selectGpioNumber) :
      OSDeviceMemoryCard(implementation),
          implementation(spiModuleId, selectGpioNumber)
    {
      OSDeviceDebug::putConstructor("avr32::uc3::MemoryCardSpi", this);
    }

    MemoryCardSpi::~MemoryCardSpi()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardSpi", this);
    }

    // ===== Implementation ===================================================

    MemoryCardSpi::Implementation::Implementation(
        avr32::uc3::spi::ModuleId_t spiModuleId,
        avr32::uc3::gpio::PinNumber_t selectGpioNumber) :
      m_spi(spiModuleId), m_gpio(selectGpioNumber), m_cs(m_gpio)
    {
      OSDeviceDebug::putConstructor(
          "avr32::uc3::MemoryCardSpi::Implementation", this);
    }

    MemoryCardSpi::Implementation::~Implementation()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::MemoryCardSpi::Implementation",
          this);
    }

    // ----- Public methods ---------------------------------------------------

    //    void
    //    MemoryCardSpi::Implementation::setOpenParameters(void)
    //    {
    //      // TODO:
    //    }

    // ---- Private virtuals Methods ------------------------------------------

    OSReturn_t
    MemoryCardSpi::Implementation::initialise(void)
    {
#if false
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
#else
      m_cs.initialise();

      m_spi.initialise();
      m_spi.configureBaudRate(400000L);

      m_cs.assert();
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::sendCommand(
        OSDeviceMemoryCard::CommandCode_t commandCode __attribute__((unused)),
        OSDeviceMemoryCard::CommandArgument_t commnadArg __attribute__((unused)))
    {
#if false
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
          (uint16_t)(commandCode & OSDeviceMemoryCard::COMMAND_CODE_MASK));
      OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

      if (m_mci.sendCommand(commandWord, commnadArg) != MCI_SUCCESS)
      return OSReturn::OS_BAD_COMMAND;
      else
      return OSReturn::OS_OK;
#else
      return OSReturn::OS_OK;
#endif
    }

    OSDeviceMemoryCard::Response_t
    MemoryCardSpi::Implementation::readResponse(void)
    {
#if false
      OSDeviceMemoryCard::Response_t ret;
      ret = m_mci.moduleRegisters.readResponse(0);

      OSDeviceDebug::putString("R1=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
#else
      return OSReturn::OS_OK;
#endif
    }

    OSReturn_t
    MemoryCardSpi::Implementation::selectCard(BusWidth_t busWidth __attribute__((unused)))
    {
#if false
      m_mci.configureSdCardBusWidthAndSlot(busWidth, m_cardSlot);
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::waitNotBusy(void)
    {
#if false
      // mci_wait_busy_signal()
      while (!(m_mci.getStatusRegister() & AVR32_MCI_SR_NOTBUSY_MASK))
        {
          OSDeviceDebug::putChar('w');
        }
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::configureBusWidth(BusWidth_t busWidth __attribute__((unused)))
    {
#if false
      uint32_t mci_sdcr_register;

      // Warning: this code assumes that the MCI and the MemoryCardMci bus width
      // notation are identical!

      mci_sdcr_register = m_mci.moduleRegisters.readSdCard();
      mci_sdcr_register &= ~AVR32_MCI_SDCR_SDCBUS_MASK; // Clear previous buswidth
      mci_sdcr_register |= (busWidth << AVR32_MCI_SDCR_SDCBUS_OFFSET);
      m_mci.moduleRegisters.writeSdCard(mci_sdcr_register);
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::configureBlockLengthBytes(BlockLength_t length __attribute__((unused)))
    {
#if false
      m_mci.configureBlockLengthBytes(length);
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::configureBlockCount(BlockCount_t count __attribute__((unused)))
    {
#if false
      m_mci.configureBlockCount(count);
#endif
      return OSReturn::OS_OK;
    }

    OSReturn_t
    MemoryCardSpi::Implementation::configureClockFrequencyHz(uint32_t speed __attribute__((unused)))
    {
#if false
      m_mci.configureClockFrequencyHz(speed);
#endif
      return OSReturn::OS_OK;
    }

    bool
    MemoryCardSpi::Implementation::isRxReady(void)
    {
#if false
      return m_mci.isRxReady();
#else
      return false;
#endif
    }

    uint32_t
    MemoryCardSpi::Implementation::readData(void)
    {
#if false
      return ((m_mci.getStatusRegister() & AVR32_MCI_SR_RXRDY_MASK)) != 0;
#else
      return 0;
#endif
    }

    bool
    MemoryCardSpi::Implementation::isCrcError(void)
    {
#if false
      return m_mci.isCrcError();
#else
      return false;
#endif
    }

    void
    MemoryCardSpi::Implementation::setHighSpeedMode(void)
    {
#if false
      union u_cfg
        {
          unsigned long cfg;
          avr32_mci_cfg_t CFG;
        };
      union u_cfg val;
      val.cfg = m_mci.moduleRegisters.readConfiguration();
      val.CFG.hsmode = 1;
      m_mci.moduleRegisters.writeConfiguration(val.cfg);
#endif
    }

    bool
    MemoryCardSpi::Implementation::isTxReady(void)
    {
      return false;
    }

    void
    MemoryCardSpi::Implementation::writeData(uint32_t value __attribute__((unused)))
    {
      ;
    }

    // ----- Private methods --------------------------------------------------
    void
    MemoryCardSpi::Implementation::initGpio(void)
    {
      OSDeviceDebug::putString("avr32::uc3::MemoryCardSpi::initGpio()");
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
#endif /* defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDSPI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
