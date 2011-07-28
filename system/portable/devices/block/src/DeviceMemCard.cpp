/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_MEMCARD)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/DeviceMemCard.h"

/*
 * Memory Card support (MultiMedia & Secure Digital cards).
 * 
 * Historically, part of this code was inspired by code written in 2000 for MMC 1.0,
 * and recently updated based on "Physical Layer Simplified Specs V2.0" from 
 * SD Card Association.
 * 
 * Although with common origins, once capacity was expanded, the various cards 
 * diverged and followed slightly incompatible paths, especially for CID/CSD content
 * (MMV vs SDC, and standard vs high capacity).
 * 
 * The point of the complex initialization logic is to identify the type of the 
 * card, in order to correctly use the CID/CSD info. If the type of the card is 
 * fixed and known, initialization can be hardcoded to a simpler sequence (at the 
 * lower limit CMD0 and CMD1 can do the trick for both MMC and standard size SDC). 
 */

DeviceMemCard::DeviceMemCard()
  {
    OSDeviceDebug::putConstructor_P(PSTR("DeviceMemCard"), this);

    // default, compute as 10 * (TAAC+NSAC) and set it later
    m_waitCycles = 10000;

    m_isOpened = false;
  }

// ----- static data ---------------------------------------------------------
unsigned short DeviceMemCard::m_waitCycles;
unsigned short DeviceMemCard::m_bufLen;
unsigned short DeviceMemCard::m_capabilities;

#if defined(OS_INCLUDE_MEMCARD_MEMBER_OCR)
unsigned long DeviceMemCard::m_OCR;
#endif

#if defined(OS_INCLUDE_MEMCARD_MEMBER_VHS)
unsigned long DeviceMemCard::m_VHS;
#endif

bool DeviceMemCard::m_isOpened;
// ---------------------------------------------------------------------------

/*
 * IMPORTANT NOTICE: 
 * As per MultiMediaCard Product Manual, chapter 5.4, "After the last MMC bus
 * transaction, the host is required to provide 8 (eight) clock cycles for the card 
 * to complete the operation before shutting down the clock.
 * 
 * This can be done either before or after turning the chip select off. From practical 
 * experience, I encountered cards that do not work without the extra clock after 
 * turning the chip select off, so I preffer to play safe and add extra cycles both
 * before and after turning the chip select off.
 * 
 * Equally important are the clocks required at power-up BEFORE the chip select,
 * so be sure your hardware directly connects the clock signal to the card,
 * regardless the status of the chip select.
 */

void DeviceMemCard::safeSelectOn(void)
  {
    implSelectEnable();
    spiRead();
  }

void DeviceMemCard::safeSelectOff(void)
  {
    spiRead();
    implSelectDisable();
    spiRead();
  }

int DeviceMemCard::getCapabilities(void)
  {
    return m_capabilities;
  }

void DeviceMemCard::setWaitCycles(unsigned short n)
  {
    m_waitCycles = n;
  }

unsigned short DeviceMemCard::getWaitCycles(void)
  {
    return m_waitCycles;
  }

/*
 * initialise card.
 * logic from SD Simplified, page 95, with extentions
 * for SanDisk MMCs 1.3 that accept CMD55
 */
