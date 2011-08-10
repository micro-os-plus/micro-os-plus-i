/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CHIPSELECT_H_
#define CHIPSELECT_H_

#include "portable/kernel/include/OS.h"

// Interface for Chip Select drivers.
// Use it as a reference for actual implementations.

class ChipSelect
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  ChipSelect(Gpio& gpio);
  ~ChipSelect();

  // ----- Public methods -----------------------------------------------------

  virtual void
  powerUp(void) = 0;

  virtual void
  powerDown(void) = 0;

  virtual void
  assert(void) = 0;

  virtual void
  deassert(void) = 0;

private:

  // ----- Private members ----------------------------------------------------

  Gpio& m_gpio;

  // --------------------------------------------------------------------------

};

#endif /* CHIPSELECT_H_ */
