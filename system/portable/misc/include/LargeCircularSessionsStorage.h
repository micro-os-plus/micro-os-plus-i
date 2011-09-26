/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARSESSIONSSTORAGE_H_
#define LARGECIRCULARSESSIONSSTORAGE_H_

#include "portable/kernel/include/uOS.h"

#include "portable/util/endianness/include/BigEndian.h"
#include "portable/kernel/include/OSEventNotifier.h"
#include "portable/kernel/include/OSCondition.h"

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

  const static SessionUniqueId_t OLDEST_SESSSION_UNIQUE_ID = 0;
  const static SessionUniqueId_t NEWEST_SESSSION_UNIQUE_ID = ~0;

  const static SessionUniqueId_t NEXT_SESSSION_UNIQUE_ID = 0;

  typedef uint32_t BlockUniqueId_t;
  const static BlockUniqueId_t BEGINING_BLOCK_UNIQUE_ID = 1;

  typedef uint32_t SessionBlockNumber_t;
  typedef uint32_t SessionBlockCount_t;

  // This inner class handles the session block header fields.
  // In addition to regular getters/setters, it has methods to read/write
  // header fields at proper byte positions.

  class Header
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Header();
    ~Header();

    // ----- Public methods ---------------------------------------------------

    // Used to copy all fields from one object to another
    //Header&
    //operator=(const Header& header);

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

    bool
    isMagicValid(void);

    void
    setSessionUniqueId(SessionUniqueId_t sessionUniqueId);
    SessionUniqueId_t
    getSessionUniqueId(void);

    //void
    //incrementUniqueId(void);

    void
    setSessionFirstBlockNumber(SessionBlockNumber_t sessionFirstBlockNumber);
    SessionBlockNumber_t
    getSessionFirstBlockNumber(void);

    void
    setBlockUniqueId(BlockUniqueId_t blockUniqueId);
    BlockUniqueId_t
    getBlockUniqueId(void);

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

    const static std::size_t INVALID_BLOCKNUMBER = ~0;

  private:

    // ----- Private members --------------------------------------------------

    Magic_t m_magic;
    SessionUniqueId_t m_sessionUniqueId;
    SessionBlockNumber_t m_sessionFirstBlockNumber;
    SessionBlockNumber_t m_nextSessionFirstBlockNumber;
    BlockUniqueId_t m_blockUniqueId;

    // Offsets when stored in a block header.
    const static std::size_t OFFSET_OF_MAGIC = 0;
    const static std::size_t OFFSET_OF_SESSIONUNIQEID = OFFSET_OF_MAGIC
        + sizeof(Magic_t);
    const static std::size_t OFFSET_OF_SESSIONFIRSTBLOCKNUMBER =
        OFFSET_OF_SESSIONUNIQEID + sizeof(SessionUniqueId_t);
    const static std::size_t OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER =
        OFFSET_OF_SESSIONFIRSTBLOCKNUMBER + sizeof(SessionBlockNumber_t);
    const static std::size_t OFFSET_OF_BLOCKUNIQUEID =
        OFFSET_OF_NEXTSESSIONFIRSTBLOCKNUMBER + sizeof(SessionBlockNumber_t);

    const static std::size_t SIZE_OF_HEADER = OFFSET_OF_BLOCKUNIQUEID
        + sizeof(BlockUniqueId_t);

    // Mainly for aesthetic reasons, we start the header payload on the
    // next 16 byte border.
    const static int HEADER_ALIGN = (16 - 1);

    // Unused header space is filled with this value.
    const static uint8_t UNUSED_SPACE_FILL_BYTE = 0xFF;

    // ------------------------------------------------------------------------
  };

  class Session
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Session();
    ~Session();

    // ----- Public methods ---------------------------------------------------

    // Used to copy all fields from one object to another
    //Session&
    //operator=(const Session& session);

    //Session&
    //operator=(const Header& header);

    bool
    isCompletelyWritten(void);

    bool
    isValid(void);

    void
    setUniqueId(SessionUniqueId_t sessionUniqueId);
    SessionUniqueId_t
    getUniqueId(void);

    void
    incrementUniqueId(void);

    void
    setFirstBlockNumber(SessionBlockNumber_t sessionFirstBlockNumber);
    SessionBlockNumber_t
    getFirstBlockNumber(void);

    void
    setLastBlockNumber(SessionBlockNumber_t sessionLastBlockNumber);
    SessionBlockNumber_t
    getLastBlockNumber(void);

    void
    setLength(SessionBlockNumber_t sessionLength);
    SessionBlockNumber_t
    getLength(void);

    void
    setNextSessionFirstBlockNumber(
        SessionBlockNumber_t nextSessionFirstBlockNumber);
    SessionBlockNumber_t
    getNextSessionFirstBlockNumber(void);

    bool
    isForwardReferenceValid(void);

  private:

    // ----- Private members --------------------------------------------------

    // Valid always
    SessionUniqueId_t m_sessionUniqueId;

    // INVALID_BLOCKNUMBER if not initialised
    SessionBlockNumber_t m_sessionFirstBlockNumber;

    // INVALID_BLOCKNUMBER if unknown
    SessionBlockNumber_t m_sessionLastBlockNumber;

    // 0 if unknown
    SessionBlockNumber_t m_sessionLength;

    // INVALID_BLOCKNUMBER if unknown
    SessionBlockNumber_t m_nextSessionFirstBlockNumber;
  };

  class SessionBlock
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    SessionBlock();
    ~SessionBlock();

    // ----- Public methods ---------------------------------------------------

    void
    setUniqueId(BlockUniqueId_t blockUniqueId);

    BlockUniqueId_t
    getUniqueId(void);

    void
    incrementUniqueId(void);

    void
    setBlockNumber(SessionBlockNumber_t sessionBlockNumber);

    SessionBlockNumber_t
    getBlockNumber(void);

  private:

    // ----- Private members --------------------------------------------------

    BlockUniqueId_t m_blockUniqueId;
    SessionBlockNumber_t m_blockNumber;
  };

  // ----- Constructors & destructors -----------------------------------------

  LargeCircularSessionsStorage(OSDeviceBlock& device,
      OSEventNotifier* pEventNotifier = NULL);
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

  // Required after erasing the storage, to force open to reread session
  void
  clearCachedValues(void);

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

  bool
  isWriting(void);

  void
  setIsWriting(bool flag);

  Session&
  getMostRecentlyWrittenSession(void);

  SessionBlock&
  getMostRecentlyWrittenSessionBlock(void);

  void
  setEventNotifier(OSEventNotifier* pEventNotifier);

  OSEventNotifier*
  getEventNotifier(void);