int DeviceMemCard::open(unsigned short vhs)
  {
    int i;
    unsigned char r1;
    int res;

    if (m_isOpened)
    return MEMCARD_ERROR_OPENED;

    m_bufLen = 0;
    m_capabilities = 0;

    implSelectInit();

    spiInitLowSpeed();

    // send more than 80 clocks prior to starting bus communication
    // before slecting the card
    for (i = 10; i != 0; --i)
    spiRead();

    // 1 mS for just in case, not from the specs
    OS::busyWaitMillis(1);
    res = 0;

    safeSelectOn();

    res = enterIdle();
    if (res != 0)
      {
        OSDeviceDebug::putString("idle=");
        OSDeviceDebug::putHex((unsigned short)res);
        OSDeviceDebug::putNewLine();

        goto out;
      }

    // now in idle state

    unsigned long l;
    unsigned char icmd;
    int ires;

    icmd = 0;
    ires = 0;

    sendCommand(SDC_SEND_IF_COND, (unsigned long) vhs);
    r1 = receiveR1();
    l = receiveLong();

    if ((r1 & ~MMC_R1_IN_IDLE_STATE) == 0)
      {
        // no error, v2.0 SD Card or later
#if defined(OS_INCLUDE_MEMCARD_MEMBER_VHS)
        m_VHS = l; // store to member
#endif

        OSDeviceDebug::putString("SD Card v2.0 or later");
        OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_MEMCARD_VALIDATEVOLTAGE)

        if (!validateVoltage(l))
          {
            res = MEMCARD_ERROR_VOLTAGE;
            goto out;
          }
#endif

        // card with compatible voltage range

        sendCommand(SDC_READ_OCR, 0L);
        r1 = receiveR1();
        l = receiveLong();

        if ((r1 & ~MMC_R1_IN_IDLE_STATE) == 0)
          {
            // no error, v2.0 SD

            OSDeviceDebug::putString("OCR=");
            OSDeviceDebug::putHex((unsigned short) (l >> 16));
            OSDeviceDebug::putHex((unsigned short) l);
            OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_MEMCARD_OCR)

            m_OCR = l;
#endif

            m_capabilities |= MEMCARD_CAPABILITIES_SDC;
            m_capabilities |= MEMCARD_CAPABILITIES_SDC2;

            if ((l & SDC_OCR_CAPACITY) != 0)
              {
                OSDeviceDebug::putString("SD Card v2.0 HC");
                OSDeviceDebug::putNewLine();

                m_capabilities |= MEMCARD_CAPABILITIES_SDC2_HC;
                res = MEMCARD_OPEN_SDC2HC;
              }
            else
              {
                OSDeviceDebug::putString("SD Card v2.0 SS");
                OSDeviceDebug::putNewLine();

                res = MEMCARD_OPEN_SDC2SS;
              }
            icmd = SDC_A_SEND_OP_COND;
            ires = MEMCARD_ERROR_SD_SEND_OP_COND;
          }
        else
          {
            res = MEMCARD_ERROR_READ_OCR;
            goto out;
          }
      }
    else if ((r1 & MMC_R1_ILLEGAL_COMMAND) != 0)
      {
        OSDeviceDebug::putString("SD Card v1.x or MMC");
        OSDeviceDebug::putNewLine();

        sendCommand(SDC_READ_OCR, 0L);
        r1 = receiveR1();
        l = receiveLong();

        if ((r1 & ~MMC_R1_IN_IDLE_STATE) == 0)
          {
            OSDeviceDebug::putString("SD Card v1.x or MMC v1.3");
            OSDeviceDebug::putNewLine();

            OSDeviceDebug::putString("OCR=");
            OSDeviceDebug::putHex((unsigned short) (l >> 16));
            OSDeviceDebug::putHex((unsigned short) l);
            OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_MEMCARD_OCR)

            m_OCR = l;
#endif

            for (i = MEMCARD_IDLE_RETRIES; i > 0; --i)
              {
                sendCommand(SDC_APP_CMD, 0L);
                r1 = receiveR1();
                if ((r1 & ~MMC_R1_IN_IDLE_STATE) != 0)
                  {
                    // illegal CMD55, must be MMC
                    OSDeviceDebug::putString("MM1.3 Card a");
                    OSDeviceDebug::putNewLine();

                    m_capabilities |= MEMCARD_CAPABILITIES_MMC13;
                    res = MEMCARD_OPEN_MMC13;
                    goto exit_idle_mmc;
                  }

                sendCommand(SDC_A_SEND_OP_COND, 0L);

                r1 = receiveR1();
                if (r1 == 0)
                break;

                if ((r1 & ~MMC_R1_IN_IDLE_STATE) != 0)
                  {
                    // illegal ACMD41, must be MMC
                    // SanDisk MMCs come here
                    OSDeviceDebug::putString("MM1.3 Card b");
                    OSDeviceDebug::putNewLine();

                    m_capabilities |= MEMCARD_CAPABILITIES_MMC13;
                    res = MEMCARD_OPEN_MMC13;
                    goto exit_idle_mmc;
                  }
                if ((i % 10) == 0)
                  {
                    OS::busyWaitMillis(1);
                  }

                OSDeviceDebug::putString("CMD R1=");
                OSDeviceDebug::putHex(r1);
                OSDeviceDebug::putNewLine();
              }

            OSDeviceDebug::putString("SD Card v1.x");
            OSDeviceDebug::putNewLine();

            // OK, we have an old SD card
            m_capabilities |= MEMCARD_CAPABILITIES_SDC;
            res = MEMCARD_OPEN_SDC1;
            goto out;
          }
        else if ((r1 & MMC_R1_ILLEGAL_COMMAND) != 0)
          {
            // illegal CMD58, must be old MMC
            OSDeviceDebug::putString("MM Card");
            OSDeviceDebug::putNewLine();

            // OK, we have an old MMC card
            res = MEMCARD_OPEN_MMC;
            goto exit_idle_mmc;
          }
        else
          {
            res = MEMCARD_ERROR_READ_OCR;
            goto out;
          }
      }
    goto exit_idle;

    exit_idle_mmc: m_capabilities |= MEMCARD_CAPABILITIES_MMC;

    icmd = MMC_SEND_OP_COND;
    ires = MEMCARD_ERROR_SEND_OP_COND;

    res = enterIdle();
    if (res != 0)
      {
        OSDeviceDebug::putString("idle=");
        OSDeviceDebug::putHex((unsigned short)res);
        OSDeviceDebug::putNewLine();

        goto out;
      }

    exit_idle: OSDeviceDebug::putString("MemCard activate ");
    OSDeviceDebug::putDec(icmd);
    OSDeviceDebug::putNewLine();

    for (i = MEMCARD_IDLE_RETRIES; i > 0; --i)
      {
        if (icmd == SDC_A_SEND_OP_COND)
          {
            sendCommand(SDC_APP_CMD, 0L);
            r1 = receiveR1();
            if ((r1 & ~MMC_R1_IN_IDLE_STATE) != 0)
              {
                OSDeviceDebug::putHex(r1);

                res = MEMCARD_ERROR_APP_CMD; // CMD1 error
                goto out;
              }
          }
        sendCommand(icmd, 0L);

        r1 = receiveR1();
        if (r1 == 0)
        break;

        if ((r1 & ~MMC_R1_IN_IDLE_STATE) != 0)
          {
            res = ires; // CMD1 error
            goto out;
          }
        if ((i % 10) == 0)
          {
            OS::busyWaitMillis(1);
          }

        OSDeviceDebug::putString("CMD");
        OSDeviceDebug::putDec(icmd);
        OSDeviceDebug::putString(" R1=");
        OSDeviceDebug::putHex(r1);
        OSDeviceDebug::putNewLine();
      }

    out: safeSelectOff();

