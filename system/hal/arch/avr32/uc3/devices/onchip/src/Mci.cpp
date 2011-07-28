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
    Mci::initialise(mci::CardSlot_t cardSlot)
    {
      OSDeviceDebug::putString("avr32::uc3::Mci::init()");
      OSDeviceDebug::putNewLine();

      m_shadowStatusRegister = 0;

      softwareReset();
      disableInterface();

      disableAllInterrupts();

      clearConfiguration();
      clearModeBits();

      // Set the Data Timeout Register to 1 Mega Cycles
      initDataTimeout(MCI_DEFAULT_DTOLMUL, MCI_DEFAULT_DTOLCYC);

      // Start with low speed
      configureClockFrequencyHz(400000);

      // Set the Mode register
      // The two bits, Read Proof Enable and Write Proof Enable in the
      // MR register (MR.RDPROOF and MR.WRPROOF) allow stopping the
      // MCI Clock (CLK) during read or write access if the internal
      // FIFO is full. This will guarantee data integrity, not bandwidth
      enableModeBits(
          (MCI_DEFAULT_PWSDIV << AVR32_MCI_MR_PWSDIV)
              | AVR32_MCI_MR_RDPROOF_MASK | AVR32_MCI_MR_WRPROOF_MASK);

      // Set the SD/MMC Card register
      // Start with bus width = 1bit
      configureSdCardBusWidthAndSlot(mci::BusWidth::_1bit, cardSlot);

      // Enable the MCI and the Power Saving
      // The Power Save Mode Enable bit in the CR register (CR.PWSEN)
      // saves power by dividing the MCI clock (CLK) by 2PWSDIV + 1
      // when the bus is inactive. The Power Saving Divider field locates
      // in the Mode Register (MR.PWSDIV).
      enableInterface();
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
    Mci::configureClockFrequencyHz(ClockFrequencyHz_t frequency)
    {
      uint32_t mode;
      mode = moduleRegisters.readMode();

      if (frequency > AVR32_MCI_HSDIS_MAX_FREQ)
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
      clkdiv = getInputClockFrequencyHz() / (frequency * 2);
      rest = getInputClockFrequencyHz() % (frequency * 2);
      if (rest != 0)
        {
          // Ensure that the card_speed can not be higher than expected.
          clkdiv += 1;
        }

      // The Multimedia Card Interface Clock (CLK) is CLK_MCI
      // divided by (2*(CLKDIV+1)).

      if (clkdiv > 0)
        {
          clkdiv -= 1;
        }

      OSDeviceDebug::putString("Mci freq=");
      OSDeviceDebug::putDec(frequency);
      OSDeviceDebug::putString("Hz, clkdev=");
      OSDeviceDebug::putHex(clkdiv);
      OSDeviceDebug::putNewLine();

      // Fill in the new clock divider to mode
      mode &= ~AVR32_MCI_MR_CLKDIV_MASK; // Clear previous clkdiv value
      mode |= clkdiv; // Set the new one

      // Write new configuration
      moduleRegisters.writeMode(mode);
    }

    void
    Mci::configureBusWidth(mci::BusWidth_t busWidth)
    {
      uint32_t sdReg;

      sdReg = moduleRegisters.readSdCard();

      // Clear previous buswidth
      sdReg &= ~AVR32_MCI_SDCR_SDCBUS_MASK;
      // Include new busWidth
      sdReg |= ((busWidth & 0x3) << AVR32_MCI_SDCR_SDCBUS_OFFSET);

      moduleRegisters.writeSdCard(sdReg);
    }

    void
    Mci::configureSdCardBusWidthAndSlot(mci::BusWidth_t busWidth,
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

      // Preserve only the 4 given bits
      m_shadowStatusRegister &= (AVR32_MCI_SR_DTOE_MASK
          | AVR32_MCI_SR_DCRCE_MASK | AVR32_MCI_SR_CSTOE_MASK
          | AVR32_MCI_SR_BLKOVRE_MASK);

      // Include new status bits
      m_shadowStatusRegister |= moduleRegisters.readStatus();

      return m_shadowStatusRegister;
    }

    bool
    Mci::isBusy(void)
    {
      return ((getStatusRegister() & AVR32_MCI_SR_NOTBUSY_MASK) == 0);
    }

    bool
    Mci::isCommandReady(void)
    {
      return ((getStatusRegister() & AVR32_MCI_SR_CMDRDY_MASK) != 0);
    }

    bool
    Mci::isCrcError(void)
    {
      if (getStatusRegister() & AVR32_MCI_SR_DCRCE_MASK)
        {
          // Clear the shadow bit
          m_shadowStatusRegister &= ~AVR32_MCI_SR_DCRCE_MASK;
          return true;
        }
      else
        return false;
    }

    mci::StatusRegister_t
    Mci::sendCommand(mci::CommandWord_t cmdWord, mci::CommandArgument_t cmdArg)
    {
      OSDeviceDebug::putString("Mci cmd=");
      OSDeviceDebug::putDec(cmdWord & 0x3F);
      OSDeviceDebug::putString(", cmdWord=");
      OSDeviceDebug::putHex(cmdWord);
      OSDeviceDebug::putString(", arg=");
      OSDeviceDebug::putHex(cmdArg);
      OSDeviceDebug::putNewLine();

      moduleRegisters.writeArgument(cmdArg);
      moduleRegisters.writeCommand(cmdWord);

      while (!isCommandReady())
        ; // TODO: reset WD

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

      OSDeviceDebug::putString("Mci ret=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

    void
    Mci::configureBlockLengthBytes(BlockLength_t length)
    {
      uint32_t mode;
      mode = moduleRegisters.readMode(); // Read original mode
        {
          mode &= ~AVR32_MCI_MR_BLKLEN_MASK; // Clear previous BLKLEN
          mode |= ((length & 0xFFFF) << AVR32_MCI_MR_BLKLEN_OFFSET); // Set the new value
        }
      moduleRegisters.writeMode(mode); // Write New mode
    }

    void
    Mci::configureBlockCount(BlockCount_t cnt)
    {
      uint32_t value;
      value = moduleRegisters.readBlock(); // Read original mode
        {
          value &= ~AVR32_MCI_BLKR_BCNT_MASK; // Clear previous BCNT
          value |= ((cnt & 0xFFFF) << AVR32_MCI_BCNT_OFFSET); // Set the new value
        }
      moduleRegisters.writeBlock(value); // Write New mode
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

    uint32_t
    Mci::readData(void)
    {
      uint32_t ret;

      ret = moduleRegisters.readReceiveData();

      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putChar(' ');

      return ret;
    }

    bool
    Mci::isTxReady(void)
    {
      bool isTxReady;
      isTxReady = (getStatusRegister() & AVR32_MCI_SR_TXRDY_MASK) != 0;
      if (!isTxReady)
        OSDeviceDebug::putChar('W');

      return isTxReady;
    }

    void
    Mci::writeData(uint32_t value)
    {
      OSDeviceDebug::putHex(value);
      OSDeviceDebug::putChar(' ');

      moduleRegisters.writeTransmitData(value);
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_MCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
