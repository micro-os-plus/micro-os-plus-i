/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEMEMORYCARD_H_
#define OSDEVICEMEMORYCARD_H_

#include "portable/kernel/include/OS.h"

#include "OSDeviceBlock.h"

//! Number of bits for addresses within sectors.
#define SD_MMC_SECTOR_BITS     9

//! Sector size in bytes.
#define SD_MMC_SECTOR_SIZE     (1 << SD_MMC_SECTOR_BITS)

//! Card identification
#define UNKNOWN_CARD                   0x00
#define MMC_CARD                       0x01
#define SD_CARD                        0x02
#define MMC_CARD_V4                    0x04
#define SD_CARD_V2                     0x08
#define SD_CARD_HC                     0x10
#define MMC_CARD_HC                    0x20

//! OCR register
#define OCR_MSK_BUSY              0x80000000 // Busy flag
#define OCR_MSK_HC                0x40000000 // High Capacity flag
#define OCR_MSK_VOLTAGE_3_2V_3_3V 0x00100000 // Voltage 3.2V to 3.3V flag
#define OCR_MSK_VOLTAGE_ALL       0x00FF8000 // All Voltage flag
//! RCA register
#define RCA_RESERVE_ADR           0x00000000
#define RCA_MSK_ADR               0xFFFF0000
#define RCA_DEFAULT_ADR           0x0001FFFF // It can be changed
//! CSD register
#define CSD_REG_SIZE              0x10
#define CSD_STRUCT_1_0            0x00
#define CSD_STRUCT_1_1            0x40
#define CSD_STRUCT_1_2            0x80
#define CSD_STRUCT_SUP            0xC0
#define CSD_MSK_STRUCT            0xC0
#define CSD_SPEC_VER_1_0          0x00
#define CSD_SPEC_VER_1_4          0x04
#define CSD_SPEC_VER_2_0          0x08
#define CSD_SPEC_VER_3_1          0x0C
#define CSD_SPEC_VER_4_0          0x10
#define CSD_MSK_SPEC_VER          0x3C
#define CSD_MSK_RBP               0x80
#define CSD_MSK_WBM               0x40
#define CSD_MSK_RBM               0x20
#define CSD_MSK_RBL               0x0F
#define CSD_MSK_CSH               0x03
#define CSD_MSK_CSL               0xC0
#define CSD_MSK_CSM               0x07
#define CSD_MSK_CSMH              0x03
#define CSD_MSK_CSML              0x80
#define CSD_MSK_WBL               0x03
#define CSD_MSK_WBH               0xC0
#define CSD_MSK_WBP               0x20
#define CSD_BLEN_2048             11
#define CSD_BLEN_512              9

//! MMC Status Mask
#define MMC_STBY_STATE_MSK        ((U32)0x01D81E00)
#define MMC_STBY_STATE            ((U32)0x00000600)  // stby state
#define MMC_DATA_STATE_MSK        ((U32)0xE0040E00)
#define MMC_DATA_STATE            ((U32)0x00000A00)  // data state
#define MMC_RCV_STATE_MSK         ((U32)0xE0020E00)
#define MMC_RCV_STATE             ((U32)0x00000A00)  // rcv state
#define MMC_TRAN_STATE_MSK        ((U32)0xE0020E00)
#define MMC_TRAN_STATE            ((U32)0x00000800)  // tran state
// R1 Response bit-defines
#define MMC_R1_BUSY                       0x80  ///< R1 response: bit indicates card is busy
#define MMC_R1_PARAMETER                  0x40
#define MMC_R1_ADDRESS                    0x20
#define MMC_R1_ERASE_SEQ                  0x10
#define MMC_R1_COM_CRC                    0x08
#define MMC_R1_ILLEGAL_COM                0x04
#define MMC_R1_ERASE_RESET                0x02
#define MMC_R1_IDLE_STATE                 0x01

// Data Start tokens
#define MMC_STARTBLOCK_READ               0xFE  ///< when received from card, indicates that a block of data will follow
#define MMC_STARTBLOCK_WRITE              0xFE  ///< when sent to card, indicates that a block of data will follow
#define MMC_STARTBLOCK_MWRITE             0xFC

