/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceMemoryCard.h"

#include "hal/arch/avr32/lib/include/compiler.h"

// ----- Constructors & Destructors -------------------------------------------

OSDeviceMemoryCard::OSDeviceMemoryCard(Implementation& impl) :
  m_implementation(impl)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceMemoryCard"), this);

  m_isOpened = false;
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

// ----- Public virtual methods -----------------------------------------------

OSReturn_t
OSDeviceMemoryCard::open(void)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::open()");
  OSDeviceDebug::putNewLine();

  if (m_isOpened)
    return OSReturn::OS_ALREADY_OPENED;

  OSReturn_t ret;
  ret = initialise();

  m_isOpened = true;
  return ret;
}

OSReturn_t
OSDeviceMemoryCard::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t count)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::readBlocks() ");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(count);
  OSDeviceDebug::putNewLine();

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  if (blockNumber >= m_cardSizeBlocks)
    return OSReturn::OS_SIZE_EXCEEDED;

  if (count == 1)
    {
      ret = prepareReadBlock(blockNumber);
      if (ret != OSReturn::OS_OK)
        return ret;

      transferReadBlocks(pBuf, 1);

      ret = finaliseReadBlock();
    }
  else
    {
      ret = prepareReadBlocks(blockNumber, count);
      if (ret != OSReturn::OS_OK)
        return ret;

      transferReadBlocks(pBuf, count);

      ret = finaliseReadBlocks();
    }
  return ret;
}

OSReturn_t
OSDeviceMemoryCard::writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t count)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::writeBlocks() ");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(count);
  OSDeviceDebug::putNewLine();

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  if (blockNumber >= m_cardSizeBlocks)
    return OSReturn::OS_SIZE_EXCEEDED;

  if (count == 1)
    {
      ret = prepareWriteBlock(blockNumber);
      if (ret != OSReturn::OS_OK)
        return ret;

      transferWriteBlocks(pBuf, 1);

      ret = finaliseWriteBlock();
    }
  else
    {
      ret = prepareWriteBlocks(blockNumber, count);
      if (ret != OSReturn::OS_OK)
        return ret;

      transferReadBlocks(pBuf, count);

      ret = finaliseWriteBlocks();
    }
  return ret;
}

