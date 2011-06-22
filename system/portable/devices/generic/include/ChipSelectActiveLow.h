/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CHIPSELECTACTIVELOW_H_
#define CHIPSELECTACTIVELOW_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

typedef avr32::uc3::Gpio Gpio_t;

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

class ChipSelectActiveLow
{
public:
  ChipSelectActiveLow(Gpio_t& gpio);

  void
  init(void);
  void
  assert(void);
  void
  deassert(void);

private:
  Gpio_t& m_gpio;
};

inline void
ChipSelectActiveLow::init(void)
{
  m_gpio.setModeGpio();
  m_gpio.setPinHigh();
  m_gpio.setDirectionOutput();
}

inline void
ChipSelectActiveLow::assert(void)
{
  m_gpio.setPinLow();
}

inline void
ChipSelectActiveLow::deassert(void)
{
  m_gpio.setPinHigh();
}

#endif /* CHIPSELECTACTIVELOW_H_ */