// Data Stop tokens
#define MMC_STOPTRAN_WRITE                0xFD

// Data Error Token values
#define MMC_DE_MASK                       0x1F
#define MMC_DE_ERROR                      0x01
#define MMC_DE_CC_ERROR                   0x02
#define MMC_DE_ECC_FAIL                   0x04
#define MMC_DE_OUT_OF_RANGE               0x04
#define MMC_DE_CARD_LOCKED                0x04

// Data Response Token values
#define MMC_DR_MASK                       0x1F
#define MMC_DR_ACCEPT                     0x05
#define MMC_DR_REJECT_CRC                 0x0B
#define MMC_DR_REJECT_WRITE_ERROR         0x0D

// Arguments of MMC_SWITCH command
#define MMC_SWITCH_WRITE          ((U8)03)
#define MMC_SWITCH_BUS_WIDTH      ((U8)183)
#define MMC_SWITCH_HIGH_SPEED     ((U8)185)
#define MMC_SWITCH_CMD_SET        ((U8)03)
#define MMC_SWITCH_VAL_LS         ((U8)00)
#define MMC_SWITCH_VAL_HS         ((U8)01)
#define MMC_SWITCH_VAL_1BIT       ((U8)00)
#define MMC_SWITCH_VAL_4BIT       ((U8)01)
#define MMC_SWITCH_VAL_8BIT       ((U8)02)

// Arguments of MMC_LOCK_UNLOCK command
#define CMD_FULL_ERASE           0x08
#define CMD_UNLOCK               0x00
#define CMD_CLEAR                0x02
#define CMD_LOCK                 0x01

/*_____ D E C L A R A T I O N ______________________________________________*/

typedef struct
{
  union
  {
    unsigned int cardStatus;
    struct
    {
      unsigned int outOfRange :1;
      unsigned int addressserror :1;
      unsigned int blockLenError :1;
      unsigned int eraseSeqErro :1;
      unsigned int eraseParam :1;
      unsigned int wpViolation :1;
      unsigned int cardIsLocked :1;
      unsigned int lockUnlockFailed :1;
      unsigned int comCrcError :1;
      unsigned int illegalCommand :1;
      unsigned int cardEccFailed :1;
      unsigned int ccError :1;
      unsigned int error :1;
      unsigned int :2;
      unsigned int csdOverwrite :1;
      unsigned int wpEraseSkip :1;
      unsigned int cardEccDisabled :1;
      unsigned int eraseReset :1;
      unsigned int currentState :4;
      unsigned int readyForData :1;
      unsigned int :2;
      unsigned int appCmd :1;
      unsigned int :1;
      unsigned int akeSeqError :1;
      unsigned int :3;
    };
  };
} card_status_t;

typedef struct
{
  union
  {
    unsigned int ocr;
    struct
    {
      unsigned int busy :1;
      unsigned int ccs :1;
      unsigned int :6;
      unsigned int vRange :9;
      unsigned int :15;
    };
  };
} ocr_t;

typedef struct
{
  //CDS[0]
  unsigned int csdStructure :2;
  unsigned int :6;
  unsigned int taac :8;
  unsigned int nsac :8;
  unsigned int tranSpeed :8;
  //CSD[1]
  unsigned int ccc :12;
  unsigned int readBlLen :4;
  unsigned int readBlePartial :1;
  unsigned int writeBlkMisalign :1;
  unsigned int readBlkMisalign :1;
  unsigned int dsrImp :1;
  unsigned int :2;
  unsigned int deviceSizeH :10;
  //CSD[2]
  unsigned int deviceSizeL :2;
  unsigned int vddRCurrMin :3;
  unsigned int vddRCurrMax :3;
  unsigned int vddWCurrMin :3;
  unsigned int vddWCurrMax :3;
  unsigned int cSizeMult :3;
  unsigned int eraseBlkEn :1;
  unsigned int sectorSize :7;
  unsigned int wpGrpSize :7;
  //CSD[3]
  unsigned int wpGrpEnable :1;
  unsigned int :2;
  unsigned int r2wFactor :3;
  unsigned int writeBlLen :4;
  unsigned int writeBlPartial :1;
  unsigned int :5;
  unsigned int fileFormatGrp :1;
  unsigned int copy :1;
  unsigned int permWriteProtect :1;
  unsigned int tmpWriteProtect :1;
  unsigned int fileFormat :2;
  unsigned int :2;
  unsigned int crc :7;
  unsigned int :1;
} csd_v1_t;

