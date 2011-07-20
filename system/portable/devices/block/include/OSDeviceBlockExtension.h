/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEBLOCKEXTENSION_H_
#define OSDEVICEBLOCKEXTENSION_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlock.h"

class OSDeviceBlockExtension : public OSDeviceBlock
{
public:

  // ----- Constructors & destructors -------------------------------------------

  OSDeviceBlockExtension(OSDeviceBlock& parent);
  virtual
  ~OSDeviceBlockExtension();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operations.
  virtual OSReturn_t
  open(void);

  // Read exactly count blocks, in the given buffer.
  virtual OSReturn_t
  readBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Write the count blocks, from the given buffer.
  virtual OSReturn_t
  writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Prepare device for entering sleep
  virtual OSReturn_t
  close(void);

  // Erase part of the device.
  virtual OSReturn_t
  eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

  // Return the full size, in blocks, of the device.
  virtual OSDeviceBlock::BlockNumber_t
  getDeviceSize(void);

  // Return the device block size, in bytes.
  virtual OSDeviceBlock::BlockSize_t
  getBlockSize(void);

  OSDeviceBlock&
  getParent(void);

protected:

  // ----- Members ------------------------------------------------------------

  OSDeviceBlock& m_parent;

  // --------------------------------------------------------------------------

};

// ----- Inline methods -------------------------------------------------------

inline OSDeviceBlock&
OSDeviceBlockExtension::getParent(void)
{
  return m_parent;
}

// ----------------------------------------------------------------------------

#endif /* OSDEVICEADDRESSABLEPARTITION_H_ */
