/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLEPARTITION)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressablePartition.h"

OSDeviceAddressablePartition::OSDeviceAddressablePartition(
    OSDeviceAddressableSynchronised& dev) :
  m_dev(dev)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceAddressablePartition"), this);
}

OSDeviceAddressablePartition::~OSDeviceAddressablePartition()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSDeviceAddressablePartition"), this);
}

#endif /* defined(OS_INCLUDE_OSDEVICEADDRESSABLEPARTITION) */
