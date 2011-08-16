/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE)

#include "portable/misc/include/LargeCircularSessionsStorage.h"

// ============================================================================

LargeCircularSessionsStorage::LargeCircularSessionsStorage(
    OSDeviceBlock& device) :
  m_device(device)
{
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage"), this);

  m_blockSizeBlocks = 1;
}

LargeCircularSessionsStorage::~LargeCircularSessionsStorage()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage"), this);
}

// ============================================================================

LargeCircularSessionsStorage::Writer::Writer(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage::Writer"), this);
}

LargeCircularSessionsStorage::Writer::~Writer()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage::Writer"), this);
}

OSReturn_t
LargeCircularSessionsStorage::Writer::createSession(
    SessionUniqueId_t sessionUniqueId)
{
  if (sessionUniqueId == 0)
    {
      return OSReturn::OS_BAD_PARAMETER;
    }

  m_sessionUniqueId = sessionUniqueId;

  // TODO: search for the next free block;
  // For the moment just start from the beginning
  // and with a dummy block unique id

  m_sessionFirstBlockNumber = 0;
  m_currentBlockNumber = 0;

  m_blockUniqueId = 0x12345678;

  return getStorage().getDevice().open();
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  m_sessionUniqueId = 0;

  return getStorage().getDevice().close();
}

OSReturn_t
LargeCircularSessionsStorage::Writer::writeSessionBlock(uint8_t* pBuf)
{
  if (m_sessionUniqueId == 0)
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Fill-in the block header, currently having three fields
  //    sessionUniqueId
  //    sessionFirstBlockNumber
  //    blockUniqueId

  uint8_t* p;
  p = (uint8_t*) pBuf + getStorage().getReservedHeaderSize() - 1;

  uint8_t b;
  uint_t i;

  BlockUniqueId_t blockUniqueId;
  blockUniqueId = m_blockUniqueId;

  for (i = 0; i < sizeof(BlockUniqueId_t); i++)
    {
      b = (uint8_t) blockUniqueId;
      *p-- = b;

      blockUniqueId >>= 8;
    }

  OSDeviceBlock::BlockNumber_t sessionFirstBlockNumber;
  sessionFirstBlockNumber = m_sessionFirstBlockNumber;

  for (i = 0; i < sizeof(OSDeviceBlock::BlockNumber_t); i++)
    {
      b = (uint8_t) sessionFirstBlockNumber;
      *p-- = b;

      sessionFirstBlockNumber >>= 8;
    }

  SessionUniqueId_t sessionUniqueId;
  sessionUniqueId = m_sessionUniqueId;

  for (i = 0; i < sizeof(SessionUniqueId_t); i++)
    {
      b = (uint8_t) sessionUniqueId;
      *p-- = b;

      sessionUniqueId >>= 8;
    }

  OSReturn_t ret;

  if (m_currentBlockNumber + getStorage().getSessionlBlockSizeBlocks()
      > getStorage().getDevice().getDeviceSizeBlocks())
    {
      // If there are not enough blocks to the end of the storage, roll over
      m_currentBlockNumber = 0;
    }

  // Write several blocks to the output device
  ret = getStorage().getDevice().writeBlocks(m_currentBlockNumber, pBuf,
      getStorage().getSessionlBlockSizeBlocks());

  // Increment the block number; when reaching device size, roll over
  m_currentBlockNumber += getStorage().getSessionlBlockSizeBlocks();
  if (m_currentBlockNumber >= getStorage().getDevice().getDeviceSizeBlocks())
    {
      m_currentBlockNumber = 0;
    }

  // Increase the monotone increasing unique block number
  ++m_blockUniqueId;

  return ret;
}

// ============================================================================

LargeCircularSessionsStorage::Reader::Reader(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage::Reader"), this);
}

LargeCircularSessionsStorage::Reader::~Reader()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage::Reader"), this);
}

// ----------------------------------------------------------------------------

// Searches for the given session
OSReturn_t
LargeCircularSessionsStorage::Reader::openSession(SessionUniqueId_t sessionId)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

// Without a session id, search for the most recent one
OSReturn_t
LargeCircularSessionsStorage::Reader::openMostRecentSession(void)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openPreviousSession(void)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

// Compute the length of the current session, in session blocks
LargeCircularSessionsStorage::SessionBlockCount_t
LargeCircularSessionsStorage::Reader::computeSessionLength(void)
{
  // TODO: implement
  return 0;
}

// Read the requested session block (minimum one device block)
// The block number should be between 0 and (sessionLength-1)
OSReturn_t
LargeCircularSessionsStorage::Reader::readSessionBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pBuf,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

// Read the next session block (minimum one device block)
OSReturn_t
LargeCircularSessionsStorage::Reader::readNextSessionBlock(uint8_t* pBuf,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::readPreviousSessionBlock(uint8_t* pBuf,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::close(void)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

// ============================================================================

#endif /* defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE) */
