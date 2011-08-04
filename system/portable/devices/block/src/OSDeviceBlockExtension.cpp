/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEBLOCKEXTENSION)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlockExtension.h"

// ----- Constructors & destructors -------------------------------------------

OSDeviceBlockExtension::OSDeviceBlockExtension(OSDeviceBlock& parent) :
  m_parent(parent)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceBlockExtension"), this);
}

OSDeviceBlockExtension::~OSDeviceBlockExtension()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceBlockExtension"), this);
}

// ---- Public methods --------------------------------------------------------

OSReturn_t
OSDeviceBlockExtension::open(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlockExtension::open()"));
  OSDeviceDebug::putNewLine();

  return m_parent.open();
}

OSReturn_t
OSDeviceBlockExtension::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlockExtension::readBytes()"));
  OSDeviceDebug::putNewLine();

  return m_parent.readBlocks(blockNumber, pBuf, count);
}

OSReturn_t
OSDeviceBlockExtension::writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlockExtension::writeBytes()"));
  OSDeviceDebug::putNewLine();

  return m_parent.writeBlocks(blockNumber, pBuf, count);
}

OSReturn_t
OSDeviceBlockExtension::close(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlockExtension::close()"));
  OSDeviceDebug::putNewLine();

  return m_parent.close();
}

OSReturn_t
OSDeviceBlockExtension::eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlockExtension::erase()"));
  OSDeviceDebug::putNewLine();

  return m_parent.eraseBlocks(blockNumber, count);
}

OSDeviceBlock::BlockNumber_t
OSDeviceBlockExtension::getDeviceSizeBlocks(void)
{
  return m_parent.getDeviceSizeBlocks();
}

// Return the device block size, in bytes.
OSDeviceBlock::BlockSize_t
OSDeviceBlockExtension::getBlockSizeBytes(void)
{
  return m_parent.getBlockSizeBytes();
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSDEVICEBLOCKEXTENSION) */
