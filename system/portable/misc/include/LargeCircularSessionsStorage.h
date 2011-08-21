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
  const static BlockUniqueId_t BEGINING_BLOCK_UNIQUE_ID = 1;

  typedef uint32_t SessionBlockNumber_t;
  typedef uint32_t SessionBlockCount_t;

  // This inner class handles the session block header fields.
  // In addition to regular getters/setters, it has methods to read/write
  // hedader fields at proper byte positions.
  class Header
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Header();
    ~Header();

    // ----- Public methods ---------------------------------------------------

    // Used to copy all fields from one object to another
    Header&
    operator=(const Header& header);

    // Read all values from the block header.
    void
    readFromBlock(const uint8_t* pBlock);

    // Write all values to the block header.
    void
    writeToBlock(uint8_t* pBlock);

    // Regular getters/setters for all members
    void
    setMagic(Magic_t magic);
    Magic_t
    getMagic(void);

    void
    setSessionUniqueId(SessionUniqueId_t sessionUniqueId);
    SessionUniqueId_t
    getSessionUniqueId(void);

    void
    incrementSessionUniqueId(void);

    void
    setSessionFirstBlockNumber(SessionBlockNumber_t sessionFirstBlockNumber);
    SessionBlockNumber_t
    getSessionFirstBlockNumber(void);

    void
    setBlockUniqueId(BlockUniqueId_t blockUniqueId);
    BlockUniqueId_t
    getBlockUniqueId(void);

    void
    incrementBlockUniqueId(void);

    void
    setNextSessionFirstBlockNumber(
        SessionBlockNumber_t nextSessionFirstBlockNumber);
    SessionBlockNumber_t
    getNextSessionFirstBlockNumber(void);

    // ----- Static public methods --------------------------------------------

    // Apply on a block header
    static void
    writeMagic(Magic_t magic, uint8_t* pHeader);
    static Magic_t
    readMagic(const uint8_t* pHeader);

    static void
    writeSessionUniqueId(SessionUniqueId_t sessionUniqueId, uint8_t* pHeader);
    static SessionUniqueId_t
    readSessionUniqueId(const uint8_t* pHeader);

    static void
    writeSessionFirstBlockNumber(SessionBlockNumber_t sessionBlockNumber,
        uint8_t* pHeader);
    static SessionBlockNumber_t
    readSessionFirstBlockNumber(const uint8_t* pHeader);

    static void
    writeBlockUniqueId(BlockUniqueId_t blockUniqueId, uint8_t* pHeader);
    static BlockUniqueId_t
    readBlockUniqueId(const uint8_t* pHeader);

    static void
    writeNextSessionFirstBlockNumber(
        SessionBlockNumber_t nextSessionFirstBlockNumber, uint8_t* pHeader);
    static SessionBlockNumber_t
    readNextSessionFirstBlockNumber(const uint8_t* pHeader);

    static void
    fillUnusedSpace(uint8_t* pHeader);

    static std::size_t
    getSize(void);

    // The version byte must be increased it the structure of the header is
    // changed, to avoid trying to read wrong data.
    // The effect of changing the version is that the storage will be
    // rewritten, since the circular buffer will restart from the beginning.

    const static uint8_t VERSION = 0x01;
    const static Magic_t MAGIC = (0x55AAA500 | VERSION);

    const static std::size_t DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER = ~0;

  private:

    // ----- Private members --------------------------------------------------

    Magic_t m_magic;
    SessionUniqueId_t m_sessionUniqueId;
    SessionBlockNumber_t m_sessionFirstBlockNumber;
    BlockUniqueId_t m_blockUniqueId;
    SessionBlockNumber_t m_nextSessionFirstBlockNumber;

    // Offsets when stored in a block header.
    const static std::size_t OFFSET_OF_MAGIC = 0;
    const static std::size_t OFFSET_OF_SESSIONUNIQEID = OFFSET_OF_MAGIC
        + sizeof(Magic_t);
    const static std::size_t OFFSET_OF_SESSIONFIRSTBLOCKNUMBER =
        OFFSET_OF_SESSIONUNIQEID + sizeof(SessionUniqueId_t);
    const static std::size_t OFFSET_OF_BLOCKUNIQUEID =
        OFFSET_OF_SESSIONFIRSTBLOCKNUMBER + sizeof(SessionBlockNumber_t);
    const static std::size_t OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER =
        OFFSET_OF_BLOCKUNIQUEID + sizeof(BlockUniqueId_t);

    const static std::size_t SIZE_OF_HEADER =
        OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER + sizeof(SessionBlockNumber_t);

    // Mainly for aesthetic reasons, we start the header payload on the
    // next 16 byte border.
    const static int HEADER_ALIGN = (16 - 1);

    // Unused header space is filled with this value.
    const static uint8_t UNUSED_SPACE_FILL_BYTE = 0xFF;

    // ------------------------------------------------------------------------
  };

  // ----- Constructors & destructors -----------------------------------------

  LargeCircularSessionsStorage(OSDeviceBlock& device);
  ~LargeCircularSessionsStorage();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operation. It should always be
  // paired with a closeStorage(), since a reference counter is used.
  OSReturn_t
  openStorage(void);

  // Inform the underlying device it is no longer used
  // Only after the last close is performed, the device is closed,
  // so it is mandatory to pair it with openStorage()
  OSReturn_t
  closeStorage(void);

  // Return the amount of space the application should reserve
  // at the beginning of each written session block, or the amount
  // of space the reader should skip
  uint_t
  getReservedHeaderSizeBytes(void);

  // Give a way to access the underlying block device
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
  // only i.e. the first 'deviceBlocksCount' of a full session block.
  OSReturn_t
  readStorageBlock(SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
      OSDeviceBlock::BlockCount_t deviceBlocksCount);

  // Write a session block from the storage. The block may be incomplete,
  // only i.e. the first 'deviceBlocksCount' of a full session block.
  OSReturn_t
  writeStorageBlock(SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
      OSDeviceBlock::BlockCount_t deviceBlocksCount);

