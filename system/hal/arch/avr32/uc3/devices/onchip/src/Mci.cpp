/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_MCI)

#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"

namespace avr32
{
  namespace uc3
  {

    // ---- Constructors & Destructors ----------------------------------------

    Mci::Mci() :
          moduleRegisters(
              *reinterpret_cast<mci::ModuleRegisters*> (mci::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Mci", this);
      OSDeviceDebug::putString("MCI Version=");
      OSDeviceDebug::putHex(moduleRegisters.readVersion());
      OSDeviceDebug::putNewLine();
    }

    Mci::~Mci()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Mci", this);
    }

    // ----- Public methods ---------------------------------------------------

    void
    Mci::init(mci::CardSlot_t cardSlot)
    {
      OSDeviceDebug::putString("avr32::uc3::Mci::init()");
      OSDeviceDebug::putNewLine();

      m_shadowStatusRegister = 0;

      reset();
      disable();

      disableAllInterrupts();

      clearConfiguration();
      clearModeBits();

      // Set the Data Timeout Register to 1 Mega Cycles
      initDataTimeout(MCI_DEFAULT_DTOLMUL, MCI_DEFAULT_DTOLCYC);

      // Start with low speed
      configSpeed(400000);

      // Set the Mode register
      enableModeBits(
          (MCI_DEFAULT_PWSDIV << AVR32_MCI_MR_PWSDIV)
              | AVR32_MCI_MR_RDPROOF_MASK | AVR32_MCI_MR_WRPROOF_MASK);

      // Set the SD/MMC Card register
      initSdCardBusWidthAndSlot(mci::BusWidth::_1bit, cardSlot);

      // Enable the MCI and the Power Saving
      enable();
    }

    void
    Mci::enableModeBits(uint32_t mask)
    {
      uint32_t mode;
      mode = moduleRegisters.readMode(); // Read original mode
        {
          mode |= (mask);
        }
      moduleRegisters.writeMode(mode); // Write New mode
    }

    void
    Mci::configSpeed(Speed_t speed)
    {
      uint32_t mode;
      mode = moduleRegisters.readMode();

      if (speed > AVR32_MCI_HSDIS_MAX_FREQ)
        {
          // Use of the High Speed mode of the MCI macro.
          uint16_t config;
          config = moduleRegisters.readConfiguration();
          config |= 0x0100; // HSMODE
          moduleRegisters.writeConfiguration(config);
        }

      uint16_t clkdiv;
      uint16_t rest;

      // Multimedia Card Interface clock (MCCK or MCI_CK) is Master Clock (MCK)
      // divided by (2*(CLKDIV+1))
      clkdiv = OS_CFGLONG_PBB_FREQUENCY_HZ / (speed * 2);
      rest = OS_CFGLONG_PBB_FREQUENCY_HZ % (speed * 2);
      if (rest != 0)
        {
          // Ensure that the card_speed can not be higher than expected.
          clkdiv += 1;
        }

      if (clkdiv > 0)
        {
          clkdiv -= 1;
        }

      OSDeviceDebug::putString("speed=");
      OSDeviceDebug::putDec(speed);
      OSDeviceDebug::putString(", clkdev=");
      OSDeviceDebug::putHex(clkdiv);
      OSDeviceDebug::putNewLine();

      // Fill in the new clock divider to mode
      mode &= ~AVR32_MCI_MR_CLKDIV_MASK; // Clear previous clkdiv value
      mode |= clkdiv; // Set the new one

      // Write new configuration
      moduleRegisters.writeMode(mode);
    }

    void
    Mci::initSdCardBusWidthAndSlot(mci::BusWidth_t busWidth,
        mci::CardSlot_t cardSlot)
    {
      moduleRegisters.writeSdCard(
          ((busWidth & 0x3) >> AVR32_MCI_SDCR_SDCBUS_OFFSET)
              | ((cardSlot & 0x3) >> AVR32_MCI_SDCR_SDCSEL_OFFSET));
    }

