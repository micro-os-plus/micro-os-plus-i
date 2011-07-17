/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceMemoryCard.h"

OSDeviceMemoryCard::OSDeviceMemoryCard(Implementation& impl) :
  m_implementation(impl)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceMemoryCard"), this);
}

OSDeviceMemoryCard::~OSDeviceMemoryCard()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceMemoryCard"), this);
}

#if defined(DEBUG)
OSDeviceMemoryCard::Implementation::Implementation()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceMemoryCard::Implementation"),
      this);
}

OSDeviceMemoryCard::Implementation::~Implementation()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceMemoryCard::Implementation"),
      this);
}
#endif /* defined(DEBUG) */

OSReturn_t
OSDeviceMemoryCard::open(void)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::open()");
  OSDeviceDebug::putNewLine();

  m_implementation.init();

  // Default card is not known.
  m_cardType = UNKNOWN_CARD;

  OSReturn_t ret;

  //-- (CMD0)
  // Set card in idle state
  ret = m_implementation.sendCommand(CommandCode::GO_IDLE_STATE, 0xFFFFFFFF);
  if (ret != OSReturn::OS_OK)
    return ret;

  uint32_t u32_response;

  step1:
  // (CMD1)
  // To send its Operating Conditions Register contents command only supported by MMC card
  if (m_implementation.sendCommand(CommandCode::SEND_OP_COND,
      OCR_MSK_BUSY | OCR_MSK_VOLTAGE_ALL | OCR_MSK_HC) == OSReturn::OS_OK)
    {
      // MMC cards always respond to MMC_SEND_OP_COND
      m_cardType = MMC_CARD;
      u32_response = m_implementation.readResponse();
      if (!(u32_response & OCR_MSK_BUSY))
        {
          // here card busy, it did not completed its initialization process
          // resend command MMC_SEND_OP_COND
          goto step1;
          // loop until it is ready
        }
      if (0 != (u32_response & OCR_MSK_HC))
        {
          m_cardType |= MMC_CARD_HC;
        }
    }
  else
    {
      // SD cards do not respond to MMC_SEND_OP_COND
      m_cardType = SD_CARD;

      //-- (CMD8)
      // enables to expand new functionality to some existing commands supported only by SD HC card
      if (m_implementation.sendCommand(CommandCode::SEND_IF_COND, 0x000001AA)
          == OSReturn::OS_OK)
        {
          // It is a SD HC
          if (0x000001AA == m_implementation.readResponse())
            {
              m_cardType |= SD_CARD_V2;
            }
        }

      step2:
      //-- (CMD55)
      // Indicates to the card that the next command is an application specific command rather than a standard command
      // CMD55 shall always precede ACMD41
      ret = m_implementation.sendCommand(CommandCode::APP_CMD, 0);
      if (ret != OSReturn::OS_OK)
        return ret;

      //-- (ACMD41)
      // Sends host OCR register
      if (SD_CARD_V2 & m_cardType)
        {
          // Sends host capacity support information (HCS)
          ret = m_implementation.sendCommand(
              ApplicationCommandCode::SD_SEND_OP_COND,
              OCR_MSK_BUSY | OCR_MSK_VOLTAGE_3_2V_3_3V | OCR_MSK_HC);
          if (ret != OSReturn::OS_OK)
            return ret;
        }
      else
        {
          ret = m_implementation.sendCommand(
              ApplicationCommandCode::SD_SEND_OP_COND,
              OCR_MSK_BUSY | OCR_MSK_VOLTAGE_3_2V_3_3V);
          if (ret != OSReturn::OS_OK)
            return ret;
        }
      u32_response = m_implementation.readResponse();

      if (!(u32_response & OCR_MSK_BUSY))
        {
          // Card Busy, resend ACMD41 precede of CMD55
          goto step2;
        }
      // Card read then check HC type
      if (u32_response & OCR_MSK_HC)
        {
          m_cardType |= SD_CARD_HC; // Card SD High Capacity
        }
    }