private:

  // All new sessions are created just after the most recent existing one.
  // This requires a smart scan of the storage, to identify the
  // place where a major discontinuity in the block unique id is found.
  // Normally the block unique id is incremented after each write, and since
  // the total number of blocks in the storage is significantly lower than
  // the range of this counter, it can be used to identify more easily
  // the discontinuity.
  OSReturn_t
  searchMostRecentlyWrittenBlock(uint8_t* pBlock,
      SessionBlockNumber_t* pMostRecentBlock,
      LargeCircularSessionsStorage::Header& header);

  // Adjust the block number with a positive or negative adjustment,
  // also handling roll over, on both ends of the circular buffer
  SessionBlockNumber_t
  computeCircularSessionBlockNumber(SessionBlockNumber_t blockNumber,
      int adjustment);

  // Update the header of the current session to point to the next (future)
  // session. This is to help forward navigation, since backward
  // navigation is already possible, all blocks point to the first
  // session block
  OSReturn_t
  updateForwardReference(SessionBlockNumber_t sessionFirstBlockNumber,
      SessionBlockNumber_t nextSessionFirstBlockNumber, uint8_t* pBlock);

private:

  OSDeviceBlock& m_device;

  OSDeviceBlock::BlockSize_t m_blockSizeBlocks;

public:

  // This class handles all operations needed to write on the circular storage.
  // Basically the procedure is to create a session, to write all blocks
  // and to close the session.

  class Writer
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Writer(LargeCircularSessionsStorage& storage);
    ~Writer();

    // ----- Public methods ---------------------------------------------------

    // Return the underlying storage device.
    LargeCircularSessionsStorage&
    getStorage(void);

    // Search for the most recent written block and prepare write from there
    // If the sessionId is 0, just use the next id.
    OSReturn_t
    createSession(SessionUniqueId_t sessionUniqueId = 0);

    // Fill in the header and write the entire session block.
    // pSessionBlock must point to a byte array of multiple device blocks.
    // The write address is automatically incremented.
    OSReturn_t
    writeSessionBlock(uint8_t* pSessionBlock);

    // Update the session header.
    OSReturn_t
    closeSession(void);

  private:

    // ----- Private methods --------------------------------------------------

  private:

    // ----- Private members --------------------------------------------------

    LargeCircularSessionsStorage& m_storage;

    // Header for the current block
    Header m_currentHeader;

    SessionBlockNumber_t m_currentBlockNumber;

    // temporary buffer used for search and update forward references
    uint8_t m_block[512] __attribute__((aligned(4)));

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

    // Searches for the given session.
    OSReturn_t
    openSession(SessionUniqueId_t sessionId);

    // Search for the most recent session.
    OSReturn_t
    openMostRecentSession(void);

    // Search for the oldest session still available in the storage.
    OSReturn_t
    openOldestSession(void);

    // Navigate to the previous session.
    OSReturn_t
    openPreviousSession(void);

    // Navigate to the next session.
    OSReturn_t
    openNextSession(void);

    // Return the length of the current session, in session blocks.
    // If, for any reason, this information cannot be computed for the
    // current session, 0 will be returned.
    SessionBlockCount_t
    getSessionLength(void);

    // Read the requested session block (minimum one device block)
    // The block number should be between 0 and (sessionLength-1)
    OSReturn_t
    readSessionBlock(SessionBlockNumber_t blockNumber,
        OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlock);

