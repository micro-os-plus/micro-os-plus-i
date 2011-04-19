/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBDEVICEDESCRIPTORSCONFIG_H_
#define OSUSBDEVICEDESCRIPTORSCONFIG_H_

#include "portable/kernel/include/OS_Defines.h"

// ----------------------------------------------------------------------------

// TODO: bring common code here

// ----------------------------------------------------------------------------

#if false

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif

// ----------------------------------------------------------------------------

#endif /* OSUSBDEVICEDESCRIPTORSCONFIG_H_ */