#if false
  // Here card ready and type (MMC <V4, MMC V4, MMC HC, SD V1, SD V2 HC) detected

  //-- (CMD2)
  // Send CID
  ret = implSendCommand(SD_MMC_ALL_SEND_CID_CMD, 0);
  if (ret != OSReturn::OS_OK)
  return ret;

  //-- (CMD3)
  // Set relative address
  if (MMC_CARD & m_cardType)
    {
      // For MMC card, you send an address to card
      g_u32_card_rca[slot] = RCA_DEFAULT_ADR;
      ret = implSendCommand(SD_MMC_SET_RELATIVE_ADDR_CMD, RCA_DEFAULT_ADR);
      if (ret != OSReturn::OS_OK)
      return ret;
    }
  else
    {
      // For SD  card, you ask an address to card
      ret = implSendCommand(SD_MMC_SET_RELATIVE_ADDR_CMD, RCA_RESERVE_ADR);
      if (ret != OSReturn::OS_OK)
      return ret;
    }
  if (SD_CARD & m_cardType)
    {
      // For SD  card, you receiv address of card
      g_u32_card_rca[slot] = readResponse() & RCA_MSK_ADR;
    }

  //-- (CMD9)
  // Read & analyse CSD register
  ret = getCsd();
  if (ret != OSReturn::OS_OK)
  return ret;

  //-- (CMD7)-R1b
  // select card
  ret = implSendCommand(SD_MMC_SEL_DESEL_CARD_CMD, g_u32_card_rca[slot]);
  if (ret != OSReturn::OS_OK)
  return ret;

  // Wait end of busy
  waitBusySignal();// read busy state on DAT0

  // Get clock by checking the extended CSD register
  if (MMC_CARD_V4 & m_cardType)
    {
      // Get clock (MMC V4) and size (MMC V4 HC) by checking the extended CSD register
      //-- (CMD8)
      ret = sd_mmc_get_ext_csd();
      if (ret != OSReturn::OS_OK)
      return ret;
    }

  // set bus width
  if (SD_CARD & m_cardType)
    {
      // set 4-bit bus for SD Card
      //-- (CMD55)
      ret = implSendCommand(SD_MMC_APP_CMD, g_u32_card_rca[slot]);
      if (ret != OSReturn::OS_OK)
      return ret;

      //-- (CMD6)
      ret = implSendCommand(SD_MMC_SDCARD_SET_BUS_WIDTH_CMD, SD_BUS_4_BIT);
      if (ret != OSReturn::OS_OK)
      return ret;

      g_u8_card_bus_width[slot] = MCI_BUS_SIZE_4_BIT;
      ret = setBusSize(MCI_BUS_SIZE_4_BIT);
      if (ret != OSReturn::OS_OK)
      return ret;
    }
  else // MMC bus width management

    {
      // set 8-bit bus for MMC Card
      if (MMC_CARD_V4 & m_cardType)
        {
          //-- (CMD6)-R1b
          // set 8-bit bus width (appeared from V4.0 specification)
          ret = implSendCommand(
              MMC_SWITCH_CMD,
              ((U32) MMC_SWITCH_WRITE << 24) | ((U32) MMC_SWITCH_BUS_WIDTH
                  << 16) | ((U32) MMC_SWITCH_VAL_8BIT << 8)
              | ((U32) MMC_SWITCH_CMD_SET));
          if (ret != OSReturn::OS_OK)
            {
              return ret;
            }
          // Wait end of busy
          waitBusySignal();// read busy state on DAT0
          g_u8_card_bus_width[slot] = MCI_BUS_SIZE_8_BIT;
          ret = setBusWidth(mci, MCI_BUS_SIZE_8_BIT);
          if (ret != OSReturn::OS_OK)
          return ret;
        }
    }

  if (MMC_CARD_V4 & m_cardType)
    {
      //-- (CMD6)-R1b
      // set high speed interface timing
      ret = implSendCommand(
          MMC_SWITCH_CMD,
          ((U32) MMC_SWITCH_WRITE << 24) | ((U32) MMC_SWITCH_HIGH_SPEED << 16)
          | ((U32) MMC_SWITCH_VAL_HS << 8) | ((U32) MMC_SWITCH_CMD_SET));
      if (ret != OSReturn::OS_OK)
        {
          return ret;
        }
      // Wait end of busy
      waitBusySignal();
    }

  if (SD_CARD_V2 & m_cardType)
    {
      /** \brief Switch func argument definitions */
#define SDMMC_SWITCH_FUNC_MODE_CHECK  (0 << 31)   /**< Check function */
#define SDMMC_SWITCH_FUNC_MODE_SWITCH (1 << 31)   /**< Switch function */
#define SDMMC_SWITCH_FUNC_HIGH_SPEED  (1 << 0)    /**< High Speed */
#define SDMMC_SWITCH_FUNC_G1_KEEP     (0xf << 0)  /**< Group 1 No influence */
#define SDMMC_SWITCH_FUNC_G2_KEEP     (0xf << 4)  /**< Group 2 No influence */
#define SDMMC_SWITCH_FUNC_G3_KEEP     (0xf << 8)  /**< Group 3 No influence */
#define SDMMC_SWITCH_FUNC_G4_KEEP     (0xf << 12) /**< Group 4 No influence */
#define SDMMC_SWITCH_FUNC_G5_KEEP     (0xf << 16) /**< Group 5 No influence */
#define SDMMC_SWITCH_FUNC_G6_KEEP     (0xf << 20) /**< Group 6 No influence */

      setBlockSize(512 / 8); // CMD6 512 bits status
      setBlockCount(1);

      //-- (CMD6)
      // Check if we can enter into the High Speed mode.
      ret = implSendCommand(SD_SWITCH_FUNC,
          SDMMC_SWITCH_FUNC_MODE_CHECK | SDMMC_SWITCH_FUNC_HIGH_SPEED);
      if (ret != OSReturn::OS_OK)
        {
          return ret;
        }
      // Wait end of busy
      waitBusySignal();// read busy state on DAT0

      bool b_hs_supported = false;
        {
          U8 i;
          for (i = 0; i < (512L / 8); i += 4)
            {
              volatile U32 data;
              while (!(mci_rx_ready(mci)))
              ;
              data = mci_rd_data(mci);
              if (i == 16)
                {
                  if (((data >> 24) & 0xf) == 1)
                  b_hs_supported = TRUE;
                  break;
                }
            }
        }

      if (b_hs_supported == FALSE)
      goto sd_mmc_init_step3;

      ret = implSendCommand(
          SD_SWITCH_FUNC,
          SDMMC_SWITCH_FUNC_MODE_SWITCH | SDMMC_SWITCH_FUNC_G6_KEEP
          | SDMMC_SWITCH_FUNC_G5_KEEP | SDMMC_SWITCH_FUNC_G4_KEEP
          | SDMMC_SWITCH_FUNC_G3_KEEP | SDMMC_SWITCH_FUNC_G2_KEEP
          | SDMMC_SWITCH_FUNC_HIGH_SPEED);
      if (ret != OSReturn::OS_OK)
        {
          return ret;
        }
      else
        {
          U8 i;
          for (i = 0; i < (512L / 8); i += 4)
            {
              volatile U32 data;
              while (!(mci_rx_ready(mci)))
              ;
              data = mci_rd_data(mci);
            }
        }

      /* A 8 cycle delay is required after switch command
       * @ 200KHz clock this should be 40 uS, but we use
       * 80 to handle unprecise clock setting.
       */
      cpu_delay_us(80, g_cpu_hz);

      union u_cfg
        {
          unsigned long cfg;
          avr32_mci_cfg_t CFG;
        };
      union u_cfg val;
      val.cfg = mci->cfg;
      val.CFG.hsmode = 1;
      mci->cfg = val.cfg;

      // deselect card
      ret = implSendCommand(SD_MMC_SEL_DESEL_CARD_CMD, 0);
      if (ret != OSReturn::OS_OK)
      return ret;

      // Wait end of busy
      waitBusySignal();// read busy state on DAT0

      //-- (CMD9)
      // Read & analyse CSD register
      ret = getCsd();
      if (ret != OSReturn::OS_OK)
      return ret;

      // select card
      ret = implSendCommand(SD_MMC_SEL_DESEL_CARD_CMD, g_u32_card_rca[slot]);
      if (ret != OSReturn::OS_OK)
      return ret;

      // Wait end of busy
      waitBusySignal();// read busy state on DAT0
    }

  sd_mmc_init_step3:
  // Set clock
  mci_set_speed(mci, g_pbb_hz, g_u16_card_freq[slot] * 1000);

  //-- (CMD13)
  // Check if card is ready, the card must be in TRAN state
  if (sd_mmc_mci_cmd_send_status(slot) != TRUE)
  return FALSE;

  if ((readResponse() & MMC_TRAN_STATE_MSK) != MMC_TRAN_STATE)
  return OSReturn::OS_ERROR;

  //-- (CMD16)
  // Set the card block length to 512B
  ret = sd_mmc_set_block_len(slot, SD_MMC_SECTOR_SIZE);
  if (ret != OSReturn::OS_OK)
  return ret;

  // USB Test Unit Attention requires a state "busy" between "not present" and "ready" state
  // otherwise never report card change
  //sd_mmc_mci_init_done[slot] = TRUE;
  m_isInitialised = true;