private:

  // All new sessions are created just after the most recent existing one.
  // This requires a smart scan of the storage, to identify the
  // place where a major discontinuity in the block unique id is found.
  // Normally the block unique id is incremented after each write, and since
  // the total number of blocks in the storage is significantly lower than
  // the range of this counter, it can be used to identify more easily
  // the discontinuity.
  OSReturn_t
  searchMostRecentlyWrittenBlock(uint8_t* pBlockBuffer,
      LargeCircularSessionsStorage::SessionBlock& block,
      LargeCircularSessionsStorage::Session& session);

  // Adjust the block number with a positive or negative adjustment,
  // also handling roll over, on both ends of the circular buffer
  SessionBlockNumber_t
  computeCircularSessionBlockNumber(SessionBlockNumber_t blockNumber,
      int adjustment);

  SessionBlockNumber_t
  computeCircularSessionLength(SessionBlockNumber_t last,
      SessionBlockNumber_t first);

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

  Session m_mostRecentlyWrittenSession;
  SessionBlock m_mostRecentlyWrittenSessionBlock;

  bool m_isWriting;

  OSEvent_t m_event;

  OSEventNotifier* m_pEventNotifier;

public:

  OSEvent_t
  getEvent(void);

  void
  setEvent(OSEvent_t event);

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
    // If the sessionId is NEXT_SESSSION_UNIQUE_ID, just use the next id.
    OSReturn_t
    createSession(SessionUniqueId_t sessionUniqueId = NEXT_SESSSION_UNIQUE_ID);

    SessionUniqueId_t
    getSessionUniqueId(void);

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

    void
    notifyReaders(void);

  private:

    // ----- Private members --------------------------------------------------

    LargeCircularSessionsStorage& m_storage;

    Session m_currentSession;
    SessionBlock m_currentBlock;

    // temporary buffer used for search and update forward references
    uint8_t m_blockBuffer[512] __attribute__((aligned(4)));

    // ------------------------------------------------------------------------
  };

  class Reader
  {
  public:

    // ----- Constructors & destructors ---------------------------------------

    Reader(LargeCircularSessionsStorage& storage);
    ~Reader();

    class ReadCondition : public OSCondition
    {
    public:

      ReadCondition(Reader& reader);
      ~ReadCondition();

      virtual OSReturn_t
      prepareCheckCondition(void);

      virtual OSReturn_t
      checkSynchronisedCondition(void);

      void
      setSessionBlockNumber(SessionBlockNumber_t blockNumber);

      Reader&
      getReader(void);

    private:

      // Object that needs the synchronisation
      Reader& m_reader;

      // Input parameter
      SessionBlockNumber_t m_sessionBlockNumber;
    };

    // ----- Public methods ---------------------------------------------------

    LargeCircularSessionsStorage&
    getStorage(void);

    // Searches for the given session.
    // Special cases are
    //  OLDEST_SESSSION_UNIQUE_ID - open the oldest session available
    //  NEWEST_SESSSION_UNIQUE_ID - open the most recently written session
    OSReturn_t
    openSession(SessionUniqueId_t sessionId, bool doNotBlock = false);

    // Search for the most recent session.
    OSReturn_t
    openMostRecentSession(void);

#if false
    // Search for the oldest session still available in the storage.
    OSReturn_t
    openOldestSession(void);
#endif

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
    readSessionBlock(SessionBlockNumber_t sessionBlockNumber,
        OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlock,
        bool doNotBlock = false);

    // Mainly for completeness, not much to do.
    OSReturn_t
    closeSession(void);

    Session&
    getCurrentSession(void);

  protected:

    // ----- Private members --------------------------------------------------

    LargeCircularSessionsStorage& m_storage;

    Session m_currentSession;
    SessionBlock m_currentBlock;

    ReadCondition m_readSessionBlockCondition;

    // temporary buffer
    uint8_t m_blockBuffer[512] __attribute__((aligned(4)));

    // ------------------------------------------------------------------------
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

inline bool
LargeCircularSessionsStorage::Header::isMagicValid(void)
{
  return (m_magic == MAGIC);
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

#if false

inline void
LargeCircularSessionsStorage::Header::incrementBlockUniqueId(void)
  {
    ++m_blockUniqueId;
  }

#endif

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

inline bool
LargeCircularSessionsStorage::Session::isCompletelyWritten(void)
{
  return (m_sessionLength > 0);
}

inline bool
LargeCircularSessionsStorage::Session::isValid(void)
{
  return (m_sessionFirstBlockNumber != Header::INVALID_BLOCKNUMBER);
}

inline void
LargeCircularSessionsStorage::Session::setUniqueId(
    SessionUniqueId_t sessionUniqueId)
{
  m_sessionUniqueId = sessionUniqueId;
}

inline LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Session::getUniqueId(void)
{
  return m_sessionUniqueId;
}

inline void
LargeCircularSessionsStorage::Session::incrementUniqueId(void)
{
  ++m_sessionUniqueId;
}

inline void
LargeCircularSessionsStorage::Session::setFirstBlockNumber(
    SessionBlockNumber_t sessionFirstBlockNumber)
{
  m_sessionFirstBlockNumber = sessionFirstBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Session::getFirstBlockNumber(void)
{
  return m_sessionFirstBlockNumber;
}

inline void
LargeCircularSessionsStorage::Session::setLastBlockNumber(
    SessionBlockNumber_t sessionLastBlockNumber)
{
  m_sessionLastBlockNumber = sessionLastBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Session::getLastBlockNumber(void)
{
  return m_sessionLastBlockNumber;
}

inline void
LargeCircularSessionsStorage::Session::setLength(
    SessionBlockNumber_t sessionLength)
{
  m_sessionLength = sessionLength;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Session::getLength(void)
{
  return m_sessionLength;
}

inline void
LargeCircularSessionsStorage::Session::setNextSessionFirstBlockNumber(
    SessionBlockNumber_t nextSessionFirstBlockNumber)
{
  m_nextSessionFirstBlockNumber = nextSessionFirstBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Session::getNextSessionFirstBlockNumber(void)
{
  return m_nextSessionFirstBlockNumber;
}

inline bool
LargeCircularSessionsStorage::Session::isForwardReferenceValid(void)
{
  return (m_nextSessionFirstBlockNumber != Header::INVALID_BLOCKNUMBER);
}

// ============================================================================

inline void
LargeCircularSessionsStorage::SessionBlock::setUniqueId(
    BlockUniqueId_t blockUniqueId)
{
  m_blockUniqueId = blockUniqueId;
}

inline LargeCircularSessionsStorage::BlockUniqueId_t
LargeCircularSessionsStorage::SessionBlock::getUniqueId(void)
{
  return m_blockUniqueId;
}

inline void
LargeCircularSessionsStorage::SessionBlock::incrementUniqueId(void)
{
  ++m_blockUniqueId;
}

inline void
LargeCircularSessionsStorage::SessionBlock::setBlockNumber(
    SessionBlockNumber_t sessionBlockNumber)
{
  m_blockNumber = sessionBlockNumber;
}

inline LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::SessionBlock::getBlockNumber(void)
{
  return m_blockNumber;
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

inline bool
LargeCircularSessionsStorage::isWriting(void)
{
  return m_isWriting;
}

inline void
LargeCircularSessionsStorage::setIsWriting(bool flag)
{
  m_isWriting = flag;
}

inline LargeCircularSessionsStorage::Session&
LargeCircularSessionsStorage::getMostRecentlyWrittenSession(void)
{
  return m_mostRecentlyWrittenSession;
}

inline LargeCircularSessionsStorage::SessionBlock&
LargeCircularSessionsStorage::getMostRecentlyWrittenSessionBlock(void)
{
  return m_mostRecentlyWrittenSessionBlock;
}

inline OSEvent_t
LargeCircularSessionsStorage::getEvent(void)
{
  return m_event;
}

inline void
LargeCircularSessionsStorage::setEvent(OSEvent_t event)
{
  m_event = event;
}

inline void
LargeCircularSessionsStorage::setEventNotifier(OSEventNotifier* pEventNotifier)
{
  m_pEventNotifier = pEventNotifier;
}

inline OSEventNotifier*
LargeCircularSessionsStorage::getEventNotifier(void)
{
  return m_pEventNotifier;
}

// ============================================================================

inline LargeCircularSessionsStorage&
LargeCircularSessionsStorage::Writer::getStorage(void)
{
  return m_storage;
}

inline LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Writer::getSessionUniqueId(void)
{
  return m_currentSession.getUniqueId();
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
  return m_currentSession.getLength();
}

inline LargeCircularSessionsStorage::Session&
LargeCircularSessionsStorage::Reader::getCurrentSession(void)
{
  return m_currentSession;
}

// ============================================================================

inline void
LargeCircularSessionsStorage::Reader::ReadCondition::setSessionBlockNumber(
    LargeCircularSessionsStorage::SessionBlockNumber_t blockNumber)
{
  m_sessionBlockNumber = blockNumber;
}

inline LargeCircularSessionsStorage::Reader&
LargeCircularSessionsStorage::Reader::ReadCondition::getReader(void)
{
  return m_reader;
}

// ============================================================================


#endif /* LARGECIRCULARSESSIONSSTORAGE_H_ */
