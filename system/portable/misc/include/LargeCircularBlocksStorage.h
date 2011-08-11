/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARBLOCKSTORAGE_H_
#define LARGECIRCULARBLOCKSTORAGE_H_

#include "portable/kernel/include/uOS.h"

// ----------------------------------------------------------------------------

class LargeCircularBlocksStorageWriter
{
public:
  typedef uint32_t BlockUniqueId_t;

  LargeCircularBlocksStorageWriter(OSDeviceBlock& storage);
  ~LargeCircularBlocksStorageWriter();

  OSReturn_t
  open(void);

  OSReturn_t
  close(void);

  // Fill in the
  OSReturn_t
  writeBlock(uint8_t* pBuf);

  // Return the amount of space the application should reserve
  // at the beginning of each written block
  uint_t
  getReservedHeaderSize(void);

  OSDeviceBlock&
  getStorage(void);

  // set/get the number of physical blocks per logical block
  void
  setBlockSizeBlocks(OSDeviceBlock::BlockSize_t blockSizeBlocks);

  OSDeviceBlock::BlockSize_t
  getBlockSizeBlocks(void);

private:
  OSDeviceBlock& m_storage;

  OSDeviceBlock::BlockSize_t m_blockSizeBlocks;

  // A monotone increasing unique Id
  BlockUniqueId_t m_blockUniqueId;

  OSDeviceBlock::BlockNumber_t m_firstBlockNumber;
  OSDeviceBlock::BlockNumber_t m_currentBlockNumber;
};

inline uint_t
LargeCircularBlocksStorageWriter::getReservedHeaderSize(void)
{
  return sizeof(BlockUniqueId_t);
}

inline OSDeviceBlock&
LargeCircularBlocksStorageWriter::getStorage(void)
{
  return m_storage;
}

inline void
LargeCircularBlocksStorageWriter::setBlockSizeBlocks(
    OSDeviceBlock::BlockSize_t blockSizeBlocks)
{
  m_blockSizeBlocks = blockSizeBlocks;
}

inline OSDeviceBlock::BlockSize_t
LargeCircularBlocksStorageWriter::getBlockSizeBlocks(void)
{
  return m_blockSizeBlocks;
}

// ----------------------------------------------------------------------------

#endif /* LARGECIRCULARBLOCKSTORAGE_H_ */
