/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef SPIMASTER_H_
#define SPIMASTER_H_

#include "portable/kernel/include/OS.h"

// Interface for SPI Master implementations
// Use it as a reference for actual implementations.

class SpiMaster
{

  // --------------------------------------------------------------------------

public:

  // ----- Type definitions ---------------------------------------------------

  typedef uint32_t ClockFrequencyHz_t;

public:

  // ----- Constructors & Destructors -----------------------------------------

  SpiMaster();
  ~SpiMaster();

public:

  // ----- Public methods -----------------------------------------------------

  virtual void
  powerUp(void) = 0;

  virtual void
  powerDown(void) = 0;

  virtual void
  configureClockFrequencyHz(ClockFrequencyHz_t clockFrequency) = 0;

  virtual uint8_t
  writeWaitReadByte(uint8_t value) = 0;

  virtual uint16_t
  writeWaitReadWord(uint16_t value) = 0;

  // --------------------------------------------------------------------------

};

#endif /* SPIMASTER_H_ */
