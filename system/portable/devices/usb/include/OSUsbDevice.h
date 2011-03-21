/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBDEVICE_H_
#define OSUSBDEVICE_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbLed.h"
#include "portable/devices/usb/include/OSUsbDevice_Defines.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptors.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceImpl.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDeviceImpl.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

// ----------------------------------------------------------------------------

class OSUsbDevice : public OSUsbDeviceImpl
{
public:
  OSUsbDevice();

// ----------------------------------------------------------------------------

// TODO: bring common code here

// ----------------------------------------------------------------------------

private:

};

// ----------------------------------------------------------------------------

#endif /* OSUSBDEVICE_H_ */