    mci::StatusRegister_t
    Mci::getStatusRegister(void)
    {
      // The trick here is to preserve some bits across calls.
      // Currently only AVR32_MCI_SR_DCRCE_MASK is used, and cleared
      // in isCrcError()
      m_shadowStatusRegister = (m_shadowStatusRegister & (AVR32_MCI_SR_DTOE_MASK
          | AVR32_MCI_SR_DCRCE_MASK | AVR32_MCI_SR_CSTOE_MASK
          | AVR32_MCI_SR_BLKOVRE_MASK)) | moduleRegisters.readStatus();
      return m_shadowStatusRegister;
    }

    bool
    Mci::wasLastCommandSent(void)
    {
      return ((getStatusRegister() & AVR32_MCI_SR_CMDRDY_MASK) != 0);
    }

    bool
    Mci::isCrcError(void)
    {
      if (getStatusRegister() & AVR32_MCI_SR_DCRCE_MASK)
        {
          m_shadowStatusRegister &= ~AVR32_MCI_SR_DCRCE_MASK;
          return true;
        }
      else
        return false;
    }

    mci::StatusRegister_t
    Mci::sendCommand(mci::CommandWord_t cmdWord, mci::CommandArgument_t cmdArg)
    {
      OSDeviceDebug::putString("mci cmd=");
      OSDeviceDebug::putDec(cmdWord & 0x3F);
      OSDeviceDebug::putString(", cmdWord=");
      OSDeviceDebug::putHex(cmdWord);
      OSDeviceDebug::putString(", arg=");
      OSDeviceDebug::putHex(cmdArg);
      OSDeviceDebug::putNewLine();

      moduleRegisters.writeArgument(cmdArg);
      moduleRegisters.writeCommand(cmdWord);

      while (!wasLastCommandSent())
        ; // TODO: fix loop

      mci::StatusRegister_t ret;
      ret = MCI_SUCCESS;

      // Test error  ==> if crc error and response R3 ==> don't check error
      mci::StatusRegister_t status;
      status = (getStatusRegister() & MCI_SR_ERROR);

      if (status != 0)
        {
          // if the command is SEND_OP_COND the CRC error flag
          // is always present (cf : R3 response)
          if ((cmdWord != mci::CommandWord::SD_MMC_SDCARD_APP_OP_COND_CMD)
              && (cmdWord != mci::CommandWord::SD_MMC_MMC_SEND_OP_COND_CMD))
            {
              if (status != AVR32_MCI_SR_RTOE_MASK)
                // filter RTOE error which happens when using the HS mode
                ret = status;
            }
          else
            {
              if (status != AVR32_MCI_SR_RCRCE_MASK)
                ret = status;
            }
        }

      OSDeviceDebug::putString("mci ret=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

    void
    Mci::setBlockLength(BlockLength_t length)
    {

      uint32_t mode;
      mode = moduleRegisters.readMode(); // Read original mode
        {
          mode &= ~AVR32_MCI_MR_BLKLEN_MASK; // Clear previous BLKLEN
          mode |= (length << AVR32_MCI_MR_BLKLEN_OFFSET); // Set the new value
        }
      moduleRegisters.writeMode(mode); // Write New mode
    }

    void
    Mci::setBlockCount(BlockCount_t cnt)
    {
      union u_blkr
      {
        unsigned long blkr;
        avr32_mci_blkr_t BLKR;
      };
      union u_blkr val;

      val.blkr = moduleRegisters.readBlock();
      val.BLKR.bcnt = cnt;
      moduleRegisters.writeBlock(val.blkr);
    }

    bool
    Mci::isRxReady(void)
    {
      bool isRxReady;
      isRxReady = (getStatusRegister() & AVR32_MCI_SR_RXRDY_MASK) != 0;
      if (!isRxReady)
        OSDeviceDebug::putChar('W');

      return isRxReady;
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_MCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
