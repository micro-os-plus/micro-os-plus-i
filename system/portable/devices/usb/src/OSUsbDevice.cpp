/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptors.h"

/*
 * This file contains the USB endpoint 0 management routines corresponding to
 * the USB device specification.
 */

// ----- Static data ----------------------------------------------------------

// ----- Constructors ---------------------------------------------------------

OSUsbDevice::OSUsbDevice()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSUsbDevice()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */
