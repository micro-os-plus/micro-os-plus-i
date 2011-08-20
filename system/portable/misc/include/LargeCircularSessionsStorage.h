/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARSESSIONSSTORAGE_H_
#define LARGECIRCULARSESSIONSSTORAGE_H_

#include "portable/kernel/include/uOS.h"

#include "portable/util/endianness/include/BigEndian.h"

// ----------------------------------------------------------------------------
//
// These classes handle the management of sessions on a large block storage,
// used in a circular manner. Sessions are ordered sets of data, grouped
// by a common id, usually representing the absolute session start time.
// Large storage means a device with a very large number of blocks, like
// a memory card.
//
// The intended applications are data loggers.
//
// These classes require a generic OSDeviceBlock device for storage.
//
// One session block may contain multiple device blocks.
//
// There are many nested classes:
//      LargeCircularSessionsStorage - manages the read/write of session blocks
//      LargeCircularSessionsStorage::Header
//      LargeCircularSessionsStorage::Writer
//      LargeCircularSessionsStorage::Reader
//
// ----------------------------------------------------------------------------

class LargeCircularSessionsStorage
{
public:

  // ----- Public type definitions --------------------------------------------

  typedef uint32_t Magic_t;

#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE_LARGE_SESSIONID)
  typedef uint64_t SessionUniqueId_t;
#else
  typedef uint32_t SessionUniqueId_t;
#endif

  typedef uint32_t BlockUniqueId_t;
  const static BlockUniqueId_t BEGINING_UNIQUE_ID = 0x00000001;

  typedef uint32_t SessionBlockNumber_t;
  typedef uint32_t SessionBlockCount_t;

  class Header
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Header();
    ~Header();

    // ----- Public methods ---------------------------------------------------

    void
    copyFromHeader(uint8_t* pHeader);

    // Regular getters/setters, apply on members
    void
    setMagic(Magic_t magic);
    Magic_t
    getMagic(void);

    void
    setSessionUniqueId(SessionUniqueId_t sessionUniqueId);
    SessionUniqueId_t
    getSessionUniqueId(void);

    void
    setSessionFirstBlockNumber(SessionBlockNumber_t sessionFirstBlockNumber);
    SessionBlockNumber_t
    getSessionFirstBlockNumber(void);

    void
    setBlockUniqueId(BlockUniqueId_t blockUniqueId);
    BlockUniqueId_t
    getBlockUniqueId(void);

    // ----- Static public methods --------------------------------------------

    // Static methods, apply on a block header
    static void
    writeMagic(uint8_t* pHeader, Magic_t magic);
    static Magic_t
    readMagic(uint8_t* pHeader);

    static void
    writeSessionUniqueId(uint8_t* pHeader, SessionUniqueId_t sessionUniqueId);
    static SessionUniqueId_t
    readSessionUniqueId(uint8_t* pHeader);

    static void
    writeSessionFirstBlockNumber(uint8_t* pHeader,
        SessionBlockNumber_t sessionBlockNumber);
    static SessionBlockNumber_t
    readSessionFirstBlockNumber(uint8_t* pHeader);

    static void
    writeBlockUniqueId(uint8_t* pHeader, BlockUniqueId_t blockUniqueId);
    static BlockUniqueId_t
    readBlockUniqueId(uint8_t* pHeader);

    static std::size_t
    getSize(void);

    // The version byte must be increased it the structure of the header is
    // changed, to avoid trying to read wrong data.
    // The effect of changing the version is that the storage will be
    // rewritten, since the circular buffer will restart from the beginning.

    const static uint8_t VERSION = 0x01;
    const static Magic_t MAGIC = (0x55AAA500 | VERSION);

  private:

    // ----- Private members --------------------------------------------------

    const static std::size_t OFFSET_OF_MAGIC = 0;
    Magic_t m_magic;

    const static std::size_t OFFSET_OF_SESSIONUNIQEID = OFFSET_OF_MAGIC
        + sizeof(Magic_t);
    SessionUniqueId_t m_sessionUniqueId;

    const static std::size_t OFFSET_OF_SESSIONFIRSTBLOCKNUMBER =
        OFFSET_OF_SESSIONUNIQEID + sizeof(SessionUniqueId_t);
    SessionBlockNumber_t m_sessionFirstBlockNumber;

    const static std::size_t OFFSET_OF_BLOCKUNIQUEID =
        OFFSET_OF_SESSIONFIRSTBLOCKNUMBER + sizeof(SessionBlockNumber_t);
    BlockUniqueId_t m_blockUniqueId;

    const static std::size_t SIZE_OF_HEADER = OFFSET_OF_BLOCKUNIQUEID
        + sizeof(BlockUniqueId_t);

