/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEADDRESSABLEPARTITION_H_
#define OSDEVICEADDRESSABLEPARTITION_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressableSynchronised.h"

class OSDeviceAddressablePartition : public OSDeviceAddressable
{
public:
  OSDeviceAddressablePartition(OSDeviceAddressableSynchronised& dev);
  virtual
  ~OSDeviceAddressablePartition();

private:
  OSDeviceAddressableSynchronised& m_dev;
};

#endif /* OSDEVICEADDRESSABLEPARTITION_H_ */
