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

#endif /* defined(OS_INCLUDE_OSDEVICEADDRESSABLE) */
