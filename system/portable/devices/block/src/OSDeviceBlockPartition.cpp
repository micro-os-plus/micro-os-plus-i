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
  m_partitionSize = 0;
}

OSDeviceBlockPartition::~OSDeviceBlockPartition()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceBlockPartition"), this);
}

// ----- Public virtual methods -----------------------------------------------

OSReturn_t
OSDeviceBlockPartition::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t count)
{
  // Adjust blockNumber with partition offset
  return m_parent.readBlocks(blockNumber + m_partitionOffset, pBuf, count);
}

// Write the count blocks, from the given buffer.
OSReturn_t
OSDeviceBlockPartition::writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t count)
{
  // Adjust blockNumber with partition offset
  return m_parent.writeBlocks(blockNumber + m_partitionOffset, pBuf, count);
}

// Erase part of the device.
OSReturn_t
OSDeviceBlockPartition::eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    OSDeviceBlock::BlockCount_t count)
{
  // Adjust blockNumber with partition offset
  return m_parent.eraseBlocks(blockNumber + m_partitionOffset, count);
}

OSDeviceBlock::BlockNumber_t
OSDeviceBlockPartition::getDeviceSizeBlocks(void)
{
  return m_partitionSize;
}

// ----- Public local methods -------------------------------------------------

OSReturn_t
OSDeviceBlockPartition::setPartitioneOffsetAndSize(
    OSDeviceBlock::BlockNumber_t offset, OSDeviceBlock::BlockNumber_t size)
{
  m_partitionOffset = offset;
  m_partitionSize = size;

  return OSReturn::OS_OK;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSDEVICEBLOCKPARTITION) */
