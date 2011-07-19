/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEADDRESSABLESYNCHRONISED_H_
#define OSDEVICEADDRESSABLESYNCHRONISED_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceAddressable.h"

class OSDeviceAddressableSynchronised : public OSDeviceAddressable
{
public:
  OSDeviceAddressableSynchronised();
  //OSDeviceAddressableSynchronised(OSDeviceAddressable& dev);
  ~OSDeviceAddressableSynchronised();

private:
  //OSDeviceAddressable& m_dev;
};

#endif /* OSDEVICEADDRESSABLESYNCHRONISED_H_ */
