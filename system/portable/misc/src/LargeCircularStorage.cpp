/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE)

#include "portable/misc/include/LargeCircularStorage.h"

// ----------------------------------------------------------------------------

LargeCircularStorage::LargeCircularStorage(OSDeviceAddressable& dev) :
  m_dev(dev)
{
  debug.putConstructor_P(PSTR("GpsCoordinate"), this);
}

LargeCircularStorage::~LargeCircularStorage()
{
  debug.putDestructor_P(PSTR("GpsCoordinate"), this);
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_LARGECIRCULARSTORAGE) */
