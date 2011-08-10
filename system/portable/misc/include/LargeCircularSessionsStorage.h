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

class LargeCircularSessionsStorageWriter
{
public:
#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE_LARGE_SESSIONID)
  typedef uint64_t SessionUniqueId_t;
#else
  typedef uint32_t SessionUniqueId_t;
#endif

  typedef uint32_t BlockUniqueId_t;

  LargeCircularSessionsStorageWriter(OSDeviceBlock& storage);
  ~LargeCircularSessionsStorageWriter();

  OSReturn_t
  createSession(SessionUniqueId_t sessionId);

  // Fill in the
  OSReturn_t
  writeSessionBlock(uint8_t* pBuf);

  // Return the amount of space the application should reserve
  // at the beginning of each written block
  uint_t
  getReservedHeaderSize(void);

  OSReturn_t
  closeSession(void);

  OSDeviceBlock&
  getStorage(void);

  // set/get the number of physical blocks per logical block
  void
  setSessionBlockSizeBlocks(OSDeviceBlock::BlockSize_t blockSizeBlocks);

  OSDeviceBlock::BlockSize_t
  getSessionlBlockSizeBlocks(void);

private:
  OSDeviceBlock& m_storage;

  OSDeviceBlock::BlockSize_t m_blockSizeBlocks;

  // An application supplied unique id for identifying sessions
  SessionUniqueId_t m_sessionUniqueId;
  // A monotone increasing unique Id
  BlockUniqueId_t m_blockUniqueId;

  OSDeviceBlock::BlockNumber_t m_sessionFirstBlockNumber;
  OSDeviceBlock::BlockNumber_t m_currentBlockNumber;
};

inline uint_t
LargeCircularSessionsStorageWriter::getReservedHeaderSize(void)
{
  return sizeof(SessionUniqueId_t) + sizeof(OSDeviceBlock::BlockNumber_t)
      + sizeof(BlockUniqueId_t);
}

inline OSDeviceBlock&
LargeCircularSessionsStorageWriter::getStorage(void)
{
  return m_storage;
}

inline void
LargeCircularSessionsStorageWriter::setSessionBlockSizeBlocks(
    OSDeviceBlock::BlockSize_t blockSizeBlocks)
{
  m_blockSizeBlocks = blockSizeBlocks;
}

inline OSDeviceBlock::BlockSize_t
LargeCircularSessionsStorageWriter::getSessionlBlockSizeBlocks(void)
{
  return m_blockSizeBlocks;
}

// ----------------------------------------------------------------------------

#endif /* LARGECIRCULARSESSIONSSTORAGE_H_ */
