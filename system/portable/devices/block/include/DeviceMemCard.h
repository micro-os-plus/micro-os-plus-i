/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMEMCARD_H_
#define DEVICEMEMCARD_H_

#include "portable/devices/block/include/DeviceMemCardSelect.h"
#include "portable/devices/block/include/DeviceMemCardSPI.h"

// MMC & SDC command definitions
#define	MMC_GO_IDLE_STATE		0
#define	MMC_SEND_OP_COND		1
#define SDC_SEND_IF_COND		8
#define	MMC_SEND_CSD			9
#define	MMC_SEND_CID			10
#define	MMC_STOP_TRANSMISSION		12
#define	MMC_SEND_STATUS			13
#define	MMC_SEND_BLOCKLEN		16
#define	MMC_READ_SINGLE_BLOCK		17
//#define	MMC_READ_MULTIPLE_BLOCK		18
//#define	MMC_SEN_BLOCK_COUNT		23
#define	MMC_WRITE_BLOCK			24
//#define	MMC_WRITE_MULTIPLE_BLOCK	25
#define	MMC_PROGRAM_CSD			27
#define	MMC_SET_WRITE_PROT		28
#define	MMC_CLR_WRITE_PROT		29
#define	MMC_SEND_WRITE_PROT		30
#define	MMC_TAG_SECTOR_START		32
#define	MMC_TAG_SECTOR_END		33
#define	MMC_UNTAG_SECTOR		34
#define	MMC_TAG_ERASE_GROUP_START	35
#define	MMC_TAG_ERASE_GROUP_END		36
#define	MMC_UNTAG_ERASE_GROUP		37
#define	MMC_ERASE			38
#define SDC_A_SEND_OP_COND		41

//#define	MMC_LOCK_UNLOCK			42
#define	SDC_APP_CMD			55
//#define	MMC_GEN_CMD			56
#define	SDC_READ_OCR			58
#define	MMC_CRC_ON_OFF			59

// R1 bits definitions
#define MMC_R1_IN_IDLE_STATE		0x01
#define MMC_R1_ERASE_RESET		0x02
#define MMC_R1_ILLEGAL_COMMAND		0x04
#define MMC_R1_CMD_CRC_ERROR		0x08
#define MMC_R1_ERASE_SEQ_ERROR		0x10
#define MMC_R1_ADDDRESS_ERROR		0x20
#define MMC_R1_PARAM_ERROR		0x40

#define	MMC_BSHIFT			9
#define	MMC_FILL			0x00

#define SDC_OCR_CAPACITY		(0x40000000)

#define MEMCARD_IDLE_RETRIES		(200)

#define MEMCARD_OK                      (0)
#define MEMCARD_ERROR_NONE		(0)
#define MEMCARD_ERROR_MISSING		(-1)
#define MEMCARD_ERROR_CMD		(-5)
#define MEMCARD_ERROR_DATA		(-6)
#define MEMCARD_ERROR_DATA_CRC		(-7)
#define MEMCARD_ERROR_UNKNOWN		(-8)

#define MEMCARD_ERROR_GO_IDLE		(-10)
#define MEMCARD_ERROR_SEND_OP_COND	(-11)
#define MEMCARD_ERROR_SET_BLKLEN	(-12)
#define MEMCARD_ERROR_READ_OCR		(-13)
#define MEMCARD_ERROR_SD_SEND_OP_COND	(-14)
#define MEMCARD_ERROR_VOLTAGE		(-15)
#define MEMCARD_ERROR_APP_CMD		(-16)
#define MEMCARD_ERROR_OPENED		(-17)
#define MEMCARD_ERROR_WRITE             (-18)

#define MEMCARD_OPEN_MMC		(1)
#define MEMCARD_OPEN_MMC13		(5)
#define MEMCARD_OPEN_SDC1		(2)
#define MEMCARD_OPEN_SDC2SS		(3)
#define MEMCARD_OPEN_SDC2HC		(4)