#ifdef DEBUG_MMCIO
    OSDeviceDebug::putNewLine();
#endif /* DEBUG_MMCIO */

    // once the card is opened, set high speed
    spiInit();

    if (res > 0)
      {
        OSDeviceDebug::putString("MemCard opened");
        OSDeviceDebug::putNewLine();

        m_isOpened = true;
      }
    return res;
  }

/*
 * enter idle status
 */
int DeviceMemCard::enterIdle(void)
  {
    int i;
    int j;
    unsigned char r1;

    // ...the host must poll the card until 'in-idle-state'
    for (i = MEMCARD_IDLE_RETRIES, j = 0; i > 0; --i)
      {
        spiRead(); // extra
        sendCommand(MMC_GO_IDLE_STATE, 0L);
        r1 = receiveR1();

        if (r1 == MMC_R1_IN_IDLE_STATE)
        break; // no errors & idle state detected

        if ((i % 10) == 0)
          {
            OS::busyWaitMillis(1);
          }

        if (r1 == 0xFF)
        ++j;

        OSDeviceDebug::putString("CMD0 R1=");
        OSDeviceDebug::putHex(r1);
        OSDeviceDebug::putNewLine();
      }
    if (i == 0)
      {
        if (j == MEMCARD_IDLE_RETRIES)
        return MEMCARD_ERROR_MISSING;
        else
        return MEMCARD_ERROR_GO_IDLE;
      }

    // now in idle state
    OSDeviceDebug::putString("MemCard idle");
    OSDeviceDebug::putNewLine();

    return 0;
  }

