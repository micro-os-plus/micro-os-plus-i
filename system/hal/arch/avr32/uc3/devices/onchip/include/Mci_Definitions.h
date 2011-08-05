/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MCI_DEFINITIONS_H_
#define AVR32_UC3_MCI_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// Driver Default Value Configuration
#define MCI_DEFAULT_PWSDIV      0x07    // Power Saving Divider.

#define MCI_DEFAULT_DTOLMUL     0x07    // Data Timeout Multiplier.
//#define MCI_DEFAULT_DTOLCYC     0x02    // Data Timeout Cycle Number.
#define MCI_DEFAULT_DTOLCYC     0x0F    // Data Timeout Cycle Number.

#define MCI_DEFAULT_DTOREG      (MCI_DEFAULT_DTOLMUL<<AVR32_MCI_DTOR_DTOMUL_OFFSET) | \
                                (MCI_DEFAULT_DTOLCYC<<AVR32_MCI_DTOR_DTOCYC_OFFSET)

//! Global Error Mask
#define MCI_SR_ERROR            ( AVR32_MCI_SR_UNRE_MASK  |\
                                  AVR32_MCI_SR_OVRE_MASK  |\
                                  AVR32_MCI_SR_DTOE_MASK  |\
                                  AVR32_MCI_SR_DCRCE_MASK |\
                                  AVR32_MCI_SR_RTOE_MASK  |\
                                  AVR32_MCI_SR_RENDE_MASK |\
                                  AVR32_MCI_SR_RCRCE_MASK |\
                                  AVR32_MCI_SR_RDIRE_MASK |\
                                  AVR32_MCI_SR_RINDE_MASK)

#define MCI_SUCCESS             0 // Successful completion.

