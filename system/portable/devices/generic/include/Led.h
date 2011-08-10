/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LED_H_
#define LED_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/Gpio.h"

// Interface for Led drivers.
// Use it as a reference for actual implementations.

class Led
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  Led(Gpio& gpio);
  ~Led();

  // ----- Public methods -----------------------------------------------------

  virtual void
  powerUp(void) = 0;

  virtual void
  powerDown(void) = 0;

  virtual void
  turnOn(void) = 0;

  virtual void
  turnOff(void) = 0;

  virtual bool
  isTurnedOn(void) = 0;

private:

  // ----- Private members ----------------------------------------------------

  Gpio& m_gpio;

  // --------------------------------------------------------------------------

};

// ============================================================================

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

typedef avr32::uc3::Gpio Gpio_t;

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

// ============================================================================

class LedActiveHigh
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  LedActiveHigh(Gpio_t& gpio);
  ~LedActiveHigh();

  // ----- Public methods -----------------------------------------------------

  void
  powerUp(void);

  void
  powerDown(void);

  void
  turnOn(void);

  void
  turnOff(void);

  bool
  isTurnedOn(void);

private:

  // ----- Private members ----------------------------------------------------

  Gpio_t& m_gpio;

  // --------------------------------------------------------------------------

};

// ----- Inline methods -------------------------------------------------------

inline void
LedActiveHigh::turnOn(void)
{
  m_gpio.setPinHigh();
}

inline void
LedActiveHigh::turnOff(void)
{
  m_gpio.setPinLow();
}

inline bool
LedActiveHigh::isTurnedOn(void)
{
  return m_gpio.isPinHigh();
}

// ============================================================================

class LedActiveLow
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  LedActiveLow(Gpio_t& gpio);
  ~LedActiveLow();

  // ----- Public methods -----------------------------------------------------

  void
  powerUp(void);

  void
  powerDown(void);

  void
  turnOn(void);

  void
  turnOff(void);

  bool
  isTurnedOn(void);

private:

  // ----- Private members ----------------------------------------------------

  Gpio_t& m_gpio;

  // --------------------------------------------------------------------------

};

// ----- Inline methods -------------------------------------------------------

inline void
LedActiveLow::turnOn(void)
{
  m_gpio.setPinLow();
}

inline void
LedActiveLow::turnOff(void)
{
  m_gpio.setPinHigh();
}

inline bool
LedActiveLow::isTurnedOn(void)
{
  return m_gpio.isPinLow();
}

// ============================================================================

#endif /* LED_H_ */
