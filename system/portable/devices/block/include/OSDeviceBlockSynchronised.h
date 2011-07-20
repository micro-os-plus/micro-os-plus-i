/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICEBLOCKSYNCHRONISED_H_
#define OSDEVICEBLOCKSYNCHRONISED_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceBlockExtension.h"
#include "portable/kernel/include/OSMutex.h"

class OSDeviceBlockSynchronised : public OSDeviceBlockExtension
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSDeviceBlockSynchronised(OSDeviceBlock& parent, OSMutex& mutex);
  virtual
  ~OSDeviceBlockSynchronised();

  // ----- Public virtual methods ---------------------------------------------

  // Read exactly count blocks, in the given buffer.
  virtual OSReturn_t
  readBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Write the count blocks, from the given buffer.
  virtual OSReturn_t
  writeBlocks(OSDeviceBlock::BlockNumber_t blockNumber, uint8_t* pBuf,
      OSDeviceBlock::BlockCount_t count);

  // Erase part of the device.
  virtual OSReturn_t
  eraseBlocks(OSDeviceBlock::BlockNumber_t blockNumber,
      OSDeviceBlock::BlockCount_t count);

private:

  // ----- Members ------------------------------------------------------------

  OSMutex& m_mutex;

  // --------------------------------------------------------------------------
};

#endif /* OSDEVICEBLOCKSYNCHRONISED_H_ */