#define SD_MMC_SDCARD_APP_ALL_CMD \
            (MCI_SDCARD_SET_BUS_WIDTH_CMD + MCI_SDCARD_STATUS_CMD \
                + MCI_SDCARD_SEND_NUM_WR_BLOCKS_CMD \
                + MCI_SDCARD_SET_WR_BLK_ERASE_COUNT_CMD \
                + MCI_SDCARD_APP_OP_COND_CMD \
                + MCI_SDCARD_SET_CLR_CARD_DETECT_CMD + MCI_SDCARD_SEND_SCR_CMD)

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace mci
    {
      // ----- Type definitions -----------------------------------------------

      // These are exactly the register contents
      typedef uint32_t CommandWord_t;
      typedef uint32_t CommandArgument_t;

      typedef uint32_t StatusRegister_t;

      typedef uint8_t BusWidth_t;

      class BusWidth
      {
      public:
        const static BusWidth_t _1bit = 0;
        const static BusWidth_t _4bits = 2;
        const static BusWidth_t _8bits = 3;
      };

      typedef uint8_t CardSlot_t;

      class CardSlot
      {
      public:
        const static CardSlot_t A = 0;
        const static CardSlot_t B = 1;
      };

      typedef uint8_t TimeoutMultiplier_t;

      class TimeoutMultiplier
      {
      public:
        const static TimeoutMultiplier_t _1 = 0;
        const static TimeoutMultiplier_t _16 = 1;
        const static TimeoutMultiplier_t _128 = 2;
        const static TimeoutMultiplier_t _256 = 3;
        const static TimeoutMultiplier_t _1024 = 4;
        const static TimeoutMultiplier_t _4096 = 5;
        const static TimeoutMultiplier_t _65536 = 6;
        const static TimeoutMultiplier_t _1048576 = 7;
      };

      typedef uint8_t TimeoutCycles_t;

      class CommandWord
      {
      public:
        const static CommandWord_t MCI_SPCMD_NONE =
            (AVR32_MCI_CMDR_SPCMD_NO_SPEC_CMD << AVR32_MCI_CMDR_SPCMD_OFFSET); // = (MCI); Not a special CMD
        const static CommandWord_t MCI_SPCMD_INIT =
            (AVR32_MCI_CMDR_SPCMD_INIT_CMD << AVR32_MCI_CMDR_SPCMD_OFFSET); // = (MCI); Initialization CMD
        const static CommandWord_t MCI_SPCMD_SYNC =
            (AVR32_MCI_CMDR_SPCMD_SYNC_CMD << AVR32_MCI_CMDR_SPCMD_OFFSET); // = (MCI); Synchronized CMD
        const static CommandWord_t MCI_SPCMD_IT_CMD =
            (AVR32_MCI_CMDR_SPCMD_INT_CMD << AVR32_MCI_CMDR_SPCMD_OFFSET); // = (MCI); Interrupt command
        const static CommandWord_t MCI_SPCMD_IT_REP =
            (AVR32_MCI_CMDR_SPCMD_INT_RESP << AVR32_MCI_CMDR_SPCMD_OFFSET); // = (MCI); Interrupt response

        // Transfer Command
        const static CommandWord_t MCI_TRCMD_NO = (AVR32_MCI_TRCMD_NO_TRANS
            << AVR32_MCI_TRCMD_OFFSET); // = (MCI); No transfer
        const static CommandWord_t MCI_TRCMD_START =
            (AVR32_MCI_TRCMD_START_TRANS << AVR32_MCI_TRCMD_OFFSET); // = (MCI); Start transfer
        const static CommandWord_t MCI_TRCMD_STOP = (AVR32_MCI_TRCMD_STOP_TRANS
            << AVR32_MCI_TRCMD_OFFSET); // = (MCI); Stop transfer

        // Response Type
        const static CommandWord_t MCI_RSPTYP_NO = (AVR32_MCI_RSPTYP_NO_RESP
            << AVR32_MCI_RSPTYP_OFFSET); // = (MCI); No response
        const static CommandWord_t MCI_RSPTYP_48 =
            (AVR32_MCI_RSPTYP_48_BIT_RESP << AVR32_MCI_RSPTYP_OFFSET); // = (MCI); 48-bit response
        const static CommandWord_t MCI_RSPTYP_48B =
            (AVR32_MCI_RSPTYP_48_BIT_RESP_WITH_BUSY << AVR32_MCI_RSPTYP_OFFSET);// = (MCI); 48-bit response with busy
        const static CommandWord_t MCI_RSPTYP_136 =
            (AVR32_MCI_RSPTYP_136_BIT_RESP << AVR32_MCI_RSPTYP_OFFSET); // = (MCI); 136-bit response

        // Transfer Type
        const static CommandWord_t MCI_TRTYP_BLOCK = (AVR32_MCI_TRTYP_BLOCK
            << AVR32_MCI_TRTYP_OFFSET); // = (MCI); Block Transfer type
        const static CommandWord_t MCI_TRTYP_MULTIPLE =
            (AVR32_MCI_TRTYP_MULTI_BLOCK << AVR32_MCI_TRTYP_OFFSET); // = (MCI); Multiple Block transfer type
        const static CommandWord_t MCI_TRTYP_STREAM = (AVR32_MCI_TRTYP_STREAM
            << AVR32_MCI_TRTYP_OFFSET); // = (MCI); Stream transfer type
        const static CommandWord_t MCI_TRTYP_SDIO_BYTE =
            (AVR32_MCI_TRTYP_SDIO_BYTE << AVR32_MCI_TRTYP_OFFSET); // = (MCI); SDIO byte transfer type
        const static CommandWord_t MCI_TRTYP_SDIO_BLOCK =
            (AVR32_MCI_TRTYP_SDIO_BLOCK << AVR32_MCI_TRTYP_OFFSET); // = (MCI); SDIO block transfer type

        const static CommandWord_t MCI_OPDCMD = (AVR32_MCI_OPDCMD_MASK); // = (MCI); Open Drain Command
        const static CommandWord_t MCI_MAXLAT = (AVR32_MCI_MAXLAT_MASK); // = (MCI); Maximum Latency for Command to respond
        const static CommandWord_t MCI_TRDIR = (AVR32_MCI_TRDIR_MASK); // = (MCI); Transfer Direction

        //-- Class 0 & 1 commands: Basic commands and Read Stream commands
        const static CommandWord_t SD_MMC_GO_IDLE_STATE_CMD = (0 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE);
        const static CommandWord_t SD_MMC_INIT_STATE_CMD = (0 | MCI_TRCMD_NO
            | MCI_SPCMD_INIT);
        const static CommandWord_t SD_MMC_MMC_GO_IDLE_STATE_CMD = (0
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_OPDCMD);
        const static CommandWord_t SD_MMC_MMC_SEND_OP_COND_CMD = (1
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_OPDCMD);
        const static CommandWord_t SD_MMC_ALL_SEND_CID_CMD = (2 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_136);
        const static CommandWord_t SD_MMC_MMC_ALL_SEND_CID_CMD = (2
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_136 | MCI_OPDCMD);
        const static CommandWord_t SD_MMC_SET_RELATIVE_ADDR_CMD = (3
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_MMC_SET_RELATIVE_ADDR_CMD = (3
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT
            | MCI_OPDCMD);

        const static CommandWord_t MCI_SET_DSR_CMD = (4 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_NO | MCI_MAXLAT);

        const static CommandWord_t SD_MMC_SEL_DESEL_CARD_CMD = (7
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_48B | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SEND_EXT_CSD_CMD = (8
            | MCI_TRCMD_START | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRDIR
            | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SEND_CSD_CMD = (9 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_136 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SEND_CID_CMD = (10 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_136 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_MMC_READ_DAT_UNTIL_STOP_CMD = (11
            | MCI_TRTYP_STREAM | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRDIR
            | MCI_TRCMD_START | MCI_MAXLAT);

        const static CommandWord_t SD_MMC_STOP_READ_TRANSMISSION_CMD = (12
            | MCI_TRCMD_STOP | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_STOP_WRITE_TRANSMISSION_CMD = (12
            | MCI_TRCMD_STOP | MCI_SPCMD_NONE | MCI_RSPTYP_48B | MCI_MAXLAT);

        const static CommandWord_t SD_MMC_STOP_TRANSMISSION_SYNC_CMD = (12
            | MCI_TRCMD_STOP | MCI_SPCMD_SYNC | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SEND_STATUS_CMD = (13 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_GO_INACTIVE_STATE_CMD = (15
            | MCI_RSPTYP_NO);

        // ------------------------------------------------
        //  Class 2 commands: Block oriented Read commands
        // ------------------------------------------------

        const static CommandWord_t SD_MMC_SET_BLOCKLEN_CMD = (16 | MCI_TRCMD_NO
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_READ_SINGLE_BLOCK_CMD = (17
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_START
            | MCI_TRTYP_BLOCK | MCI_TRDIR | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_READ_MULTIPLE_BLOCK_CMD = (18
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_START
            | MCI_TRTYP_MULTIPLE | MCI_TRDIR | MCI_MAXLAT);

        // --------------------------------------------
        //  Class 3 commands: Sequential write commands
        // --------------------------------------------

        const static CommandWord_t SD_MMC_MMC_WRITE_DAT_UNTIL_STOP_CMD = (20
            | MCI_TRTYP_STREAM | MCI_SPCMD_NONE
            | (MCI_RSPTYP_48 & ~(MCI_TRDIR)) | MCI_TRCMD_START | MCI_MAXLAT); // MMC

        // ------------------------------------------------
        //  Class 4 commands: Block oriented write commands
        // ------------------------------------------------

        const static CommandWord_t SD_MMC_WRITE_BLOCK_CMD = (24
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_START
            | (MCI_TRTYP_BLOCK & ~(MCI_TRDIR)) | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_WRITE_MULTIPLE_BLOCK_CMD = (25
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_START
            | (MCI_TRTYP_MULTIPLE & ~(MCI_TRDIR)) | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_PROGRAM_CSD_CMD =
            (27 | MCI_RSPTYP_48);

        // ----------------------------------------
        //  Class 6 commands: Group Write protect
        // ----------------------------------------

        const static CommandWord_t SD_MMC_SET_WRITE_PROT_CMD = (28
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_CLR_WRITE_PROT_CMD = (29
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_SEND_WRITE_PROT_CMD = (30
            | MCI_RSPTYP_48);

        // ----------------------------------------
        //  Class 5 commands: Erase commands
        // ----------------------------------------

        const static CommandWord_t SD_MMC_TAG_SECTOR_START_CMD = (32
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_TAG_SECTOR_END_CMD = (33
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_MMC_UNTAG_SECTOR_CMD = (34
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_MMC_TAG_ERASE_GROUP_START_CMD = (35
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_MMC_TAG_ERASE_GROUP_END_CMD = (36
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_MMC_UNTAG_ERASE_GROUP_CMD = (37
            | MCI_RSPTYP_48);
        const static CommandWord_t SD_MMC_ERASE_CMD = (38 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);

        // ----------------------------------------
        //  Class 7 commands: Lock commands
        // ----------------------------------------

        const static CommandWord_t SD_MMC_LOCK_UNLOCK = (42 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT); // not tested

        // -----------------------------------------------
        //  Class 8 commands: Application specific commands
        // -----------------------------------------------
        const static CommandWord_t SD_MMC_SD_SEND_IF_COND_CMD = (8
            | MCI_TRCMD_NO | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_MAXLAT);

        const static CommandWord_t SD_MMC_APP_CMD = (55 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_GEN_CMD = (56 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT); // no tested
        const static CommandWord_t MMC_SWITCH_CMD = (6 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48B | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_SWITCH_FUNC = (6 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_TRCMD_START | MCI_TRTYP_BLOCK | MCI_TRDIR
            | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_SET_BUS_WIDTH_CMD = (6
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_STATUS_CMD = (13
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_SEND_NUM_WR_BLOCKS_CMD = (22
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_SET_WR_BLK_ERASE_COUNT_CMD =
            (23 | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_APP_OP_COND_CMD = (41
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO);
        const static CommandWord_t SD_MMC_SDCARD_SET_CLR_CARD_DETECT_CMD = (42
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_SDCARD_SEND_SCR_CMD = (51
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);

        // ----------------------------------------
        //  Class 9 commands: IO Mode commands
        // ----------------------------------------

        const static CommandWord_t SD_MMC_FAST_IO_CMD = (39 | MCI_SPCMD_NONE
            | MCI_RSPTYP_48 | MCI_MAXLAT);
        const static CommandWord_t SD_MMC_GO_IRQ_STATE_CMD = (40
            | MCI_SPCMD_NONE | MCI_RSPTYP_48 | MCI_TRCMD_NO | MCI_MAXLAT);

      };

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::MCI;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regReadWrite_t dtor; //0x0008
        regReadWrite_t sdcr; //0x000C

        regReadWrite_t argr; //0x0010
        regWriteOnly_t cmdr; //0x0014
        regReadWrite_t blkr; //0x0018
        regReadWrite_t cstor; //0x001C

        regReadOnly_t rspr[4]; //0x0020-0x002C

        regReadOnly_t rdr; //0x0030
        regWriteOnly_t tdr; //0x0034
        regNotAllocated_t
            dummy38[(0x0040 - 0x0038) / sizeof(regNotAllocated_t)]; //0x0038-0x003C

        regReadOnly_t sr; //0x0040
        regWriteOnly_t ier; //0x0044
        regWriteOnly_t idr; //0x0048
        regReadOnly_t imr; //0x004C

        regReadWrite_t dma; //0x0050
        regReadWrite_t cfg; //0x0054
        regNotAllocated_t
            dummy58[(0x00E4 - 0x0058) / sizeof(regNotAllocated_t)]; //0x0058-0x00E0

        regReadWrite_t wpmr; //0x00E4
        regReadOnly_t wpsr; //0x00E8
        regNotAllocated_t
            dummyEC[(0x00FC - 0x00EC) / sizeof(regNotAllocated_t)]; //0x00EC-0x00F8

        regReadOnly_t version; //0x00FC
        regNotAllocated_t dummy100[(0x0200 - 0x0100)
            / sizeof(regNotAllocated_t)]; //0x0100-0x01FF

        regReadWrite_t fifo[(0x4000 - 0x200) / sizeof(regReadWrite_t)]; //0x0200-0x3FFC


        // ----- Methods ------------------------------------------------------

        void
        writeControl(uint8_t mask);

        void
        writeMode(uint32_t value);

        uint32_t
        readMode(void);

        void
        writeDataTimeout(uint8_t value);

        uint8_t
        readDataTimeout(void);

        void
        writeSdCard(uint8_t value);

        uint8_t
        readSdCard(void);

        void
        writeArgument(uint32_t value);

        uint32_t
        readArgument(void);

        void
        writeCommand(uint32_t value);

        void
        writeBlock(uint32_t value);

        uint32_t
        readBlock(void);

        void
        writeCompletionSignalTimeout(uint8_t value);

        uint8_t
        readCompletionSignalTimeout(void);

        uint32_t
        readResponse(uint_t index);

        uint32_t
        readReceiveData(void);

        void
        writeTransmitData(uint32_t value);

        StatusRegister_t
        readStatus(void);

        // Writing 1 to a bit will set IMR; writing 0 will be ignored
        void
        writeInterruptEnable(uint32_t value);

        // Writing 1 to a bit will clear IMR; writing 0 will be ignored
        void
        writeInterruptDisable(uint32_t value);

        uint32_t
        readInterruptMask(void);

        void
        writeDmaConfiguration(uint16_t value);

        uint16_t
        readDmaConfiguration(void);

        void
        writeConfiguration(uint16_t value);

        uint16_t
        readConfiguration(void);

        void
        writeWriteProtectMode(uint32_t value);

        uint32_t
        readWriteProtectMode(void);

        uint32_t
        readWriteProtectStatus(void);

        uint32_t
        readVersion(void);

      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeControl(uint8_t mask)
      {
        this->cr = mask;
      }

      inline void
      ModuleRegisters::writeMode(uint32_t value)
      {
        this->mr = value;
      }

      inline uint32_t
      ModuleRegisters::readMode(void)
      {
        return this->mr;
      }

      inline void
      ModuleRegisters::writeDataTimeout(uint8_t value)
      {
        this->dtor = value;
      }

      inline uint8_t
      ModuleRegisters::readDataTimeout(void)
      {
        return this->dtor;
      }

      inline void
      ModuleRegisters::writeSdCard(uint8_t value)
      {
        this->sdcr = value;
      }

      inline uint8_t
      ModuleRegisters::readSdCard(void)
      {
        return this->sdcr;
      }

      inline void
      ModuleRegisters::writeArgument(CommandArgument_t value)
      {
        this->argr = value;
      }

      inline CommandArgument_t
      ModuleRegisters::readArgument(void)
      {
        return this->argr;
      }

      inline void
      ModuleRegisters::writeCommand(CommandWord_t value)
      {
        this->cmdr = value;
      }

      inline void
      ModuleRegisters::writeBlock(uint32_t value)
      {
        this->blkr = value;
      }

      inline uint32_t
      ModuleRegisters::readBlock(void)
      {
        return this->blkr;
      }

      inline void
      ModuleRegisters::writeCompletionSignalTimeout(uint8_t value)
      {
        this->cstor = value;
      }

      inline uint8_t
      ModuleRegisters::readCompletionSignalTimeout(void)
      {
        return this->cstor;
      }

      inline uint32_t
      ModuleRegisters::readResponse(uint_t index)
      {
        return this->rspr[index];
      }

      inline uint32_t
      ModuleRegisters::readReceiveData(void)
      {
        return this->rdr;
      }

      inline void
      ModuleRegisters::writeTransmitData(uint32_t value)
      {
        this->tdr = value;
      }

      inline StatusRegister_t
      ModuleRegisters::readStatus(void)
      {
        return this->sr;
      }

      // Writing 1 to a bit will se IMRt; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptEnable(uint32_t value)
      {
        this->ier = value;
      }

      // Writing 1 to a bit will clear IMR; writing 0 will be ignored
      inline void
      ModuleRegisters::writeInterruptDisable(uint32_t value)
      {
        this->idr = value;
      }

      inline uint32_t
      ModuleRegisters::readInterruptMask(void)
      {
        return this->imr;
      }

      inline void
      ModuleRegisters::writeDmaConfiguration(uint16_t value)
      {
        this->dma = value;
      }

      inline uint16_t
      ModuleRegisters::readDmaConfiguration(void)
      {
        return this->dma;
      }

      inline void
      ModuleRegisters::writeConfiguration(uint16_t value)
      {
        this->cfg = value;
      }

      inline uint16_t
      ModuleRegisters::readConfiguration(void)
      {
        return this->cfg;
      }

      inline void
      ModuleRegisters::writeWriteProtectMode(uint32_t value)
      {
        this->wpmr = value;
      }

      inline uint32_t
      ModuleRegisters::readWriteProtectMode(void)
      {
        return this->wpmr;
      }

      inline uint32_t
      ModuleRegisters::readWriteProtectStatus(void)
      {
        return this->wpsr;
      }

      inline uint32_t
      ModuleRegisters::readVersion(void)
      {
        return this->version;
      }

    }

  // --------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_MCI_DEFINITIONS_H_ */