#if false
    // These methods might be removed, use readSessionBlock()

    // Read the next session block (minimum one device block)
    // The block number should be between 0 and (sessionLength-1)
    OSReturn_t
    readNextSessionBlock(OSDeviceBlock::BlockCount_t deviceBlocksCount,
        uint8_t* pBlock);

    // Read the previous session block (minimum one device block)
    // The block number should be between 0 and (sessionLength-1)
    OSReturn_t
    readPreviousSessionBlock(OSDeviceBlock::BlockCount_t deviceBlocksCount,
        uint8_t* pBlock);
#endif

    // Mainly for completeness, not much to do.
    OSReturn_t
    closeSession(void);

  protected:

    LargeCircularSessionsStorage& m_storage;

    // Header for the current block
    Header m_currentHeader;

    SessionBlockNumber_t m_sessionLastBlockNumber;
    SessionBlockNumber_t m_sessionLength;

    OSDeviceBlock::BlockNumber_t m_currentBlockNumber;
    SessionBlockNumber_t m_mostRecentlyWrittenBlockNumber;

    // temporary buffer
    uint8_t m_block[512] __attribute__((aligned(4)));
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
LargeCircularSessionsStorage::Header::incrementSessionUniqueId(void)
{
  ++m_sessionUniqueId;
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

inline void
LargeCircularSessionsStorage::Header::incrementBlockUniqueId(void)
{
  ++m_blockUniqueId;
}

inline void
LargeCircularSessionsStorage::Header::setNextSessionFirstBlockNumber(
    SessionBlockNumber_t nextSessionFirstBlockNumber)
{
  m_nextSessionFirstBlockNumber = nextSessionFirstBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::getNextSessionFirstBlockNumber(void)
{
  return m_nextSessionFirstBlockNumber;
}

inline std::size_t
LargeCircularSessionsStorage::Header::getSize(void)
{
  return (SIZE_OF_HEADER + HEADER_ALIGN) & ~HEADER_ALIGN;
}

// ---- External read/write methods -------------------------------------------

inline void
LargeCircularSessionsStorage::Header::writeMagic(Magic_t value,
    uint8_t* pHeader)
{
  util::endianness::BigEndian::writeUnsigned32(value, pHeader + OFFSET_OF_MAGIC);
}

inline LargeCircularSessionsStorage::Magic_t
LargeCircularSessionsStorage::Header::readMagic(const uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(pHeader + OFFSET_OF_MAGIC);
}

inline void
LargeCircularSessionsStorage::Header::writeSessionUniqueId(
    SessionUniqueId_t value, uint8_t* pHeader)
{
  util::endianness::BigEndian::writeUnsigned32(value,
      pHeader + OFFSET_OF_SESSIONUNIQEID);
}

inline LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Header::readSessionUniqueId(
    const uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(
      pHeader + OFFSET_OF_SESSIONUNIQEID);
}

inline void
LargeCircularSessionsStorage::Header::writeSessionFirstBlockNumber(
    SessionBlockNumber_t value, uint8_t* pHeader)
{
  util::endianness::BigEndian::writeUnsigned32(value,
      pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER);
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::readSessionFirstBlockNumber(
    const uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(
      pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER);
}

inline void
LargeCircularSessionsStorage::Header::writeBlockUniqueId(BlockUniqueId_t value,
    uint8_t* pHeader)
{
  util::endianness::BigEndian::writeUnsigned32(value,
      pHeader + OFFSET_OF_BLOCKUNIQUEID);
}

inline LargeCircularSessionsStorage::BlockUniqueId_t
LargeCircularSessionsStorage::Header::readBlockUniqueId(const uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(
      pHeader + OFFSET_OF_BLOCKUNIQUEID);
}

inline void
LargeCircularSessionsStorage::Header::writeNextSessionFirstBlockNumber(
    SessionBlockNumber_t value, uint8_t* pHeader)
{
  util::endianness::BigEndian::writeUnsigned32(value,
      pHeader + OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER);
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::readNextSessionFirstBlockNumber(
    const uint8_t* pHeader)
{
  return util::endianness::BigEndian::readUnsigned32(
      pHeader + OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER);
}

// ============================================================================

inline OSReturn_t
LargeCircularSessionsStorage::openStorage(void)
{
  return getDevice().open();
}

inline OSReturn_t
LargeCircularSessionsStorage::closeStorage(void)
{
  return getDevice().close();
}

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

// Get the length of the current session, in session blocks
inline LargeCircularSessionsStorage::SessionBlockCount_t
LargeCircularSessionsStorage::Reader::getSessionLength(void)
{
  return m_sessionLength;
}

// ============================================================================


#endif /* LARGECIRCULARSESSIONSSTORAGE_H_ */