OSReturn_t
OSDeviceMemoryCard::close(void)
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::close()");
  OSDeviceDebug::putNewLine();

  m_isOpened = false;

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceMemoryCard::eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString("OSDeviceMemoryCard::erase()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceBlock::BlockNumber_t
OSDeviceMemoryCard::getDeviceSize(void)
{
  return m_cardSizeBlocks;
}

// Return the device block size, in bytes.
OSDeviceBlock::BlockSize_t
OSDeviceMemoryCard::getBlockSize(void)
{
  return 512;
}

// Allow the upper layer implementation to customise inits.

bool
OSDeviceMemoryCard::Implementation::isUsingSpi(void)
{
  return false;
}

// ----- Private methods ------------------------------------------------------

void
OSDeviceMemoryCard::waitNotBusy(void)
{
  while (m_implementation.isBusy())
    OSScheduler::yield();
}

OSReturn_t
OSDeviceMemoryCard::initialise(void)
{

  m_implementation.initialise();

  // Default card is not known.
  m_cardType = UNKNOWN_CARD;
  m_cardSizeBlocks = 0;
  m_implementation.configureBusWidth(BusWidth::_1);

  OSReturn_t ret;

  // -- (CMD0)
  // Set card in idle state
  ret = m_implementation.sendCommand(CommandCode::GO_IDLE_STATE, 0xFFFFFFFF);
  if (ret != OSReturn::OS_OK)
    return ret;

  uint32_t u32_response;

  step1:
  // -- (CMD1)
  // To send its Operating Conditions Register contents command only supported by MMC card
  if (m_implementation.sendCommand(CommandCode::SEND_OP_COND,
      OCR_MSK_BUSY | OCR_MSK_VOLTAGE_ALL | OCR_MSK_HC) == OSReturn::OS_OK)
    {
      // MMC cards always respond to MMC_SEND_OP_COND
      m_cardType = MMC_CARD;
      OSDeviceDebug::putString("MMC_CARD ");
      OSDeviceDebug::putNewLine();

      u32_response = m_implementation.readResponse();
      if (!(u32_response & OCR_MSK_BUSY))
        {
          // here card busy, it did not completed its initialisation process
          // resend command MMC_SEND_OP_COND
          goto step1;
          // loop until it is ready
        }
      if (0 != (u32_response & OCR_MSK_HC))
        {
          m_cardType |= MMC_CARD_HC;
          OSDeviceDebug::putString("MMC_CARD_HC ");
          OSDeviceDebug::putNewLine();
        }
    }
  else
    {
      // SD cards do not respond to MMC_SEND_OP_COND
      m_cardType = SD_CARD;
      OSDeviceDebug::putString("SD_CARD ");
      OSDeviceDebug::putNewLine();

      // -- (CMD8)
      // enables to expand new functionality to some existing commands supported only by SD HC card
      if (m_implementation.sendCommand(CommandCode::SEND_IF_COND, 0x000001AA)
          == OSReturn::OS_OK)
        {
          // It is a SD HC
          if (0x000001AA == m_implementation.readResponse())
            {
              m_cardType |= SD_CARD_V2;
              OSDeviceDebug::putString("SD_CARD_V2 ");
              OSDeviceDebug::putNewLine();
            }
        }

      step2:
      // -- (CMD55)
      // Indicates to the card that the next command is an application specific command rather than a standard command
      // CMD55 shall always precede ACMD41
      ret = m_implementation.sendCommand(CommandCode::APP_CMD, 0);
      if (ret != OSReturn::OS_OK)
        return ret;

      // -- (ACMD41)
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
          // Card Busy, resend ACMD41 preceded by CMD55
          goto step2;
        }
      // Card read then check HC type
      if (u32_response & OCR_MSK_HC)
        {
          m_cardType |= SD_CARD_HC; // Card SD High Capacity
          OSDeviceDebug::putString("SD_CARD_HC ");
          OSDeviceDebug::putNewLine();
        }
    }
  // Here card ready and type (MMC <V4, MMC V4, MMC HC, SD V1, SD V2 HC) detected

  // -- (CMD2)
  // Send CID
  ret = m_implementation.sendCommand(CommandCode::ALL_SEND_CID, 0);
  if (ret != OSReturn::OS_OK)
    return ret;

  // -- (CMD3)
  // Set relative address
  if (MMC_CARD & m_cardType)
    {
      // For MMC card, you send an address to card
      m_cardRca = RCA_DEFAULT_ADR;
      ret = m_implementation.sendCommand(CommandCode::SEND_RELATIVE_ADDR,
          RCA_DEFAULT_ADR);
      if (ret != OSReturn::OS_OK)
        return ret;
    }
  else
    {
      // For SD  card, you ask an address to card
      ret = m_implementation.sendCommand(CommandCode::SEND_RELATIVE_ADDR,
          RCA_RESERVE_ADR);
      if (ret != OSReturn::OS_OK)
        return ret;

      // For SD card, you receive the address of the card
      m_cardRca = m_implementation.readResponse() & RCA_MSK_ADR;
    }

  // -- (CMD9)
  // Read & analyse CSD register
  ret = getCsd();
  if (ret != OSReturn::OS_OK)
    return ret;

#if true
  // Enter Data Transfer Mode (tran)

  // -- (CMD7)-R1b
  // select card
  ret = m_implementation.sendCommand(CommandCode::SELECT_DESELECT_CARD,
      m_cardRca);
  if (ret != OSReturn::OS_OK)
    return ret;

  m_implementation.readResponse();

  // Wait end of busy
  waitNotBusy();// read busy state on DAT0
#endif

  // Get clock by checking the extended CSD register
  if (MMC_CARD_V4 & m_cardType)
    {
      // Get clock (MMC V4) and size (MMC V4 HC) by checking the extended CSD register
      //-- (CMD8)
      ret = getExtendedCsd();
      if (ret != OSReturn::OS_OK)
        return ret;
    }

  //#define OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_MULTIBIT             (1)

#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_MULTIBIT)
  // set bus width to multiple bits
  if (SD_CARD & m_cardType)
    {
      // set 4-bit bus for SD Card

      //-- (CMD55)
      ret = m_implementation.sendCommand(CommandCode::APP_CMD, m_cardRca);
      if (ret != OSReturn::OS_OK)
      return ret;

      // -- (ACMD6)
      ret = m_implementation.sendCommand(ApplicationCommandCode::SET_BUS_WIDTH,
          BusWidth::_4);
      if (ret != OSReturn::OS_OK)
      return ret;

      m_cardBusWidth = BusWidth::_4;
      ret = m_implementation.configureBusWidth(BusWidth::_4);
      if (ret != OSReturn::OS_OK)
      return ret;

      OSDeviceDebug::putString("SD bw=4");
      OSDeviceDebug::putNewLine();
    }
  else
    {
      // set 8-bit bus for MMC Card

      // TODO: configure to allow only 4 bit

      if (MMC_CARD_V4 & m_cardType)
        {
          // -- (CMD6)-R1b
          // set 8-bit bus width (appeared from V4.0 specification)
          ret = m_implementation.sendCommand(
              SwitchCommandCode::MMC_SWITCH_FUNC,
              ((uint32_t) MMC_SWITCH_WRITE << 24)
              | ((uint32_t) MMC_SWITCH_BUS_WIDTH << 16)
              | ((uint32_t) MMC_SWITCH_VAL_8BIT << 8)
              | ((uint32_t) MMC_SWITCH_CMD_SET));
          if (ret != OSReturn::OS_OK)
          return ret;

          // Wait end of busy
          waitNotBusy();// read busy state on DAT0

          m_cardBusWidth = BusWidth::_8;
          ret = m_implementation.configureBusWidth(BusWidth::_8);
          if (ret != OSReturn::OS_OK)
          return ret;
        }
    }
#endif

#define OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_HIGHSPEED            (1)

#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_HIGHSPEED)
  if (MMC_CARD_V4 & m_cardType)
    {
      // -- (CMD6)-R1b
      // set high speed interface timing
      ret = m_implementation.sendCommand(
          SwitchCommandCode::MMC_SWITCH_FUNC,
          ((uint32_t) MMC_SWITCH_WRITE << 24)
              | ((uint32_t) MMC_SWITCH_HIGH_SPEED << 16)
              | ((uint32_t) MMC_SWITCH_VAL_HS << 8)
              | ((uint32_t) MMC_SWITCH_CMD_SET));
      if (ret != OSReturn::OS_OK)
        return ret;

      // Wait end of busy
      waitNotBusy();
    }
#endif

  if (SD_CARD_V2 & m_cardType)
    {
#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_HIGHSPEED)
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

      m_implementation.configureBlockLengthBytes(512 / 8); // CMD6 512 bits status
      m_implementation.configureBlockCount(1);
      // -- (CMD6)
      // Check if we can enter into the High Speed mode.
      ret = m_implementation.sendCommand(SwitchCommandCode::SD_SWITCH_FUNC,
          SDMMC_SWITCH_FUNC_MODE_CHECK | SDMMC_SWITCH_FUNC_HIGH_SPEED);
      if (ret != OSReturn::OS_OK)
        return ret;

      // Wait end of busy
      waitNotBusy();// read busy state on DAT0

      bool b_hs_supported = false;

      uint8_t i;
      for (i = 0; i < (512 / 8); i += 4)
        {
          uint32_t data;
          while (!(m_implementation.isRxReady()))
            {
              ;
            }
          data = m_implementation.readData();
          if (i == 16)
            {
              if (((data >> 24) & 0xf) == 1)
                b_hs_supported = true;
              //break;
            }
        }

      if (!b_hs_supported)
        {
          OSDeviceDebug::putString("HS not supported");
          OSDeviceDebug::putNewLine();

          goto step3;
        }

      ret = m_implementation.sendCommand(
          SwitchCommandCode::SD_SWITCH_FUNC,
          SDMMC_SWITCH_FUNC_MODE_SWITCH | SDMMC_SWITCH_FUNC_G6_KEEP
              | SDMMC_SWITCH_FUNC_G5_KEEP | SDMMC_SWITCH_FUNC_G4_KEEP
              | SDMMC_SWITCH_FUNC_G3_KEEP | SDMMC_SWITCH_FUNC_G2_KEEP
              | SDMMC_SWITCH_FUNC_HIGH_SPEED);
      if (ret != OSReturn::OS_OK)
        return ret;

      for (i = 0; i < (512 / 8); i += 4)
        {
          uint32_t data;
          while (!(m_implementation.isRxReady()))
            ;
          data = m_implementation.readData();
        }

      /* A 8 cycle delay is required after switch command
       * @ 200KHz clock this should be 40 uS, but we use
       * 80 to handle unprecise clock setting.
       */
      //cpu_delay_us(80, g_cpu_hz);
      OS::busyWaitMicros(80);
      m_implementation.setHighSpeedMode();

      // deselect card
      ret = m_implementation.sendCommand(CommandCode::SELECT_DESELECT_CARD, 0);
      if (ret != OSReturn::OS_OK)
        return ret;

      // Wait end of busy
      waitNotBusy();// read busy state on DAT0
#endif

#if false
      // -- (CMD9)
      // Read & analyse CSD register
      ret = getCsd();
      if (ret != OSReturn::OS_OK)
      return ret;

      // select card
      ret = m_implementation.sendCommand(CommandCode::SELECT_DESELECT_CARD,
          m_cardRca);
      if (ret != OSReturn::OS_OK)
      return ret;

      // Wait end of busy
      m_implementation.waitNotBusy();// read busy state on DAT0
#endif
    }

  step3:

#define OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_CARDFREQUENCY        (1)

#if defined(OS_INCLUDE_OSDEVICEMEMORYCARD_INIT_CARDFREQUENCY)
  // Set clock
  m_implementation.configureClockFrequencyHz(m_cardFrequencyKHz * 1000);
#endif

#if true
  // -- (CMD13)
  // Check if card is ready, the card must be in TRAN state
  ret = sendStatus();
  if (ret != OSReturn::OS_OK)
    return ret;

  if ((m_implementation.readResponse() & MMC_TRAN_STATE_MSK) != MMC_TRAN_STATE)
    return OSReturn::OS_BAD_STATE;

  OSDeviceDebug::putString("tran state");
  OSDeviceDebug::putNewLine();

#endif

  // -- (CMD16)
  // Set the card block length to 512B
  ret = setBlockLength(SD_MMC_SECTOR_SIZE);
  if (ret != OSReturn::OS_OK)
    return ret;

  // USB Test Unit Attention requires a state "busy" between "not present" and "ready" state
  // otherwise never report card change

  // sd_mmc_mci_init_done[slot] = TRUE;
  m_isInitialised = true;

  OSDeviceDebug::putString("card type=");
  OSDeviceDebug::putHex(m_cardType);
  OSDeviceDebug::putNewLine();

  m_isOpened = true;

  //ret = m_implementation.sendCommand(CommandCode::SEND_STATUS, m_cardRca);
  //m_implementation.readResponse();

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::getCsd(void)
{
  csd_t csd; // Structure allocated on stack!

  uint32_t max_Read_DataBlock_Length;
  uint32_t mult;
  uint32_t blocknr;
  uint8_t tmp;
  const uint16_t freq_unit[4] =
    { 10, 100, 1000, 10000 };
  const uint8_t mult_fact[16] =
    { 0, 10, 12, 13, 15, 20, 26, 30, 35, 40, 45, 52, 55, 60, 70, 80 }; // MMC tabs...

  // Select Slot card before any other command.
  m_implementation.selectCard();

  //-- (CMD9)
  if (m_implementation.sendCommand(CommandCode::SEND_CSD, m_cardRca)
      != OSReturn::OS_OK)
    return OSReturn::OS_ERROR;

  csd.csd[0] = m_implementation.readResponse();
  csd.csd[1] = m_implementation.readResponse();
  csd.csd[2] = m_implementation.readResponse();
  csd.csd[3] = m_implementation.readResponse();

  //-- Read "System specification version", only available on MMC card
  // field: SPEC_VERS (only on MMC)
  if (MMC_CARD & m_cardType) // TO BE ADDED
    {
      if (CSD_SPEC_VER_4_0 == (MSB0(csd.csd[0]) & CSD_MSK_SPEC_VER))
        {
          m_cardType |= MMC_CARD_V4;
          OSDeviceDebug::putString("MMC_CARD_V4 ");
          OSDeviceDebug::putNewLine();
        }
    }

  //-- Compute MMC/SD speed
  // field: TRAN_SPEED (CSD V1 & V2 are the same)
  m_cardFrequencyKHz = mult_fact[csd.csd_v1.tranSpeed >> 3]; // Get Multiplier factor
  if (SD_CARD & m_cardType)
    {
      // SD card don't have same frequency that MMC card
      if (26 == m_cardFrequencyKHz)
        {
          m_cardFrequencyKHz = 25;
        }
      else if (52 == m_cardFrequencyKHz)
        {
          m_cardFrequencyKHz = 50;
        }
    }
  m_cardFrequencyKHz *= freq_unit[csd.csd_v1.tranSpeed & 0x07]; // Get transfer rate unit

  //-- Compute card size in number of block
  // field: WRITE_BL_LEN, READ_BL_LEN, C_SIZE (CSD V1 & V2 are not the same)
  if (SD_CARD_HC & m_cardType)
    {
      m_cardSizeBlocks = (csd.csd_v2.deviceSizeH << 16)
          + (csd.csd_v2.deviceSizeL & 0xFFff);

      // memory capacity = (C_SIZE+1) * 1K sector
      m_cardSizeBlocks = (m_cardSizeBlocks + 1) << 10; // unit 512B
    }
  else
    {
      // Check block size
      tmp = csd.csd_v1.writeBlLen; // WRITE_BL_LEN
      if (tmp < CSD_BLEN_512)
        return OSReturn::OS_TOO_SHORT; // block size < 512B not supported by firmware

      tmp = csd.csd_v1.readBlLen; // READ_BL_LEN
      if (tmp < CSD_BLEN_512)
        return OSReturn::OS_TOO_SHORT; // block size < 512B not supported by firmware

      //// Compute Memory Capacity
      // compute MULT
      mult = 1 << (csd.csd_v1.cSizeMult + 2);
      max_Read_DataBlock_Length = 1 << csd.csd_v1.readBlLen;
      // compute MSB of C_SIZE
      blocknr = csd.csd_v1.deviceSizeH << 2;
      // compute MULT * (LSB of C-SIZE + MSB already computed + 1) = BLOCKNR
      blocknr = mult * (blocknr + csd.csd_v1.deviceSizeL + 1);
      m_cardSizeBlocks = ((max_Read_DataBlock_Length * blocknr) / 512);
    }

  OSDeviceDebug::putString("getCsd() size=");
  OSDeviceDebug::putDec(m_cardSizeBlocks * 512 / 1024 / 1024);
  OSDeviceDebug::putString(" MB");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::getExtendedCsd(void)
{
  uint8_t i;
  uint32_t val;

  // Select Slot card before any other command.
  m_implementation.selectCard();

  m_implementation.configureBlockLengthBytes(SD_MMC_SECTOR_SIZE); // Ext CSD = 512B size
  m_implementation.configureBlockCount(1);

  //** (CMD8)
  // read the extended CSD
  if (m_implementation.sendCommand(ApplicationCommandCode::SEND_EXT_CSD, 0)
      != OSReturn::OS_OK)
    return OSReturn::OS_ERROR;

  // READ_EXT_CSD   // discard bytes not used
  for (i = (512L / 8); i != 0; i--)
    {
      while (!(m_implementation.isRxReady()))
        ;
      m_implementation.readData();
      while (!(m_implementation.isRxReady()))
        ;
      if (((64 - 26) == i) && (m_cardType & MMC_CARD_HC))
        {
          // If MMC HC then read Sector Count
          m_cardSizeBlocks = m_implementation.readData();
        }
      else
        {
          val = m_implementation.readData();
          if ((64 - 24) == i)
            { // Read byte at offset 196
              if (MSB0(val) & 0x02)
                m_cardFrequencyKHz = 52 * 1000;
              else
                m_cardFrequencyKHz = 26 * 1000;
            }
        }
    }

  return TRUE;

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::sendStatus()
{
  // Select Slot card before any other command.
  m_implementation.selectCard();

  OSReturn_t ret;
  // -- (CMD13)
  ret = m_implementation.sendCommand(CommandCode::SEND_STATUS, m_cardRca);
  if (ret != OSReturn::OS_OK)
    return ret;

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::setBlockLength(uint16_t length)
{
  // Select Slot card before any other command.
  m_implementation.selectCard();

  OSReturn_t ret;
  // -- (CMD16)
  ret = m_implementation.sendCommand(CommandCode::SET_BLOCKLEN, length);
  if (ret != OSReturn::OS_OK)
    return ret;

  // check response, card must be in TRAN state
  if ((m_implementation.readResponse() & MMC_TRAN_STATE_MSK) != MMC_TRAN_STATE)
    return OSReturn::OS_BAD_STATE;

  m_implementation.configureBlockLengthBytes(length);
  m_implementation.configureBlockCount(1);

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::prepareReadBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    OSDeviceBlock::BlockCount_t count)
{
  //OSDeviceDebug::putString("1 ");

  // Select Slot card before any other command.
  m_implementation.selectCard();
  //OSDeviceDebug::putString("2 ");

  // wait for MMC not busy
  waitNotBusy();
  //OSDeviceDebug::putString("3 ");

  uint32_t addr;
  addr = blockNumber;
  // send command
  if ((!(SD_CARD_HC & m_cardType)) && (!(MMC_CARD_HC & m_cardType)))
    {
      addr <<= 9; // For NO HC card the address must be translate in byte address
    }

  OSReturn_t ret;

  // -- (CMD13)
  // Necessary to clear flag error "ADDRESS_OUT_OF_RANGE" (ID LABO = MMC15)
  ret = m_implementation.sendCommand(CommandCode::SEND_STATUS, m_cardRca);
  if (ret != OSReturn::OS_OK)
    return ret;

  m_implementation.readResponse();

  m_implementation.configureBlockLengthBytes(SD_MMC_SECTOR_SIZE);
  m_implementation.configureBlockCount(count);

  // -- (CMD18)
  ret = m_implementation.sendCommand(CommandCode::READ_MULTIPLE_BLOCK, addr);
  if (ret != OSReturn::OS_OK)
    return ret;

  // check response
  if ((m_implementation.readResponse() & CardStatus::FLAGERROR_RD_WR) != 0)
    return OSReturn::OS_ERROR;

  OSDeviceDebug::putString("read open ok");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::prepareReadBlock(OSDeviceBlock::BlockNumber_t blockNumber)
{
  //OSDeviceDebug::putString("1 ");

  // Select Slot card before any other command.
  m_implementation.selectCard();
  //OSDeviceDebug::putString("2 ");

  // wait for MMC not busy
  waitNotBusy();
  OSDeviceDebug::putString("3 ");

  uint32_t addr;
  addr = blockNumber;

  // For NO HC card the address must be translate to a byte address
  if ((!(SD_CARD_HC & m_cardType)) && (!(MMC_CARD_HC & m_cardType)))
    {
      addr <<= 9;
    }

  OSReturn_t ret;

  m_implementation.configureBlockLengthBytes(SD_MMC_SECTOR_SIZE);
  m_implementation.configureBlockCount(1); // might not be needed

  // -- (CMD17)
  ret = m_implementation.sendCommand(CommandCode::READ_SINGLE_BLOCK, addr);
  if (ret != OSReturn::OS_OK)
    return ret;

  // check response
  if ((m_implementation.readResponse() & CardStatus::FLAGERROR_RD_WR) != 0)
    return OSReturn::OS_ERROR;

  OSDeviceDebug::putString("read open ok");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

void
OSDeviceMemoryCard::transferReadBlocks(void *pBuf,
    OSDeviceBlock::BlockCount_t count)
{
  uint32_t wordsToRead;
  // Warning: buffer must be word aligned
  uint32_t* pLong = (uint32_t*) pBuf;

  // Read data
  wordsToRead = (SD_MMC_SECTOR_SIZE / sizeof(*pLong));

  OSDeviceDebug::putString("ptr=");
  OSDeviceDebug::putPtr(pLong);
  OSDeviceDebug::putString(", words=");
  OSDeviceDebug::putDec(wordsToRead);
  OSDeviceDebug::putNewLine();

  while (count-- > 0)
    {
      wordsToRead = (SD_MMC_SECTOR_SIZE / sizeof(*pLong));

      while (wordsToRead > 0)
        {
          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation. readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          while (!(m_implementation.isRxReady()))
            ;
          *pLong++ = m_implementation.readData();

          wordsToRead -= 8;
        }
    }
  OSDeviceDebug::putString("done");
  OSDeviceDebug::putNewLine();
}

OSReturn_t
OSDeviceMemoryCard::finaliseReadBlocks(void)
{
  if (m_implementation.isCrcError())
    {
      return OSReturn::OS_BAD_CHECKSUM; // An CRC error has been seen
    }

#if false
  m_implementation.waitNotBusy();

  OSReturn_t ret;

  ret
  = m_implementation.sendCommand(CommandCode::STOP_TRANSMISSION, 0xffffffff);
  if (ret != OSReturn::OS_OK)
  return ret;
#endif

  /*
   if( (mci_overrun_error(mci)) )
   {
   return FALSE;
   }

   if( (mci_underrun_error(mci)) )
   {
   return FALSE;
   }*/

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::finaliseReadBlock(void)
{
  if (m_implementation.isCrcError())
    {
      return OSReturn::OS_BAD_CHECKSUM; // An CRC error has been seen
    }

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::prepareWriteBlock(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)))
{
  //uint32_t addr;

  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::prepareWriteBlocks(
    OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)), OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  //uint32_t addr;

  return OSReturn::OS_ERROR;
}

void
OSDeviceMemoryCard::transferWriteBlocks(const void *pBuf,
    OSDeviceBlock::BlockCount_t count)
{
  uint32_t wordsToWrite;
  const uint32_t *pLong = (const uint32_t *) pBuf;

  // Write Data
  while (count-- > 0)
    {
      wordsToWrite = (SD_MMC_SECTOR_SIZE / sizeof(*pLong));

      while (wordsToWrite > 0)
        {
          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          while (!m_implementation.isTxReady())
            ;
          m_implementation.writeData(*pLong++);

          wordsToWrite -= 8;
        }
    }
}

OSReturn_t
OSDeviceMemoryCard::finaliseWriteBlock(void)
{
  return OSReturn::OS_OK;
}

OSReturn_t
OSDeviceMemoryCard::finaliseWriteBlocks(void)
{
  return OSReturn::OS_ERROR;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSDEVICEMEMORYCARD) */
