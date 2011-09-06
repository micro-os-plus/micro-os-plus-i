/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEBLOCKPARTITION)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlockPartition.h"

// ----- Constructors & destructors -------------------------------------------

OSDeviceBlockPartition::OSDeviceBlockPartition(OSDeviceBlock& parent) :
  OSDeviceBlockExtension(parent)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceBlockPartition"), this);

  m_partitionOffset = 0;
  m_partitionSizeBlocks = 0;
}

OSDeviceBlockPartition::~OSDeviceBlockPartition()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceBlockPartition"), this);
}

// ----- Public virtual methods -----------------------------------------------

OSReturn_t
OSDeviceBlockPartition::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t blockCount)
{
  if (blockNumber + blockCount > m_partitionSizeBlocks)
    return OSReturn::OS_SIZE_EXCEEDED;

  // Adjust blockNumber with partition offset
  return m_parent.readBlocks(blockNumber + m_partitionOffset, pBuf, blockCount);
}

// Write the count blocks, from the given buffer.
OSReturn_t
OSDeviceBlockPartition::writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t blockCount)
{
  if (blockNumber + blockCount > m_partitionSizeBlocks)
    return OSReturn::OS_SIZE_EXCEEDED;

  // Adjust blockNumber with partition offset
  return m_parent.writeBlocks(blockNumber + m_partitionOffset, pBuf, blockCount);
}

// Erase part of the device.
OSReturn_t
OSDeviceBlockPartition::eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    OSDeviceBlock::BlockCount_t blockCount)
{
  if (blockNumber + blockCount > m_partitionSizeBlocks)
    return OSReturn::OS_SIZE_EXCEEDED;

  // Adjust blockNumber with partition offset
  return m_parent.eraseBlocks(blockNumber + m_partitionOffset, blockCount);
}

OSDeviceBlock::BlockNumber_t
OSDeviceBlockPartition::getDeviceSizeBlocks(void)
{
  return m_partitionSizeBlocks;
}

// ----- Public local methods -------------------------------------------------

OSReturn_t
OSDeviceBlockPartition::setPartitioneOffsetAndSize(
    OSDeviceBlock::BlockNumber_t offset, OSDeviceBlock::BlockNumber_t size)
{
  OSDeviceDebug::putString("setPartitioneOffsetAndSize(");
  OSDeviceDebug::putDec(offset);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(size);
  OSDeviceDebug::putChar(')');
  OSDeviceDebug::putNewLine();

  m_partitionOffset = offset;
  m_partitionSizeBlocks = size;

  return OSReturn::OS_OK;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSDEVICEBLOCKPARTITION) */
