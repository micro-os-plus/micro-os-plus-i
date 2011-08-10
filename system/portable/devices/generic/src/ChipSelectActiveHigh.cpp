/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CHIPSELECTACTIVEHIGH)

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/ChipSelectActiveHigh.h"

ChipSelectActiveHigh::ChipSelectActiveHigh(Gpio_t& gpio) :
  m_gpio(gpio)
{
  OSDeviceDebug::putConstructor("ChipSelectActiveHigh", this);
}

ChipSelectActiveHigh::~ChipSelectActiveHigh()
{
  OSDeviceDebug::putDestructor("ChipSelectActiveHigh", this);
}

void
ChipSelectActiveHigh::powerUp(void)
{
  m_gpio.configureModeGpio();
  m_gpio.setPinLow();
  m_gpio.configureDirectionOutput();
}

void
ChipSelectActiveHigh::powerDown(void)
{
  m_gpio.setPinLow();
  m_gpio.configureDirectionInput();
}

#endif /* defined(OS_INCLUDE_CHIPSELECTACTIVEHIGH) */