#if false
// template to be used for user code
bool
DeviceMemCard::validateVoltage(unsigned long l)
  {
    l = l;
    return true;
  }
#endif

int DeviceMemCard::close(void)
  {
    m_isOpened = false;

    return 0;
  }

/*
 * read control structures CID/CSD
 */
int DeviceMemCard::readCtrl(unsigned char cmd, unsigned char*  pbuf,
    unsigned short len)
  {
    return execCmd(cmd, 0L, pbuf, len);
  }

int DeviceMemCard::readBuffer(memCardAddr_t addr, unsigned char*  pbuf,
    unsigned short len)
  {
    return execCmd(MMC_READ_SINGLE_BLOCK, addr, pbuf, len);
  }

int DeviceMemCard::writeBuffer(memCardAddr_t addr, unsigned char*  pbuf,
    unsigned short len)
  {
    return execCmd(MMC_WRITE_BLOCK, addr, pbuf, len);
  }

int DeviceMemCard::tagEraseStart(memCardAddr_t addr)
  {
    return execCmd(MMC_TAG_SECTOR_START, addr, 0, 0);
  }

int DeviceMemCard::tagEraseEnd(memCardAddr_t addr)
  {
    return execCmd(MMC_TAG_SECTOR_END, addr, 0, 0);
  }

int DeviceMemCard::erase(void)
  {
    return execCmd(MMC_ERASE, 0L, 0, 0);
  }

/*
 * execute command
 * cmd = MMC_SEND_CSD, MMC_SEND_CID, MMC_READ_SINGLE_BLOCK, 
 * MMC_WRITE_BLOCK
 */