#endif

  OSDeviceDebug::putString("card type=");
  OSDeviceDebug::putHex(m_cardType);
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::getCsd(void)
{
  // sd_mmc_mci_get_csd()
  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::sd_mmc_get_ext_csd(void)
{
  return OSReturn::OS_OK;
}

OSDeviceAddressable::Offset_t
OSDeviceMemoryCard::getDeviceSize(void)
{
  return 0; // No device
}

bool
OSDeviceMemoryCard::sd_mmc_mci_cmd_send_status(uint8_t slot  __attribute__((unused)))
{
#if false
  if (slot > MCI_LAST_SLOTS)
  return FALSE;
  // Select Slot card before any other command.
  mci_select_card(mci, slot, g_u8_card_bus_width[slot]);

  if (mci_send_cmd(mci, SD_MMC_SEND_STATUS_CMD, g_u32_card_rca[slot])
      != MCI_SUCCESS)
  return FALSE;
#endif
  return true;
}

bool
OSDeviceMemoryCard::sd_mmc_set_block_len(uint8_t slot  __attribute__((unused)), uint16_t length __attribute__((unused)))
{
#if false
  if (slot > MCI_LAST_SLOTS)
  return FALSE;
  // Select Slot card before any other command.
  mci_select_card(mci, slot, g_u8_card_bus_width[slot]);

  if (mci_send_cmd(mci, SD_MMC_SET_BLOCKLEN_CMD, length) != MCI_SUCCESS)
  return FALSE;

  // check response, card must be in TRAN state
  if ((mci_read_response(mci) & MMC_TRAN_STATE_MSK) != MMC_TRAN_STATE)
  return FALSE;

  mci_set_block_size(mci, length);
  mci_set_block_count(mci, 1);
#endif

  return true;
}

OSReturn_t
OSDeviceMemoryCard::readBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::readBytes()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceMemoryCard::writeBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::writeBytes()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceAddressable::Alignnment_t
OSDeviceMemoryCard::getWriteAlignment(void)
{
  return 512; // Default alignment size, from MMC specs
}

OSReturn_t
OSDeviceMemoryCard::close(void)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::close()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceMemoryCard::erase(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::erase()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceAddressable::Alignnment_t
OSDeviceMemoryCard::getEraseAlignment(void)
{
  return 512; // Default alignment size, from MMC specs
}

OSReturn_t
OSDeviceMemoryCard::eraseEntireDevice(void)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::eraseEntireDevice()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

#endif /* defined(OS_INCLUDE_OSDEVICEMEMORYCARD) */
