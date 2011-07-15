/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEMEMORYCARD_H_
#define OSDEVICEMEMORYCARD_H_

#include "portable/kernel/include/OS.h"

#include "OSDeviceAddressable.h"

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
//! Flag error of "Card Status" in R1
#define CS_FLAGERROR_RD_WR  (CS_ADR_OUT_OF_RANGE|CS_ADR_MISALIGN|CS_BLOCK_LEN_ERROR|CS_ERASE_SEQ_ERROR|CS_ILLEGAL_COMMAND|CS_CARD_ERROR)
#define CS_ADR_OUT_OF_RANGE (1<<31)
#define CS_ADR_MISALIGN     (1<<30)
#define CS_BLOCK_LEN_ERROR  (1<<29)
#define CS_ERASE_SEQ_ERROR  (1<<28)
#define CS_ERASE_PARAM      (1<<27)
#define CS_WP_VIOLATION     (1<<26)
#define CS_CARD_IS_LOCKED   (1<<25)
#define CS_LOCK_UNLOCK_     (1<<24)
#define CS_COM_CRC_ERROR    (1<<23)
#define CS_ILLEGAL_COMMAND  (1<<22)
#define CS_CARD_ECC_FAILED  (1<<21)
#define CS_CARD_ERROR       (1<<20)
#define CS_EXEC_ERROR       (1<<19)
#define CS_UNDERRUN         (1<<18)
#define CS_OVERRUN          (1<<17)
#define CS_CIDCSD_OVERWRITE (1<<16)
#define CS_WP_ERASE_SKIP    (1<<15)
#define CS_ERASE_RESET      (1<<13)
#define CS_READY_FOR_DATA   (1<<8)
#define CS_SWITCH_ERROR     (1<<7)
#define CS_APP_CMD          (1<<5)

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

class OSDeviceMemoryCard : public OSDeviceAddressable
{
public:

  // ----- Type definitions ---------------------------------------------------

  typedef uint16_t CommandClass_t;

  const static uint_t COMMAND_CLASS_SHIFT = 0;

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
  };

  typedef uint8_t CommandCode_t;

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
    const static CommandCode_t READ_MULTIPE_BLOCK = 18;
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
    const static CommandCode_t SWITCH_FUNC = (6 | (CommandClass::SWITCH_10
        << COMMAND_CLASS_SHIFT));
  };

  typedef uint32_t CommandArgument_t;

  typedef uint32_t Response_t;

  typedef uint8_t BusWidth_t;

  class BusWidth
  {
  public:
    const static BusWidth_t _1 = 1;
    const static BusWidth_t _4 = 4;
    const static BusWidth_t _8 = 8;
  };

  typedef uint_t BlockSize_t;
  typedef uint_t BlockCount_t;

public:

  // ----- Constructors & Destructors -----------------------------------------

  OSDeviceMemoryCard();

  virtual
  ~OSDeviceMemoryCard();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operations.
  virtual OSReturn_t
  open(void);

  // Read exactly count bytes, in the given buffer.
  // Offset need not be aligned.
  virtual OSReturn_t
  readBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
      OSDeviceAddressable::Count_t count);

  // Write the given buffer. Offset need to be aligned with
  // getWriteAlignement() and count should be multiple of it.
  virtual OSReturn_t
  writeBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
      OSDeviceAddressable::Count_t count);

  // Prepare device for entering sleep
  virtual OSReturn_t
  close(void);

  // Erase part of the device, aligned to page if needed.
  virtual OSReturn_t
  erase(OSDeviceAddressable::Offset_t offset,
      OSDeviceAddressable::Count_t count);

  // Erase the entire device
  virtual OSReturn_t
  eraseEntireDevice(void);

  // Return the full size, in bytes, of the device.
  virtual OSDeviceAddressable::Offset_t
  getDeviceSize(void);

  // Return the write block size, to be used as alignment.
  virtual OSDeviceAddressable::Alignnment_t
  getWriteAlignment(void);

  // Return the erase page size, to be used as alignment.
  virtual OSDeviceAddressable::Alignnment_t
  getEraseAlignment(void);

private:
  // ----- Implementation methods ---------------------------------------------

  virtual OSReturn_t
  implInit(void) = 0;

  virtual OSReturn_t
  implSendCommand(CommandCode_t code, CommandArgument_t arg) = 0;

  virtual Response_t
  implReadResponse(void) = 0;

  virtual OSReturn_t
  implWaitBusySignal(void) = 0;

  virtual OSReturn_t
  implSetBusWidth(BusWidth_t busWidth) = 0;

  virtual OSReturn_t
  implSetBlockSize(BlockSize_t size) = 0;

  virtual OSReturn_t
  implSetBlockCount(BlockCount_t count) = 0;

  virtual OSReturn_t
  mci_set_speed() = 0;

  OSReturn_t
  getCsd(void);

  OSReturn_t
  sd_mmc_get_ext_csd(void);

  bool
  sd_mmc_set_block_len(uint8_t slot, uint16_t length);

  bool
  sd_mmc_mci_cmd_send_status(uint8_t slot);

private:

  // ----- Private members ----------------------------------------------------

  uint8_t m_cardType;
  bool m_isInitialised;

  // --------------------------------------------------------------------------
};

#endif /* OSDEVICEMEMORYCARD_H_ */
