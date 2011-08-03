/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARSTORAGE_H_
#define LARGECIRCULARSTORAGE_H_

#include "portable/kernel/include/uOS.h"

// ----------------------------------------------------------------------------

class LargeCircularStorageWriter
{
public:
#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE_LARGE_SESSIONID)
  typedef uint64_t SessionUniqueId_t;
#else
  typedef uint32_t SessionUniqueId_t;
#endif

  typedef uint32_t BlockUniqueId_t;

  LargeCircularStorageWriter(OSDeviceBlock& device);
  ~LargeCircularStorageWriter();

  OSReturn_t
  createSession(SessionUniqueId_t sessionId);

  // Fill in the
  OSReturn_t
  writeBlock(uint8_t* pBuf);

  // Return the amount of space the application should reserve
  // at the beginning of each written block
  uint_t
  getReservedHeaderSize(void);

  OSReturn_t
  closeSession(void);

private:
  OSDeviceBlock& m_device;

  // An application supplied unique id for identifying sessions
  SessionUniqueId_t m_sessionUniqueId;
  // A monotone increasing unique Id
  BlockUniqueId_t m_blockUniqueId;

  OSDeviceBlock::BlockNumber_t m_sessionFirstBlockNumber;
  OSDeviceBlock::BlockNumber_t m_currentBlockNumber;
};

inline uint_t
LargeCircularStorageWriter::getReservedHeaderSize(void)
{
  return sizeof(SessionUniqueId_t) + sizeof(OSDeviceBlock::BlockNumber_t)
      + sizeof(BlockUniqueId_t);
}

// ----------------------------------------------------------------------------

#endif /* LARGECIRCULARSTORAGE_H_ */
