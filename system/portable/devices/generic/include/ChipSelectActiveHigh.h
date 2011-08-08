/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CHIPSELECTACTIVEHIGH_H_
#define CHIPSELECTACTIVEHIGH_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

typedef avr32::uc3::Gpio Gpio_t;

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

class ChipSelectActiveHigh
{
  // --------------------------------------------------------------------------

public:

  // ----- Constructors & Destructors -----------------------------------------

  ChipSelectActiveHigh(Gpio_t& gpio);
  ~ChipSelectActiveHigh();

  // ----- Public methods -----------------------------------------------------

  void
  powerUp(void);

  void
  powerDown(void);

  void
  assert(void);
  void
  deassert(void);

private:

  // ----- Private members ----------------------------------------------------

  Gpio_t& m_gpio;

  // --------------------------------------------------------------------------

};

// ----- Inline methods -------------------------------------------------------

inline void
ChipSelectActiveHigh::powerUp(void)
{
  m_gpio.configureModeGpio();
  m_gpio.setPinLow();
  m_gpio.configureDirectionOutput();
}

inline void
ChipSelectActiveHigh::powerDown(void)
{
  m_gpio.setPinLow();
  m_gpio.configureDirectionInput();
}

inline void
ChipSelectActiveHigh::assert(void)
{
  m_gpio.setPinHigh();
}

inline void
ChipSelectActiveHigh::deassert(void)
{
  m_gpio.setPinLow();
}

// ----------------------------------------------------------------------------

#endif /* CHIPSELECTACTIVEHIGH_H_ */
