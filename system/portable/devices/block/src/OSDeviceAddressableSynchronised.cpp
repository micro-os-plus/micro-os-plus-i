/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressableSynchronised.h"

#if false
OSDeviceAddressableSynchronised::OSDeviceAddressableSynchronised(
    OSDeviceAddressable& dev) :
  m_dev(dev)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceAddressableSynchronised"), this);
}
#else
OSDeviceAddressableSynchronised::OSDeviceAddressableSynchronised(
   )
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceAddressableSynchronised"), this);
}
#endif


OSDeviceAddressableSynchronised::~OSDeviceAddressableSynchronised()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceAddressableSynchronised"), this);
}

#endif /* defined(OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED) */