#define MEMCARD_CAPABILITIES_MMC	(0x0001)
#define MEMCARD_CAPABILITIES_SDC	(0x0002)
#define MEMCARD_CAPABILITIES_SDC2	(0x0004)
#define MEMCARD_CAPABILITIES_SDC2_HC	(0x0008)
#define MEMCARD_CAPABILITIES_MMC13	(0x0010)

typedef unsigned long memCardAddr_t;

class DeviceMemCard : public DeviceMemCardSelect, public DeviceMemCardSPI
  {
public:
  DeviceMemCard();

  static int open(unsigned short vhs = 0);
  static int close(void);

  static int execCmd(unsigned char cmd, unsigned long arg, unsigned char *pbuf,
      unsigned short len);
  static int setBlkLen(unsigned short blen);
  static int
      readCtrl(unsigned char cmd, unsigned char *pbuf, unsigned short len);
  static int readBuffer(memCardAddr_t addr, unsigned char *pbuf,
      unsigned short len);
  static int writeBuffer(memCardAddr_t addr, unsigned char *pbuf,
      unsigned short len);

  static int tagEraseStart(memCardAddr_t addr);
  static int tagEraseEnd(memCardAddr_t addr);
  static int erase(void);

  static unsigned short getWriteBufferSize(void);

#if defined(OS_INCLUDE_MEMCARD_VALIDATEVOLTAGE)

  static bool validateVoltage( unsigned long l );
#endif

  static int getCapabilities(void);
  static void setWaitCycles(unsigned short n);
  static unsigned short getWaitCycles(void);

  static bool isOpened(void);

  static void waitWhileBusy(void);

private:

  static void safeSelectOn(void);
  static void safeSelectOff(void);

  static int enterIdle(void);

  static void sendCommand(unsigned char cmd, unsigned long arg);
  static unsigned char receiveR1(void);
  static unsigned char receiveDataBlock(void);
  static unsigned long receiveLong(void);

  static unsigned char crc7(unsigned char crc, unsigned char data);
  static unsigned short crc16(unsigned short crc, unsigned char data);

  // member variables
  static unsigned short m_waitCycles;
  static unsigned short m_bufLen;
  static unsigned short m_capabilities;

#if defined(OS_INCLUDE_MEMCARD_MEMBER_OCR)
  static unsigned long m_OCR;
#endif

#if defined(OS_INCLUDE_MEMCARD_MEMBER_VHS)
  static unsigned long m_VHS;
#endif

  static bool m_isOpened;

  };

inline bool DeviceMemCard::isOpened(void)
  {
    return m_isOpened;
  }

// MMC 1.0 CID
typedef struct
__attribute__  ( ( __packed__ ) )
    {
      unsigned long manufacturerID:
      24;
      unsigned char productName[ 7 ];
      unsigned char hwRevision:
      4;
      unsigned char fwRevision:
      4;
      unsigned long serialNumber:
      24;
      unsigned char monthCode:
      4;
      unsigned char yearCode:
      4;
      unsigned char crc7:
      7;
    }
  memCardCID_mmc10_t;

  // MMC 1.3 CID
typedef struct
__attribute__  ( ( __packed__ ) )
    {
      unsigned int manufacturerID:
      8; // [0]
      unsigned int applicationID:
      16; // [1]
      unsigned char productName[ 6 ]; // [3]
      unsigned char productRevision:
      8; // [9]
      unsigned long serialNumber:
      32; // [10]
      unsigned char manufacturingDateCode:
      8; // [14]
      unsigned char crc7:
      7; // [15]
    }
  memCardCID_mmc13_t;

  // SDC CID
typedef struct
__attribute__  ( ( __packed__ ) )
    {
      unsigned int manufacturerID:
      8; // [0]
      unsigned int applicationID:
      16; // [1]
      unsigned char productName[ 5 ]; // [3]
      unsigned char productRevision:
      8; // [8]
      unsigned long serialNumber:
      32; // [9]
      unsigned char reserved:
      4; // [13]
      unsigned char yearCode:
      8;
      unsigned char monthCode:
      4;
      unsigned char crc7:
      7; // [15]
    }
  memCardCID_sdc_t;

#endif /*DEVICEMEMCARD_H_*/