int DeviceMemCard::execCmd(unsigned char cmd, unsigned long arg,
    unsigned char* pbuf, unsigned short len)
  {
    OSDeviceDebug::putString(" cmd=");
    OSDeviceDebug::putDec(cmd);
    OSDeviceDebug::putChar(' ');

    int res;
    res = 0;

    unsigned short maxlen;

    if (cmd == MMC_SEND_CSD || cmd == MMC_SEND_CID)
    maxlen = 16;
    else if (cmd == MMC_READ_SINGLE_BLOCK)
    maxlen = m_bufLen;
    else if (cmd == MMC_WRITE_BLOCK)
      {
        maxlen = getWriteBufferSize();
        if (len > maxlen)
        len = maxlen;
      }
    else if (cmd == MMC_TAG_SECTOR_START || cmd == MMC_TAG_SECTOR_END || cmd
        == MMC_ERASE)
      {
        maxlen = 0;
      }
    else
      {
        OSDeviceDebug::putString("unknown command");
        OSDeviceDebug::putNewLine();
        res = MEMCARD_ERROR_UNKNOWN; // unsupported command
        return res;
      }

    safeSelectOn();

    // wait previous write to complete
    int i;
    for (i = 1; spiRead() != 0xFF; i++)
      {
        if ((i % 100) == 0)
          {
            OSDeviceDebug::putChar('y');
            OSScheduler::yield();
          }
      }

    unsigned char r;

    sendCommand(cmd, arg);
    r = receiveR1();

    if (r != 0)
      {
        OSDeviceDebug::putString("cmd r1=");
        OSDeviceDebug::putHex(r);
        OSDeviceDebug::putNewLine();
        res = MEMCARD_ERROR_CMD;
        goto out;
      }

    if (cmd == MMC_ERASE)
      {
        int i;
        for (i = 0; spiRead() == 0x00; i++)
          {
            if ((i % 100) == 0)
              {
                OSDeviceDebug::putChar('y');
                OSScheduler::yield();
              }
          }

        goto out;
      }

    if (cmd != MMC_SEND_CSD && cmd != MMC_SEND_CID)
      {
        while (spiRead() != 0xFF)
        ; // Nac >= 1
      }

    if (cmd == MMC_TAG_SECTOR_START || cmd == MMC_TAG_SECTOR_END)
    goto out;

    if (cmd == MMC_WRITE_BLOCK)
      {
        if (false)
          {
            OSDeviceDebug::putString(" addr=");
            OSDeviceDebug::putHex((unsigned short)(arg >> 16));
            OSDeviceDebug::putHex((unsigned short)(arg));
            OSDeviceDebug::putChar(' ');
          }

        spiReadWrite(0xFF); // Nwr min = 1

        spiReadWrite(0xFE); // start block

        unsigned short crc;
        crc = 0;

        unsigned char* p;
        unsigned int i;
        for (i = 0, p = pbuf; i < maxlen; ++i, ++p)
          {
            unsigned char r;
            if (i < len)
            r = *p;
            else
            r = 0xFF;
            spiReadWrite(r);
            crc = crc16(crc, r);
          }

        spiReadWrite(crc >> 8);
        spiReadWrite(crc & 0xFF);

        do
          {
            r = spiRead();
          }while ((r & 0x11) != 0x1);

        r &= 0x1F;
        if (r != ((0x02 << 1) | 0x1))
          {
            if (r == ((0x05 << 1) | 0x1))
            res = MEMCARD_ERROR_DATA_CRC;
            else if (r == ((0x06 << 1) | 0x1))
            res = MEMCARD_ERROR_WRITE;
            else
            res = MEMCARD_ERROR_UNKNOWN;

            OSDeviceDebug::putString("wr r=");
            OSDeviceDebug::putHex(r);
            OSDeviceDebug::putNewLine();
            goto out;
          }

        while (spiRead() != 0x00)
        ; // wait for first busy response

        // not mandatory, but safer
        for (i = 8; (((r = spiRead())) == 0x00) && (i != 0); --i)
        ;

      }
    else
      {
        r = receiveDataBlock();
        if (r != 0xFE)
          {
            OSDeviceDebug::putString("db r=");
            OSDeviceDebug::putHex(r);
            OSDeviceDebug::putNewLine();

            res = MEMCARD_ERROR_DATA;
            goto out;
          }

        unsigned short crc;
        crc = 0;

        unsigned char* p;
        unsigned int i;
        for (i = 0, p = pbuf; i < maxlen + 2; ++i, ++p) // +2 crc

          {
            r = spiRead();
            if (i < len)
              {
                *p = r;
              }
            crc = crc16(crc, r);
          }

        if (crc != 0)
          {
            OSDeviceDebug::putString("crc");
            OSDeviceDebug::putNewLine();

            res = MEMCARD_ERROR_DATA_CRC;
            goto out;
          }
      }
    out: safeSelectOff();

#ifdef DEBUG_MMCIO
    OSDeviceDebug::putNewLine();
#endif /* DEBUG_MMCIO */

    return res;
  }

/*
 *  set block length
 */
int DeviceMemCard::setBlkLen(unsigned short blen)
  {
    int res;
    unsigned char r;

    res = 0;

    if (blen != m_bufLen)
      {
        OSDeviceDebug::putString(" sblk=");
        OSDeviceDebug::putDec(blen);
        OSDeviceDebug::putChar(' ');

        safeSelectOn();

        sendCommand(MMC_SEND_BLOCKLEN, (unsigned long)blen);

        r = receiveR1();
        if (r != 0)
          {
            OSDeviceDebug::putString("sblk r1=");
            OSDeviceDebug::putHex(r);
            OSDeviceDebug::putNewLine();

            res = MEMCARD_ERROR_SET_BLKLEN;
          }

        safeSelectOff();

        m_bufLen = blen;
      }

    return res;
  }

