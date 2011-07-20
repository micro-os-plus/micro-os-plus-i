/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEBLOCK)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlock.h"

OSDeviceBlock::OSDeviceBlock()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceBlock"), this);
}

OSDeviceBlock::~OSDeviceBlock()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceBlock"), this);
}

OSReturn_t
OSDeviceBlock::open(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlock::open()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceBlock::BlockNumber_t
OSDeviceBlock::getDeviceSize(void)
{
  return 0; // No device
}

OSReturn_t
OSDeviceBlock::readBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlock::readBytes()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceBlock::writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlock::writeBytes()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceBlock::close(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlock::close()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceBlock::eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber __attribute__((unused)),
    OSDeviceBlock::BlockCount_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceBlock::erase()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

#endif /* defined(OS_INCLUDE_OSDEVICEBLOCK) */