    // ------------------------------------------------------------------------
  };

  // ----- Constructors & destructors -----------------------------------------

  LargeCircularSessionsStorage(OSDeviceBlock& device);
  ~LargeCircularSessionsStorage();

  // ----- Public methods -----------------------------------------------------

  // Common (writer & reader) methods

  // Return the amount of space the application should reserve
  // at the beginning of each written session block, or the amount
  // of space the reader should skip
  uint_t
  getReservedHeaderSizeBytes(void);

  OSDeviceBlock&
  getDevice(void);

  // set/get the number of device blocks per session block
  void
  setSessionBlockSizeBlocks(OSDeviceBlock::BlockSize_t blockSizeBlocks);
  OSDeviceBlock::BlockSize_t
  getSessionBlockSizeBlocks(void);

  // Return the number of session blocks available in the device (partition)
  // This is done by dividing the physical size by the number of blocks
  // per session block
  SessionBlockNumber_t
  getStorageSizeSessionBlocks(void);

  // Read a session block from the storage. The block may be incomplete,
  // the first 'deviceBlocksCount'.
  OSReturn_t
  readStorageBlock(SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
      OSDeviceBlock::BlockCount_t deviceBlocksCount);

  // Write a session block from the storage. The block may be incomplete,
  // the first 'deviceBlocksCount'.
  OSReturn_t
  writeStorageBlock(SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
      OSDeviceBlock::BlockCount_t deviceBlocksCount);


private:

  OSReturn_t
  searchMostRecentWrittenBlock(SessionBlockNumber_t* plast,
      BlockUniqueId_t* pid);

  OSReturn_t
  readBlkHdr(uint8_t* pbuf, SessionBlockNumber_t blk, BlockUniqueId_t* pid,
      SessionBlockNumber_t* pbeg, uint_t rtry = 0);

  SessionBlockNumber_t
  computeCircularSessionBlockNumber(SessionBlockNumber_t blockNumber, int adjustment);

private:
  OSDeviceBlock& m_device;

  OSDeviceBlock::BlockSize_t m_blockSizeBlocks;

  // temporary buffer used for search
  uint8_t m_ibuf[512] __attribute__((aligned(4)));

public:
  // Writer
  class Writer
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Writer(LargeCircularSessionsStorage& storage);
    ~Writer();

    // ----- Public methods ---------------------------------------------------

    // Return the underlying storage device
    LargeCircularSessionsStorage&
    getStorage(void);

    // Search for the most recent written block and prepare write from there
    OSReturn_t
    createSession(SessionUniqueId_t sessionId);

    // Fill in the header and write the entire session block.
    // pSessionBlock must point to a byte array of multiple device blocks.
    // The write address is automatically incremented.
    OSReturn_t
    writeSessionBlock(uint8_t* pSessionBlock);

    OSReturn_t
    closeSession(void);

  private:

    // ----- Private methods --------------------------------------------------

  private:

    // ----- Private members --------------------------------------------------

    LargeCircularSessionsStorage& m_storage;

    // An application supplied unique id for identifying sessions
    SessionUniqueId_t m_sessionUniqueId;
    // A monotone increasing unique Id
    BlockUniqueId_t m_blockUniqueId;

    // The block number is the session block number, not the
    // device physical block number; one session block contains
    // setSessionBlockSizeBlocks() physical blocks
    OSDeviceBlock::BlockNumber_t m_sessionFirstBlockNumber;
    OSDeviceBlock::BlockNumber_t m_currentBlockNumber;

    // ------------------------------------------------------------------------
  };

  class Reader
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Reader(LargeCircularSessionsStorage& storage);
    ~Reader();

    // ----- Public methods ---------------------------------------------------

    LargeCircularSessionsStorage&
    getStorage(void);

    // Searches for the given session
    OSReturn_t
    openSession(SessionUniqueId_t sessionId);

    // Without a session id, search for the most recent one
    OSReturn_t
    openMostRecentSession(void);

    OSReturn_t
    openOldestSession(void);

    OSReturn_t
    openPreviousSession(void);

    OSReturn_t
    openNextSession(void);

    // Compute the length of the current session, in session blocks
    SessionBlockCount_t
    computeSessionLength(void);

    // Read the requested session block (minimum one device block)
    // The block number should be between 0 and (sessionLength-1)
    OSReturn_t
    readSessionBlock(SessionBlockNumber_t blockNumber, uint8_t* pBuf,
        OSDeviceBlock::BlockCount_t deviceBlocksCount);

    OSReturn_t
    closeSession(void);

  protected:
    LargeCircularSessionsStorage& m_storage;

    // The block number is the session block number, not the
    // device physical block number; one session block contains
    // setSessionBlockSizeBlocks() physical blocks
    SessionBlockNumber_t m_sessionFirstBlockNumber;

    SessionUniqueId_t m_sessionUniqueId;

    // temporary buffer
    uint8_t m_ibuf[512] __attribute__((aligned(4)));

  };

};

// ============================================================================

inline void
LargeCircularSessionsStorage::Header::setMagic(Magic_t magic)
{
  m_magic = magic;
}

inline LargeCircularSessionsStorage::Magic_t
LargeCircularSessionsStorage::Header::getMagic(void)
{
  return m_magic;
}

inline void
LargeCircularSessionsStorage::Header::setSessionUniqueId(
    SessionUniqueId_t sessionUniqueId)
{
  m_sessionUniqueId = sessionUniqueId;
}

inline LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Header::getSessionUniqueId(void)
{
  return m_sessionUniqueId;
}

inline void
LargeCircularSessionsStorage::Header::setSessionFirstBlockNumber(
    SessionBlockNumber_t sessionFirstBlockNumber)
{
  m_sessionFirstBlockNumber = sessionFirstBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::getSessionFirstBlockNumber(void)
{
  return m_sessionFirstBlockNumber;
}

inline void
LargeCircularSessionsStorage::Header::setBlockUniqueId(
    BlockUniqueId_t blockUniqueId)
{
  m_blockUniqueId = blockUniqueId;
}

inline LargeCircularSessionsStorage::BlockUniqueId_t
LargeCircularSessionsStorage::Header::getBlockUniqueId(void)
{
  return m_blockUniqueId;
}

inline std::size_t
LargeCircularSessionsStorage::Header::getSize(void)
{
  return LargeCircularSessionsStorage::Header::SIZE_OF_HEADER;
}

inline void
LargeCircularSessionsStorage::Header::writeMagic(uint8_t* pHeader,
    Magic_t value)
{
  util::endianness::BigEndian::writeUnsigned32(pHeader + OFFSET_OF_MAGIC, value);
}

inline LargeCircularSessionsStorage::Magic_t
LargeCircularSessionsStorage::Header::readMagic(uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(pHeader + OFFSET_OF_MAGIC);
}

inline void
LargeCircularSessionsStorage::Header::writeSessionUniqueId(uint8_t* pHeader,
    SessionUniqueId_t value)
{
  util::endianness::BigEndian::writeUnsigned32(pHeader + OFFSET_OF_SESSIONUNIQEID, value);
}

inline LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Header::readSessionUniqueId(uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(pHeader + OFFSET_OF_SESSIONUNIQEID);
}

inline void
LargeCircularSessionsStorage::Header::writeSessionFirstBlockNumber(
    uint8_t* pHeader, SessionBlockNumber_t value)
{
  util::endianness::BigEndian::writeUnsigned32(pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER,
      value);
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::readSessionFirstBlockNumber(
    uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(
      pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER);
}

inline void
LargeCircularSessionsStorage::Header::writeBlockUniqueId(uint8_t* pHeader,
    BlockUniqueId_t value)
{
  util::endianness::BigEndian::writeUnsigned32(pHeader + OFFSET_OF_BLOCKUNIQUEID, value);
}

inline LargeCircularSessionsStorage::BlockUniqueId_t
LargeCircularSessionsStorage::Header::readBlockUniqueId(uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(pHeader + OFFSET_OF_BLOCKUNIQUEID);
}

// ============================================================================

inline uint_t
LargeCircularSessionsStorage::getReservedHeaderSizeBytes(void)
{
  return LargeCircularSessionsStorage::Header::getSize();
}

inline OSDeviceBlock&
LargeCircularSessionsStorage::getDevice(void)
{
  return m_device;
}

inline void
LargeCircularSessionsStorage::setSessionBlockSizeBlocks(
    OSDeviceBlock::BlockSize_t blockSizeBlocks)
{
  m_blockSizeBlocks = blockSizeBlocks;
}

inline OSDeviceBlock::BlockSize_t
LargeCircularSessionsStorage::getSessionBlockSizeBlocks(void)
{
  return m_blockSizeBlocks;
}

// ============================================================================

inline LargeCircularSessionsStorage&
LargeCircularSessionsStorage::Writer::getStorage(void)
{
  return m_storage;
}

// ============================================================================

inline LargeCircularSessionsStorage&
LargeCircularSessionsStorage::Reader::getStorage(void)
{
  return m_storage;
}

// ============================================================================


#endif /* LARGECIRCULARSESSIONSSTORAGE_H_ */