/*
 * Polynomial: x^7 + x^3 + 1 (0x42)
 * Initial value: 0x0
 */

unsigned char DeviceMemCard::crc7(unsigned char crc, unsigned char b)
  {
    int i;

    for (i = 0; i < 8; ++i)
      {
        crc <<= 1;
        if (((b << i) ^ crc) & 0x80)
        crc ^= 0x09;
      }
    return crc;
  }

/*
 * Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
 * Initial value: 0x0
 *
 *  This is the CRC used by the Xmodem-CRC protocol.
 */

unsigned short DeviceMemCard::crc16(unsigned short crc, unsigned char b)
  {
    int i;

    crc = crc ^ ((unsigned short) b << 8);
    for (i = 0; i < 8; i++)
      {
        if (crc & 0x8000)
        crc = (crc << 1) ^ 0x1021;
        else
        crc <<= 1;
      }

    return crc;
  }

// ---------------------------------------------------------------------------

void DeviceMemCard::sendCommand(unsigned char cmd, unsigned long arg)
  {
    // to simplify separation of consecutive commands
    spiRead();

    unsigned char a3, a2, a1, a0;

    a0 = arg;
    arg >>= 8;
    a1 = arg;
    arg >>= 8;
    a2 = arg;
    arg >>= 8;
    a3 = arg;

    unsigned char crc;
    crc = 0;

    unsigned char c0;
    c0 = (cmd & 0x3F) | 0x40; // start bits
    crc = crc7(crc, c0);
    spiReadWrite(c0);

    crc = crc7(crc, a3);
    spiReadWrite(a3);
    crc = crc7(crc, a2);
    spiReadWrite(a2);
    crc = crc7(crc, a1);
    spiReadWrite(a1);
    crc = crc7(crc, a0);
    spiReadWrite(a0);

    crc <<= 1;
    crc |= 0x01; // stop bit

    spiReadWrite(crc);

    if (true)
    while (spiRead() != 0xFF)
    ; // Ncr >= 1, at least one H after command
  }

unsigned char DeviceMemCard::receiveR1(void)
  {
    unsigned char r;
    int i;

    // Ncr in [1..8]
    // Get first response byte with bit7 = 0
    for (i = 8 + 1; (((r = spiRead()) & 0x80) == 0x80) && (i != 0); --i)
      {
        ;
      }

    // although Nac >= 1, Ncx may be 0, so we do not 
    // hunt for FFs here.

    return r;
  }

unsigned long DeviceMemCard::receiveLong(void)
  {
    unsigned long l;
    int i;
    unsigned char c;

    l = 0;
    for (i = 4; i > 0; --i)
      {
        c = spiRead(); // read R7 extended part
        l <<= 8;
        l |= c;
      }

    return l;
  }

unsigned char DeviceMemCard::receiveDataBlock(void)
  {
    unsigned char r;
    int i;
    int j;

    for (i = m_waitCycles, j = 0; ((r = spiRead()) != 0xFE) && (i != 0); --i,
        ++j)
      {
        // 0xE0/0x1F are from MMC manual
        if (((r & 0xE0) == 0x00) && ((r & 0x1F) != 0x00))
          {
            return r; // Data Error Token
          }

        if (j == 100)
          {
            // yield from time to time
            OSScheduler::yield();
            j = 0;
          }
      }

    return r;
  }

unsigned short DeviceMemCard::getWriteBufferSize(void)
  {
    return 512; // TODO: get from card structures
  }

void DeviceMemCard::waitNotBusy(void)
  {
    safeSelectOn();

    // wait previous write to complete
    int i;
    for (i = 1; spiRead() != 0xFF; i++)
      {
        if ((i % 100) == 0)
          {
            OSDeviceDebug::putChar('y');
            OSScheduler::yield();
          }
      }

    safeSelectOff();
  }

#endif
