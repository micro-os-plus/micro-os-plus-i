/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBLED_H_
#define OSUSBLED_H_

#include "portable/kernel/include/OS.h"

class OSUsbLed
{
public:
  OSUsbLed();

  static void
  init(void);

  static void
  on(void);

  static void
  off(void);

  static void
  toggle(void);

  unsigned char
  bitNumber(void);

private:
};

inline
OSUsbLed::OSUsbLed()
{
  // nothing to do, init() will do the magic
}

inline void
OSUsbLed::init(void)
{
#if defined(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG) && defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE) && defined(OS_CFGPIN_OSUSBLED_LED_PIN)

  // enable port
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG, OS_CFGPIN_OSUSBLED_LED_PIN);
  // init led port as output
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG, OS_CFGPIN_OSUSBLED_LED_PIN);
  // turn led off
#if defined(OS_CFGBOOL_OSUSBLED_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_HIGH(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#else
  OS_GPIO_PIN_LOW(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#endif /* defined(APP_CONFIG_LED_ISACTIVE_LOW) */

#else
#warning "Missing OS_CFGREG_OSUSBLED_LED_* definitions"
#endif
}

inline void
OSUsbLed::toggle(void)
{
#if defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE) && defined(OS_CFGPIN_OSUSBLED_LED_PIN)

  // toggle led
  OS_GPIO_PIN_TOGGLE(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);

#endif
}

inline void
OSUsbLed::on(void)
{
#if defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE) && defined(OS_CFGPIN_OSUSBLED_LED_PIN)

  // turn led on
#if defined(OS_CFGBOOL_OSUSBLED_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_HIGH(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#else
  OS_GPIO_PIN_LOW(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#endif /* defined(APP_CONFIG_LED_ISACTIVE_LOW) */

#endif
}

inline void
OSUsbLed::off(void)
{
#if defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE) && defined(OS_CFGPIN_OSUSBLED_LED_PIN)

  // turn led off
#if defined(OS_CFGBOOL_OSUSBLED_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_LOW(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#else
  OS_GPIO_PIN_HIGH(OS_CFGREG_OSUSBLED_LED_PORT_WRITE, OS_CFGPIN_OSUSBLED_LED_PIN);
#endif /* defined(APP_CONFIG_LED_ISACTIVE_LOW) */

#endif
}

inline unsigned char
OSUsbLed::bitNumber(void)
{
#if  defined(OS_CFGPIN_OSUSBLED_LED_PIN)
  return OS_CFGPIN_OSUSBLED_LED_PIN;
#else
  return 0;
#endif
}

#endif /* OSUSBLED_H_ */
