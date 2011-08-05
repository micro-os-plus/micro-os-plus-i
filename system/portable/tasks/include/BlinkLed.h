/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "portable/kernel/include/OS.h"

class BlinkLed
  {
public:
  BlinkLed();

  static void toggle(void);
  static void on(void);
  static void off(void);
  static void init(void);

  unsigned char bitNumber(void);

private:
  };

inline BlinkLed::BlinkLed()
  {
  }

inline void BlinkLed::init(void)
  {
    // initialise led port as output
    OS_CONFIG_BLINK_LED_PORT_INIT |= _BV(OS_CONFIG_BLINK_LED_BIT);
  }

inline void BlinkLed::toggle(void)
  {
    OSCriticalSection::enter();
      {
        // toggle led
        OS_CONFIG_BLINK_LED_PORT ^= _BV(OS_CONFIG_BLINK_LED_BIT);
      }
    OSCriticalSection::exit();
  }

inline void BlinkLed::on(void)
  {
    OSCriticalSection::enter();
      {
        // toggle led
        OS_CONFIG_BLINK_LED_PORT |= _BV(OS_CONFIG_BLINK_LED_BIT);
      }
    OSCriticalSection::exit();
  }

inline void BlinkLed::off(void)
  {
    OSCriticalSection::enter();
      {
        // toggle led
        OS_CONFIG_BLINK_LED_PORT &= ~_BV(OS_CONFIG_BLINK_LED_BIT);
      }
    OSCriticalSection::exit();
  }

inline unsigned char BlinkLed::bitNumber(void)
  {
    return OS_CONFIG_BLINK_LED_BIT;
  }

#endif /* BLINKLED_H_ */
