/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LARGECIRCULARBLOCKSSSTORAGE)

#include "portable/misc/include/LargeCircularBlocksStorage.h"

// ----------------------------------------------------------------------------

LargeCircularBlocksStorageWriter::LargeCircularBlocksStorageWriter(
    OSDeviceBlock& device) :
  m_storage(device)
{
  debug.putConstructor_P(PSTR("LargeCircularBlocksStorageWriter"), this);

  m_blockSizeBlocks = 1;
}

LargeCircularBlocksStorageWriter::~LargeCircularBlocksStorageWriter()
{
  debug.putDestructor_P(PSTR("LargeCircularBlocksStorageWriter"), this);
}

OSReturn_t
LargeCircularBlocksStorageWriter::open(void)
{
  // TODO: search for the next free block;
  // For the moment just start from the beginning
  // and with a dummy block unique id

  m_currentBlockNumber = 0;

  m_blockUniqueId = 0x12345678;

  return m_storage.open();
}

OSReturn_t
LargeCircularBlocksStorageWriter::close(void)
{
  return m_storage.close();
}

OSReturn_t
LargeCircularBlocksStorageWriter::writeBlock(uint8_t* pBuf)
{
  // Fill-in the block header, currently having one field
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
  if (m_currentBlockNumber == m_storage.getDeviceSizeBlocks())
    {
      m_currentBlockNumber = 0;
    }

  // Increase the monotone increasing unique block number
  ++m_blockUniqueId;

  return ret;
}

#endif /* defined(OS_INCLUDE_LARGECIRCULARBLOCKSSSTORAGE) */

// ----------------------------------------------------------------------------
