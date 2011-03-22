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

// Read/Write multi-byte, in little-endian style.

// Read USB Word, LSB | MSB

unsigned short
OSUsbDevice::readWord(void)
{
  unsigned short w;

  w = readByte();
  w |= (readByte() << 8);
  return w;
}

// Read USB Long, LSB | MSB

unsigned long
OSUsbDevice::readLong(void)
{
  unsigned long l;

  l = readByte();
  l |= (((unsigned long) readByte()) << 8);
  l |= (((unsigned long) readByte()) << 16);
  l |= (((unsigned long) readByte()) << 24);
  return l;
}

// Write USB Word, LSB | MSB

void
OSUsbDevice::writeWord(unsigned short w)
{
  writeByte((unsigned char) w & 0xFF);
  w >>= 8;
  writeByte((unsigned char) w & 0xFF);
}

// Write USB Long, LSB | MSB

void
OSUsbDevice::writeLong(unsigned long l)
{
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */
