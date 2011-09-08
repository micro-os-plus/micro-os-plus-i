/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEBLOCK_H_
#define OSDEVICEBLOCK_H_

#include "portable/kernel/include/OS.h"

// The interface design was in general intended to be generic,
// but the fact is that it was modelled to be better accommodate SD Cards.

// The transfer unit is a block, in general 512 bytes long.

class OSDeviceBlock
{
public:

  // ----- Type definitions ---------------------------------------------------

  // Index identifying the block, between 0 and size-1
  typedef uint32_t BlockNumber_t;

  // Count of block to process in one request, usually very small
  typedef uint16_t BlockCount_t;

  // Size of a block, in bytes or in smaller blocks
  typedef uint32_t BlockSize_t;

public:

  // ----- Constructors & Destructors -----------------------------------------

  OSDeviceBlock();

  virtual
  ~OSDeviceBlock();

  // ----- Public methods -----------------------------------------------------

  // Prepare the device for operations.
  virtual OSReturn_t
  open(void) = 0;

  // Read exactly count blocks, in the given buffer.
  virtual OSReturn_t
  readBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count) = 0;

  // Write the count blocks, from the given buffer.
  virtual OSReturn_t
  writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count) = 0;

  // Prepare device for entering sleep
  virtual OSReturn_t
  close(void) = 0;

  // Erase part of the device.
  virtual OSReturn_t
  eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count) = 0;

  // Return the full size, in blocks, of the device.
  virtual OSDeviceBlock::BlockNumber_t
  getDeviceSizeBlocks(void) = 0;

  // Return the device block size, in bytes.
  virtual OSDeviceBlock::BlockSize_t
  getBlockSizeBytes(void) = 0;

  // --------------------------------------------------------------------------
};

#endif /* OSDEVICEBLOCK_H_ */
