/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LEDACTIVEHIGH)

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/Led.h"

LedActiveHigh::LedActiveHigh(Gpio_t& gpio) :
  m_gpio(gpio)
{
  OSDeviceDebug::putConstructor("LedActiveHigh", this);
}

LedActiveHigh::~LedActiveHigh()
{
  OSDeviceDebug::putDestructor("ChipSelectActiveHigh", this);
}

void
LedActiveHigh::powerUp(void)
{
  m_gpio.configureModeGpio();
  m_gpio.setPinLow();
  m_gpio.configureDirectionOutput();
}

void
LedActiveHigh::powerDown(void)
{
  m_gpio.setPinLow();
  m_gpio.configureDirectionInput();
}

#endif /* defined(OS_INCLUDE_LEDACTIVEHIGH) */

#if defined(OS_INCLUDE_LEDACTIVELOW)

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/Led.h"

LedActiveLow::LedActiveLow(Gpio_t& gpio) :
  m_gpio(gpio)
{
  OSDeviceDebug::putConstructor("LedActiveLow", this);
}

LedActiveLow::~LedActiveLow()
{
  OSDeviceDebug::putDestructor("LedActiveLow", this);
}

void
LedActiveLow::powerUp(void)
{
  m_gpio.configureModeGpio();
  m_gpio.setPinHigh();
  m_gpio.configureDirectionOutput();
}

void
LedActiveLow::powerDown(void)
{
  m_gpio.setPinHigh();
  m_gpio.configureDirectionInput();
}

#endif /* defined(OS_INCLUDE_LEDACTIVELOW) */
