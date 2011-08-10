/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "portable/kernel/include/OS.h"

// Generic interface for Gpio drivers.
// Use it as a reference for actual implementations.

class Gpio
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  Gpio();
  ~Gpio();

  // ----- Public methods -----------------------------------------------------

  virtual void
  powerUp(void) = 0;

  virtual void
  powerDown(void) = 0;

  virtual void
  configureModeGpio(void) = 0;
  virtual void
  configureModePeripheral(void) = 0;
  virtual void
  toggleMode(void) = 0;
  virtual bool
  isModeGpio(void) = 0;

  //void
  //configurePeripheralFunction(gpio::PeripheralFunction_t f);

  virtual void
  setPinHigh(void) = 0;
  virtual void
  setPinLow(void) = 0;
  virtual void
  togglePin(void) = 0;

  virtual bool
  isPinHigh(void) = 0;
  virtual bool
  isPinLow(void) = 0;

  virtual void
  configureDirectionOutput(void) = 0;
  virtual void
  configureDirectionInput(void) = 0;
  virtual void
  toggleDirection(void) = 0;
  virtual bool
  isDirectionOutputEnabled(void) = 0;

  virtual void
  enablePullup(void) = 0;
  virtual void
  disablePullup(void) = 0;
  virtual void
  togglePullup(void) = 0;
  virtual bool
  isPullupEnabled(void) = 0;

  virtual void
  enableInterrupt(void) = 0;
  virtual void
  disableInterrupt(void) = 0;
  virtual void
  toggleInterrupt(void) = 0;
  virtual bool
  isInterruptEnabled(void) = 0;

  virtual bool
  isInterruptRequested(void) = 0;
  virtual void
  clearInterruptRequest(void) = 0;

  //void
  //registerInterruptHandler(avr32::uc3::intc::InterruptHandler_t handler);

private:

  // ----- Private members ----------------------------------------------------


  // --------------------------------------------------------------------------

};

#endif /* CHIPSELECT_H_ */
