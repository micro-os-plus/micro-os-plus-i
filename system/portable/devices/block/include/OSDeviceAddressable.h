/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEADDRESSABLE_H_
#define OSDEVICEADDRESSABLE_H_

#include "portable/kernel/include/OS.h"

class OSDeviceAddressable
{
public:

  // ----- Type definitions ---------------------------------------------------

  typedef uint32_t Offset_t;
  typedef uint16_t Count_t;
  typedef uint32_t Alignnment_t;

public:

  // ----- Constructors & Destructors -----------------------------------------

  OSDeviceAddressable();

  virtual
  ~OSDeviceAddressable();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operations.
  virtual OSReturn_t
  open(void);

  // Read exactly count bytes, in the given buffer.
  // Offset need not be aligned.
  virtual OSReturn_t
  readBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
      OSDeviceAddressable::Count_t count);

  // Write the given buffer. Offset need to be aligned with
  // getWriteAlignement() and count should be multiple of it.
  virtual OSReturn_t
  writeBytes(OSDeviceAddressable::Offset_t offset, uint8_t* pBuf,
      OSDeviceAddressable::Count_t count);

  // Prepare device for entering sleep
  virtual OSReturn_t
  close(void);

  // Erase part of the device, aligned to page if needed.
  virtual OSReturn_t
  erase(OSDeviceAddressable::Offset_t offset,
      OSDeviceAddressable::Count_t count);

  // Erase the entire device
  virtual OSReturn_t
  eraseEntireDevice(void);

  // Return the full size, in bytes, of the device.
  virtual OSDeviceAddressable::Offset_t
  getDeviceSize(void);

  // Return the write block size, to be used as alignment.
  virtual OSDeviceAddressable::Alignnment_t
  getWriteAlignment(void);

  // Return the erase page size, to be used as alignment.
  virtual OSDeviceAddressable::Alignnment_t
  getEraseAlignment(void);

  // --------------------------------------------------------------------------
};

#endif /* OSDEVICEADDRESSABLE_H_ */
