/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARSESSIONSSTORAGE_H_
#define LARGECIRCULARSESSIONSSTORAGE_H_

#include "portable/kernel/include/uOS.h"

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
// ----------------------------------------------------------------------------

class LargeCircularSessionsStorage
{
public:

  // ----- Public type definitions --------------------------------------------

#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE_LARGE_SESSIONID)
  typedef uint64_t SessionUniqueId_t;
#else
  typedef uint32_t SessionUniqueId_t;
#endif

  typedef uint32_t BlockUniqueId_t;

  typedef uint32_t SessionBlockNumber_t;
  typedef uint32_t SessionBlockCount_t;

  // ----- Constructors & Destructors -----------------------------------------

  LargeCircularSessionsStorage(OSDeviceBlock& device);
  ~LargeCircularSessionsStorage();

  // ----- Public methods -----------------------------------------------------

  // Common (writer & reader) methods

  // Return the amount of space the application should reserve
  // at the beginning of each written block, or the amount of space the
  // reader should skip
  uint_t
  getReservedHeaderSize(void);

  OSDeviceBlock&
  getDevice(void);

  // set/get the number of physical blocks per logical block
  void
  setSessionBlockSizeBlocks(OSDeviceBlock::BlockSize_t blockSizeBlocks);

  OSDeviceBlock::BlockSize_t
  getSessionlBlockSizeBlocks(void);

private:
  OSDeviceBlock& m_device;

  OSDeviceBlock::BlockSize_t m_blockSizeBlocks;

public:
  // Writer
  class Writer
  {
  public:

    // ----- Constructors & Destructors ---------------------------------------

    Writer(LargeCircularSessionsStorage& storage);
    ~Writer();

    // ----- Public methods ---------------------------------------------------

    LargeCircularSessionsStorage&
    getStorage(void);

    OSReturn_t
    createSession(SessionUniqueId_t sessionId);

    // Fill in the
    OSReturn_t
    writeSessionBlock(uint8_t* pBuf);

    OSReturn_t
    closeSession(void);

  private:
    LargeCircularSessionsStorage& m_storage;

    // An application supplied unique id for identifying sessions
    SessionUniqueId_t m_sessionUniqueId;
    // A monotone increasing unique Id
    BlockUniqueId_t m_blockUniqueId;

    OSDeviceBlock::BlockNumber_t m_sessionFirstBlockNumber;
    OSDeviceBlock::BlockNumber_t m_currentBlockNumber;
  };

  class Reader
  {
  public:

    // ----- Constructors & Destructors ---------------------------------------

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
    openPreviousSession(void);

    // Compute the length of the current session, in session blocks
    SessionBlockCount_t
    computeSessionLength(void);

    // Read the requested session block (minimum one device block)
    // The block number should be between 0 and (sessionLength-1)
    OSReturn_t
    readSessionBlock(SessionBlockNumber_t blockNumber, uint8_t* pBuf,
        OSDeviceBlock::BlockCount_t deviceBlocksCount);

    // Read the next session block (minimum one device block)
    OSReturn_t
    readNextSessionBlock(uint8_t* pBuf,
        OSDeviceBlock::BlockCount_t deviceBlocksCount);

    // Read the previous session block (minimum one device block)
    OSReturn_t
    readPreviousSessionBlock(uint8_t* pBuf,
        OSDeviceBlock::BlockCount_t deviceBlocksCount);

    OSReturn_t
    close(void);

  protected:
    LargeCircularSessionsStorage& m_storage;

  };

};

// ============================================================================

inline uint_t
LargeCircularSessionsStorage::getReservedHeaderSize(void)
{
  return sizeof(SessionUniqueId_t) + sizeof(OSDeviceBlock::BlockNumber_t)
      + sizeof(BlockUniqueId_t);
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
LargeCircularSessionsStorage::getSessionlBlockSizeBlocks(void)
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
