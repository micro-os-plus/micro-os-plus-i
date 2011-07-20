/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEBLOCKSYNCHRONISED)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlockSynchronised.h"

// ----- Constructors & destructors -------------------------------------------

OSDeviceBlockSynchronised::OSDeviceBlockSynchronised(OSDeviceBlock& parent,
    OSMutex& mutex) :
  OSDeviceBlockExtension(parent), m_mutex(mutex)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceBlockSynchronised"), this);
}

OSDeviceBlockSynchronised::~OSDeviceBlockSynchronised()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceBlockSynchronised"), this);
}

// ----- Public virtual methods -----------------------------------------------

// Synchronised read of exactly count blocks, in the given buffer.
OSReturn_t
OSDeviceBlockSynchronised::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
    uint8_t* pBuf, OSDeviceBlock::BlockCount_t count)
{
  OSReturn_t ret;
  m_mutex.acquire();
    {
      ret = m_parent.readBlocks(blockNumber, pBuf, count);
    }
  m_mutex.release();

  return ret;
}

// Synchronised write the count blocks, from the given buffer.
OSReturn_t
OSDeviceBlockSynchronised::writeBlocks(
    OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
    OSDeviceBlock::BlockCount_t count)
{
  OSReturn_t ret;
  m_mutex.acquire();
    {
      ret = m_parent.writeBlocks(blockNumber, pBuf, count);
    }
  m_mutex.release();

  return ret;
}

// Synchronised erase part of the device.
OSReturn_t
OSDeviceBlockSynchronised::eraseBlocks(
    OSDeviceBlock::BlockNumber_t blockNumber, OSDeviceBlock::BlockCount_t count)
{
  OSReturn_t ret;
  m_mutex.acquire();
    {
      ret = m_parent.eraseBlocks(blockNumber, count);
    }
  m_mutex.release();

  return ret;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSDEVICEBLOCKSYNCHRONISED) */
