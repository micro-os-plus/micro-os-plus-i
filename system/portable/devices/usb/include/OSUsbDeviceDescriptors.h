/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBDEVICEDESCRIPTORS_H_
#define OSUSBDEVICEDESCRIPTORS_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceDescriptors.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDeviceDescriptors.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

// ----------------------------------------------------------------------------

// TODO: bring common code here

// ----------------------------------------------------------------------------

#endif /* OSUSBDEVICEDESCRIPTORS_H_ */
