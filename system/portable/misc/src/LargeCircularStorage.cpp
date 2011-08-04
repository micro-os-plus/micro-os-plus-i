/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE)

#include "portable/misc/include/LargeCircularStorage.h"

// ----------------------------------------------------------------------------

LargeCircularStorageWriter::LargeCircularStorageWriter(OSDeviceBlock& device) :
  m_storage(device)
{
  debug.putConstructor_P(PSTR("LargeCircularStorage"), this);

  m_blockSizeBlocks = 1;
}

LargeCircularStorageWriter::~LargeCircularStorageWriter()
{
  debug.putDestructor_P(PSTR("LargeCircularStorage"), this);
}

OSReturn_t
LargeCircularStorageWriter::createSession(SessionUniqueId_t sessionUniqueId)
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

  return m_storage.open();
}

OSReturn_t
LargeCircularStorageWriter::closeSession(void)
{
  m_sessionUniqueId = 0;

  return m_storage.close();
}

OSReturn_t
LargeCircularStorageWriter::writeSessionBlock(uint8_t* pBuf)
{
  if (m_sessionUniqueId == 0)
    {
      return OSReturn::OS_NOT_INITIALIZED;
    }

  // Fill-in the block header, currently having three fields
  //    sessionUniqueId
  //    sessionFirstBlockNumber
  //    blockUniqueId

  uint8_t* p;
  p = (uint8_t*) pBuf + getReservedHeaderSize() - 1;

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

  if (m_currentBlockNumber + m_blockSizeBlocks
      > m_storage.getDeviceSizeBlocks())
    {
      // If there are not enough blocks to the end of the storage, roll over
      m_currentBlockNumber = 0;
    }

  // Write several blocks to the output device
  ret = m_storage.writeBlocks(m_currentBlockNumber, pBuf, m_blockSizeBlocks);

  // Increment the block number; when reaching device size, roll over
  m_currentBlockNumber += m_blockSizeBlocks;
  if (m_currentBlockNumber >= m_storage.getDeviceSizeBlocks())
    {
      m_currentBlockNumber = 0;
    }

  // Increase the monotone increasing unique block number
  ++m_blockUniqueId;

  return ret;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_LARGECIRCULARSTORAGE) */
