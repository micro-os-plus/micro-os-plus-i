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
    Mci::Mci() :
          moduleRegisters(
              *reinterpret_cast<mci::ModuleRegisters*> (mci::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Mci", this);
      OSDeviceDebug::putString("MCI Version=");
      OSDeviceDebug::putHex(moduleRegisters.readVersion());
      OSDeviceDebug::putNewLine();
    }

    void
    Mci::init(Speed_t speed, mci::BusWidth_t busWidth, mci::CardSlot_t cardSlot)
    {
      OSDeviceDebug::putString("avr32::uc3::Mci::init()");
      OSDeviceDebug::putNewLine();

      reset();
      disable();

      // Disable all interrupts
      moduleRegisters.writeInterruptDisable(0xFFFFFFFF);

      // Clear the Configuration register
      moduleRegisters.writeConfiguration(0);

      // Clear the Mode register
      moduleRegisters.writeMode(0);

      // Set the Data Timeout Register to 1 Mega Cycles
      moduleRegisters.writeDataTimeout(MCI_DEFAULT_DTOREG);

      // Set the Mode register
      initSpeed(speed);

      uint32_t mode;
      mode = moduleRegisters.readMode();

      mode |= ((MCI_DEFAULT_PWSDIV << AVR32_MCI_MR_PWSDIV)
          | AVR32_MCI_MR_RDPROOF_MASK | AVR32_MCI_MR_WRPROOF_MASK);

      // Write new configuration
      moduleRegisters.writeMode(mode);

      // Set the SD/MMC Card register
      moduleRegisters.writeSdCard(
          (busWidth >> AVR32_MCI_SDCR_SDCBUS_OFFSET) | (cardSlot
              >> AVR32_MCI_SDCR_SDCSEL_OFFSET));

      // Enable the MCI and the Power Saving
      enable();
    }

    void
    Mci::initSpeed(Speed_t speed)
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
      if (rest)
        {
          // Ensure that the card_speed can not be higher than expected.
          clkdiv += 1;
        }

      if (clkdiv > 0)
        {
          clkdiv -= 1;
        }

      // Fill in new clock config to mode
      mode &= ~AVR32_MCI_MR_CLKDIV_MASK; // Clear previous value
      mode |= clkdiv; // Set the new one

      // Write new configuration
      moduleRegisters.writeMode(mode);
    }

    mci::StatusRegister_t
    Mci::getStatusRegister(void)
    {
      shadowStatusRegister = (shadowStatusRegister & (AVR32_MCI_SR_DTOE_MASK
          | AVR32_MCI_SR_DCRCE_MASK | AVR32_MCI_SR_CSTOE_MASK
          | AVR32_MCI_SR_BLKOVRE_MASK)) | moduleRegisters.readStatus();
      return shadowStatusRegister;
    }

    bool
    Mci::isReady(void)
    {
      return ((getStatusRegister() & AVR32_MCI_SR_CMDRDY_MASK) != 0);
    }

    mci::StatusRegister_t
    Mci::sendCommand(mci::Command_t cmd, mci::CommandArgument_t arg)
    {
      OSDeviceDebug::putString("MCI cmd=");
      OSDeviceDebug::putHex(cmd);
      OSDeviceDebug::putString(", arg=");
      OSDeviceDebug::putHex(arg);
      OSDeviceDebug::putNewLine();

      moduleRegisters.writeArgument(arg);
      moduleRegisters.writeCommand(cmd);

      while (!isReady())
        ; // TODO: fix loop

      mci::StatusRegister_t error;
      mci::StatusRegister_t ret;

      ret = MCI_SUCCESS;

      // Test error  ==> if crc error and response R3 ==> don't check error
      error = getStatusRegister() & MCI_SR_ERROR;
      if (error != 0)
        {
          // if the command is SEND_OP_COND the CRC error flag is always present (cf : R3 response)
          if ((cmd != SD_MMC_SDCARD_APP_OP_COND_CMD) && (cmd
              != SD_MMC_MMC_SEND_OP_COND_CMD))
            {
              if (error != AVR32_MCI_SR_RTOE_MASK)
                // filter RTOE error which happens when using the HS mode
                ret = error;
            }
          else
            {
              if (error != AVR32_MCI_SR_RCRCE_MASK)
                ret = error;
            }
        }

      OSDeviceDebug::putString("MCI ret=");
      OSDeviceDebug::putHex(ret);
      OSDeviceDebug::putNewLine();

      return ret;
    }

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_MCI) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
