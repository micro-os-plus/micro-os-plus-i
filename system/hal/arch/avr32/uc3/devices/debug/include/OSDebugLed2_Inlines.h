/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEBUGLED2_INLINES_H_
#define OSDEBUGLED2_INLINES_H_

#include "portable/kernel/include/OS.h"

inline void
OSDebugLed2::init(void)
{
  // init led port as output
  OS_GPIO_PIN_CONFIG(OS_CONFIG_DEBUG_LED2_PORT_INIT, OS_CONFIG_DEBUG_LED2_BIT,
      1);
}

inline void
OSDebugLed2::toggle(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_TOGGLE(OS_CONFIG_DEBUG_LED2_PORT, OS_CONFIG_DEBUG_LED2_BIT);
    }
  OSCriticalSection::exit();
}

inline void
OSDebugLed2::on(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_LED2_PORT, OS_CONFIG_DEBUG_LED2_BIT);
    }
  OSCriticalSection::exit();
}

inline void
OSDebugLed2::off(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_LED2_PORT, OS_CONFIG_DEBUG_LED2_BIT);
    }
  OSCriticalSection::exit();
}

inline unsigned char
OSDebugLed2::bitNumber(void)
{
  return OS_CONFIG_DEBUG_LED2_BIT;
}

#endif /*OSDEBUGLED2_INLINES_H_*/