typedef struct
{
  //CDS[0]
  unsigned int csdStructure :2;
  unsigned int :6;
  unsigned int taac :8;
  unsigned int nsac :8;
  unsigned int tranSpeed :8;
  //CSD[1]
  unsigned int ccc :12;
  unsigned int readBlLen :4;
  unsigned int readBlePartial :1;
  unsigned int writeBlkMisalign :1;
  unsigned int readBlkMisalign :1;
  unsigned int dsrImp :1;
  unsigned int :6;
  unsigned int deviceSizeH :6;
  //CSD[2]
  unsigned int deviceSizeL :16;
  unsigned int :1;
  unsigned int eraseBlkEn :1;
  unsigned int sectorSize :7;
  unsigned int wpGrpSize :7;
  //CSD[3]
  unsigned int wpGrpEnable :1;
  unsigned int :2;
  unsigned int r2wFactor :3;
  unsigned int writeBlLen :4;
  unsigned int writeBlPartial :1;
  unsigned int :5;
  unsigned int fileFormatGrp :1;
  unsigned int copy :1;
  unsigned int permWriteProtect :1;
  unsigned int tmpWriteProtect :1;
  unsigned int fileFormat :2;
  unsigned int :2;
  unsigned int crc :7;
  unsigned int :1;
} csd_v2_t;

typedef struct
{
  union
  {
    unsigned int csd[4];
    union
    {
      csd_v1_t csd_v1;
      csd_v2_t csd_v2;
    };
  };
} csd_t;

typedef struct
{
  union
  {
    unsigned int cid[4];
    struct
    {
      unsigned int mid :8; // Manufacturer ID
      unsigned int oid :16; // OEM/Application ID
      unsigned int pnmh :32; // Product name high bits
      unsigned int pnml :8; // Product name low bits
      unsigned int prv :8; // Product revision
      unsigned int psn :32; // Product serial number
      unsigned int :4;
      unsigned int mdt :12; // Manufacturing date
      unsigned int crc :7; // CRC7 Checksum
      unsigned int :1;
    };
  };
} cid_t;

typedef struct
{
  union
  {
    unsigned int rep1[2];
    struct
    {
      unsigned int startBit :1;
      unsigned int transBit :1;
      unsigned int cmdIndex :6;
      card_status_t cardStatus;
      unsigned int crc7 :6;
      unsigned int endBit :1;
      unsigned int pad :16;
    };
  };
} rep1_t;

typedef struct
{
  union
  {
    unsigned int rep2[5];
    struct
    {
      union
      {
        cid_t cid;
        csd_t csd;
      };
      unsigned int endBit :1;
      unsigned int pad :32;
    };
  };
} rep2_t;

typedef struct
{
  union
  {
    unsigned int rep3[2];
    struct
    {
      unsigned int startBit :1;
      unsigned int transBit :1;
      unsigned int :6;
      ocr_t ocr;
      unsigned int :7;
      unsigned int endBit :1;
      unsigned int pad :16;
    };
  };
} rep3_t;

class OSDeviceMemoryCard : public OSDeviceBlock
{
public:

  // ----- Type definitions ---------------------------------------------------

  typedef uint16_t CommandClass_t;

  const static uint_t COMMAND_CLASS_SHIFT = 8;
  const static uint_t COMMAND_CODE_MASK = 0xFF;

  class CommandClass
  {
  public:
    const static CommandClass_t BASIC_0 = 0;
    const static CommandClass_t RESERVED_1 = 1;
    const static CommandClass_t BLOCK_READ_2 = 2;
    const static CommandClass_t RESERVED_3 = 3;
    const static CommandClass_t BLOCK_WRITE_4 = 4;
    const static CommandClass_t ERASE_5 = 5;
    const static CommandClass_t WRITE_PROTECTION_6 = 6;
    const static CommandClass_t LOCK_CARD_7 = 7;
    const static CommandClass_t APPLICATION_8 = 8;
    const static CommandClass_t IO_MODE__9 = 9;
    const static CommandClass_t SWITCH_10 = 10;
    const static CommandClass_t RESERVED_11 = 11;

