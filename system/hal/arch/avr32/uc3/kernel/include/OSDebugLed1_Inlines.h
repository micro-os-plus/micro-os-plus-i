/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEBUGLED1_INLINES_H_
#define OSDEBUGLED1_INLINES_H_

#include "portable/kernel/include/OS.h"

inline void
OSDebugLed1::init(void)
{
  // init led port as output
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_DEBUG_LED1_PORT_INIT, OS_CONFIG_DEBUG_LED1_BIT);
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_DEBUG_LED1_PORT_INIT, OS_CONFIG_DEBUG_LED1_BIT);
}

inline void
OSDebugLed1::toggle(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_TOGGLE(OS_CONFIG_DEBUG_LED1_PORT, OS_CONFIG_DEBUG_LED1_BIT);
    }
  OSCriticalSection::exit();
}

inline void
OSDebugLed1::on(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_LED1_PORT, OS_CONFIG_DEBUG_LED1_BIT);
    }
  OSCriticalSection::exit();
}

inline void
OSDebugLed1::off(void)
{
  OSCriticalSection::enter();
    {
      // toggle led
      OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_LED1_PORT, OS_CONFIG_DEBUG_LED1_BIT);
    }
  OSCriticalSection::exit();
}

inline unsigned char
OSDebugLed1::bitNumber(void)
{
  return OS_CONFIG_DEBUG_LED1_BIT;
}

#endif /*OSDEBUGLED1_INLINES_H_*/
