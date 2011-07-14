/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLE)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressable.h"

OSDeviceAddressable::OSDeviceAddressable()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceAddressable"), this);
}

OSDeviceAddressable::~OSDeviceAddressable()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceAddressable"), this);
}

OSReturn_t
OSDeviceAddressable::open(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::open()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceAddressable::Offset_t
OSDeviceAddressable::getDeviceSize(void)
{
  return 0; // No device
}

OSReturn_t
OSDeviceAddressable::readBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::readBytes()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceAddressable::writeBytes(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    uint8_t* pBuf __attribute__((unused)), OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::writeBytes()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceAddressable::Alignnment_t
OSDeviceAddressable::getWriteAlignment(void)
{
  return 512; // Default alignment size, from MMC specs
}

OSReturn_t
OSDeviceAddressable::close(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::close()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
OSDeviceAddressable::erase(OSDeviceAddressable::Offset_t offset __attribute__((unused)),
    OSDeviceAddressable::Count_t count __attribute__((unused)))
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::erase()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSDeviceAddressable::Alignnment_t
OSDeviceAddressable::getEraseAlignment(void)
{
  return 512; // Default alignment size, from MMC specs
}

OSReturn_t
OSDeviceAddressable::eraseEntireDevice(void)
{
  OSDeviceDebug::putString_P(PSTR("OSDeviceAddressable::eraseEntireDevice()"));
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_NOT_IMPLEMENTED;
}

#endif /* defined(OS_INCLUDE_OSDEVICEADDRESSABLE) */