    const static CommandClass_t MMC = 0x8000;
  };

  typedef uint16_t CommandCode_t;

  class CommandCode
  {
  public:
    // Class 0
    const static CommandCode_t GO_IDLE_STATE = 0;
    const static CommandCode_t SEND_OP_COND = 1;
    const static CommandCode_t ALL_SEND_CID = 2;
    const static CommandCode_t SEND_RELATIVE_ADDR = 3;
    const static CommandCode_t SET_DSR = 4;
    const static CommandCode_t SELECT_DESELECT_CARD = 7;
    const static CommandCode_t SEND_IF_COND = 8;
    const static CommandCode_t SEND_CSD = 9;
    const static CommandCode_t SEND_CID = 10;
    const static CommandCode_t VOLTAGE_SWITCH = 11;
    const static CommandCode_t STOP_TRANSMISSION = 12;
    const static CommandCode_t SEND_STATUS = 13;
    const static CommandCode_t GO_INACTIVE_STATE = 15;

    // Class 2
    const static CommandCode_t SET_BLOCKLEN = 16;
    const static CommandCode_t READ_SINGLE_BLOCK = 17;
    const static CommandCode_t READ_MULTIPLE_BLOCK = 18;
    const static CommandCode_t SEND_TUNING_BLOCK = 19;
    const static CommandCode_t SPEED_CLASS_CONTROL = 20;
    const static CommandCode_t SET_BLOCK_COUNT = 23;

    // Class 4
    //const static CommandCode_t SET_BLOCKLEN = 16;
    //const static CommandCode_t SPEED_CLASS_CONTROL = 20;
    //const static CommandCode_t SET_BLOCK_COUNT = 23;
    const static CommandCode_t WRITE_BLOCK = 24;
    const static CommandCode_t WRITE_MULTIPLE_BLOCK = 25;
    const static CommandCode_t PROGRAM_CSD = 27;

    // Class 5
    const static CommandCode_t ERASE_WR_BLK_START = 32;
    const static CommandCode_t ERASE_WR_BLK_END = 33;
    const static CommandCode_t ERASE = 38;
    // Class 6

    const static CommandCode_t SET_WRITE_PROT = 28;
    const static CommandCode_t CLR_WRITE_PROT = 29;
    const static CommandCode_t SEND_WRITE_PROT = 30;

    // Class 7
    //const static CommandCode_t SET_BLOCKLEN = 16;
    const static CommandCode_t LOCK_UNLOCK = 42;

    // Class 8 - pus 6, 13, 22, 23
    const static CommandCode_t APP_CMD = 55;
    const static CommandCode_t GEN_CMD = 56;

    // Class 9 - SDIO commands - 52-54

  };

  class ApplicationCommandCode
  {
  public:
    // Application Specific Commands
    const static CommandCode_t SET_BUS_WIDTH = (6
        | (CommandClass::APPLICATION_8 << COMMAND_CLASS_SHIFT));
    // ???
    const static CommandCode_t SEND_EXT_CSD = (8 | (CommandClass::APPLICATION_8
        << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SD_STATUS = (13 | (CommandClass::APPLICATION_8
        << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SEND_NUM_WR_BLOCKS = (22
        | (CommandClass::APPLICATION_8 << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SET_WR_BLK_ERASE_COUNT = (23
        | (CommandClass::APPLICATION_8 << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SD_SEND_OP_COND = (41
        | (CommandClass::APPLICATION_8 << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SET_CLR_CARD_DETECT = (42
        | (CommandClass::APPLICATION_8 << COMMAND_CLASS_SHIFT));
    const static CommandCode_t SEND_SCR = (51 | (CommandClass::APPLICATION_8
        << COMMAND_CLASS_SHIFT));
  };

  class SwitchCommandCode
  {
  public:
    // Class 10 - v1.10 - 34-37, 50, 57
    const static CommandCode_t SD_SWITCH_FUNC = (6 | (CommandClass::SWITCH_10
        << COMMAND_CLASS_SHIFT));
    const static CommandCode_t MMC_SWITCH_FUNC = (6 | (CommandClass::SWITCH_10
        << COMMAND_CLASS_SHIFT) | CommandClass::MMC);
  };

  typedef uint32_t CardStatus_t;

  class CardStatus
  {
  public:
    const static CardStatus_t ADR_OUT_OF_RANGE = (1 << 31); // 0x80000000
    const static CardStatus_t ADR_MISALIGN = (1 << 30);
    const static CardStatus_t BLOCK_LEN_ERROR = (1 << 29);
    const static CardStatus_t ERASE_SEQ_ERROR = (1 << 28);
    const static CardStatus_t ERASE_PARAM = (1 << 27);
    const static CardStatus_t WP_VIOLATION = (1 << 26);
    const static CardStatus_t CARD_IS_LOCKED = (1 << 25);
    const static CardStatus_t LOCK_UNLOCK_ = (1 << 24); // 0x01000000

    const static CardStatus_t COM_CRC_ERROR = (1 << 23); // 0x00800000
    const static CardStatus_t ILLEGAL_COMMAND = (1 << 22); // 0x00400000
    const static CardStatus_t CARD_ECC_FAILED = (1 << 21); // 0x00200000
    const static CardStatus_t CARD_ERROR = (1 << 20); // 0x00100000
    const static CardStatus_t EXEC_ERROR = (1 << 19); // 0x00080000
    const static CardStatus_t UNDERRUN = (1 << 18); // 0x00040000
    const static CardStatus_t OVERRUN = (1 << 17); // 0x00020000
    const static CardStatus_t CIDCSD_OVERWRITE = (1 << 16); // 0x00010000

    const static CardStatus_t WP_ERASE_SKIP = (1 << 15); // 0x00008000
    const static CardStatus_t CARD_ECC_DISABLE = (1 << 14); // 0x00004000
    const static CardStatus_t ERASE_RESET = (1 << 13); // 0x00002000
    const static CardStatus_t CURRENT_STATE_SHIFT = 9;
    const static CardStatus_t CURRENT_STATE_MASK = (0xF << CURRENT_STATE_SHIFT); // 0x00001E00
    const static CardStatus_t READY_FOR_DATA = (1 << 8); // 0x00000100


    const static CardStatus_t SWITCH_ERROR = (1 << 7); // 0x00000080
    const static CardStatus_t APP_CMD = (1 << 5);

    const static CardStatus_t FLAGERROR_RD_WR = (ADR_OUT_OF_RANGE
        | ADR_MISALIGN | BLOCK_LEN_ERROR | ERASE_SEQ_ERROR | ILLEGAL_COMMAND
        | CARD_ERROR);

    const static CardStatus_t CURRENT_STATE_IDLE = (0x0);
    const static CardStatus_t CURRENT_STATE_READY = (0x1);
    const static CardStatus_t CURRENT_STATE_IDENT = (0x2);
    const static CardStatus_t CURRENT_STATE_STBY = (0x3);
    const static CardStatus_t CURRENT_STATE_TRAN = (0x4);
    const static CardStatus_t CURRENT_STATE_DATA = (0x5);
    const static CardStatus_t CURRENT_STATE_RCV = (0x6);
    const static CardStatus_t CURRENT_STATE_PRG = (0x7);
    const static CardStatus_t CURRENT_STATE_DIS = (0x8);

  };

  typedef uint32_t CommandArgument_t;

  typedef uint32_t Response_t;

  typedef uint8_t BusWidth_t;

  class BusWidth
  {
  public:
    const static BusWidth_t _1 = 0;
    const static BusWidth_t _4 = 2;
    const static BusWidth_t _8 = 3;
  };

  typedef uint32_t BlockLength_t;
  typedef uint_t BlockCount_t;
  typedef uint32_t ClockFrequencyHz_t;

  // ==========================================================================

  // Base class for implementation classes

  class Implementation
  {
  public:

#if defined(DEBUG)
    Implementation();
    virtual
    ~Implementation();
#endif /* defined(DEBUG) */

    virtual bool
    isUsingSpi(void);

    friend class OSDeviceMemoryCard;

  private:

    // ----- Abstract methods ------------------------------------------------

    virtual OSReturn_t
    initialise(void) = 0;

    virtual OSReturn_t
    sendCommand(CommandCode_t code, CommandArgument_t arg) = 0;

    virtual Response_t
    readResponse(void) = 0;

    virtual bool
    isBusy(void) = 0;

    virtual bool
    isTransferDone(void) = 0;

    virtual OSReturn_t
    configureBusWidth(BusWidth_t busWidth) = 0;

    virtual OSReturn_t
    configureBlockLengthBytes(BlockLength_t length) = 0;

    virtual OSReturn_t
    configureBlockCount(BlockCount_t count) = 0;

    virtual OSReturn_t
    configureClockFrequencyHz(ClockFrequencyHz_t frequency) = 0;

    virtual OSReturn_t
    selectCard(void) = 0;

#if false
    virtual bool
    isRxReady(void) = 0;

    virtual uint32_t
    readData(void) = 0;

    virtual bool
    isTxReady(void) = 0;

    virtual void
    writeData(uint32_t value) = 0;
#endif

    virtual void
    configureHighSpeedMode(void) = 0;

    virtual bool
    isCrcError(void) = 0;

    virtual void
    transferIncommingBytes(void *pBuf, size_t bytes) = 0;

    virtual void
    transferOutgoingBytes(void *pBuf, size_t bytes) = 0;

    virtual void
    transferIncommingBytesWithoutDma(void *pBuf, size_t bytes) = 0;

    virtual bool
    isDataLineBusy(void) = 0;

  };

  typedef Implementation Implementation_t;

public:

  // ----- Constructors & Destructors -----------------------------------------

  OSDeviceMemoryCard(Implementation& impl);

  virtual
  ~OSDeviceMemoryCard();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operations.
  virtual OSReturn_t
  open(void);

  // Read exactly count blocks, in the given buffer.
  virtual OSReturn_t
  readBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Write the count blocks, from the given buffer.
  virtual OSReturn_t
  writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Prepare device for entering sleep
  virtual OSReturn_t
  close(void);

  // Erase part of the device.
  virtual OSReturn_t
  eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

  // Return the full size, in blocks, of the device.
  virtual OSDeviceBlock::BlockNumber_t
  getDeviceSize(void);

  // Return the device block size, in bytes.
  virtual OSDeviceBlock::BlockSize_t
  getBlockSize(void);

  Implementation_t&
  getImplementation(void);

private:

  OSReturn_t
  initialise(void);

  OSReturn_t
  executeCommand(CommandCode_t code, CommandArgument_t arg);

  void
  waitNotBusy(void);

  OSReturn_t
  getCsd(void);

  OSReturn_t
  getExtendedCsd(void);

#if false
  OSReturn_t
  setBlockLength(uint16_t length);

  OSReturn_t
  sendStatus(void);
#endif

  OSReturn_t
  prepareAccess(void);

  OSReturn_t
  prepareReadBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

  OSReturn_t
  prepareReadBlock(OSDeviceBlock::BlockNumber_t blockNumber);

  OSReturn_t
  finaliseReadBlocks(void);

  OSReturn_t
  finaliseReadBlock(void);

  OSReturn_t
  prepareWriteBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

  OSReturn_t
  prepareWriteBlock(OSDeviceBlock::BlockNumber_t blockNumber);

  void
  transferWriteBlocks(const void *pBuf, OSDeviceBlock::BlockCount_t count);

  OSReturn_t
  finaliseWriteBlocks(void);

  OSReturn_t
  finaliseWriteBlock(void);

private:

  // ----- Private members ----------------------------------------------------

  Implementation_t& m_implementation;

  uint32_t m_response;

  bool m_isOpened;

  uint8_t m_openCounter;

  uint8_t m_cardType;

  uint32_t m_cardRca;
  uint16_t m_cardFrequencyKHz;
  uint32_t m_cardSizeBlocks;

  uint32_t m_buf[64 / sizeof(uint32_t)];

  // --------------------------------------------------------------------------
};

inline OSDeviceMemoryCard::Implementation_t&
OSDeviceMemoryCard::getImplementation(void)
{
  return m_implementation;
}

#endif /* OSDEVICEMEMORYCARD_H_ */
