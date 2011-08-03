/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEBLOCKPARTITION_H_
#define OSDEVICEBLOCKPARTITION_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlockExtension.h"

class OSDeviceBlockPartition : public OSDeviceBlockExtension
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSDeviceBlockPartition(OSDeviceBlock& parent);
  virtual
  ~OSDeviceBlockPartition();

  // ----- Public virtual methods ---------------------------------------------

  // Read exactly count blocks, in the given buffer.
  virtual OSReturn_t
  readBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Write the count blocks, from the given buffer.
  virtual OSReturn_t
  writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Erase part of the device.
  virtual OSReturn_t
  eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

  // Return the full size, in blocks, of the device.
  virtual OSDeviceBlock::BlockNumber_t
  getDeviceSize(void);

  // ----- Public local methods -----------------------------------------------

  // Store partition start and size.
  // Must be called before any read/write/erase methods.
  OSReturn_t
  setPartitioneOffsetAndSize(OSDeviceBlock::BlockNumber_t offset,
      OSDeviceBlock::BlockNumber_t size);

  OSDeviceBlock::BlockNumber_t
  getPartitionOffset(void);

  OSDeviceBlock::BlockNumber_t
  getPartitionSize(void);

private:

  // ----- Members ------------------------------------------------------------

  // Both offset and size are in blocks.
  OSDeviceBlock::BlockNumber_t m_partitionOffset;
  OSDeviceBlock::BlockNumber_t m_partitionSize;

  // --------------------------------------------------------------------------

};

inline OSDeviceBlock::BlockNumber_t
OSDeviceBlockPartition::getPartitionOffset(void)
{
  return m_partitionOffset;
}

inline OSDeviceBlock::BlockNumber_t
OSDeviceBlockPartition::getPartitionSize(void)
{
  return m_partitionSize;
}

#endif /* OSDEVICEADDRESSABLEPARTITION_H_ */
