/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef SIGNALIMUX_H_
#define SIGNALIMUX_H_

// Deprecated
#include "portable/kernel/include/uOS.h"

#if defined(OS_CONFIG_BOARD_A0739)

class SignalIMUX
{
public:
  SignalIMUX();

  static void
  init(void);
  static void
  i2c(void);
  static void
  usart(void);
  static bool
  isI2C(void);
};

inline void
SignalIMUX::init(void)
{
  // init led port as output, value 0
  APP_CONFIG_IMUX_PORT_INIT |= _BV(APP_CONFIG_IMUX_BIT);
  APP_CONFIG_IMUX_PORT &= ~_BV(APP_CONFIG_IMUX_BIT);
}

inline void
SignalIMUX::i2c(void)
{
  os.sched.critical.enter();
    {
      // set bit to 1
      APP_CONFIG_IMUX_PORT |= _BV(APP_CONFIG_IMUX_BIT);
    }
  os.sched.critical.exit();
}

inline void
SignalIMUX::usart(void)
{
  os.sched.critical.enter();
    {
      // set bit to 0
      APP_CONFIG_IMUX_PORT &= ~_BV(APP_CONFIG_IMUX_BIT);
    }
  os.sched.critical.exit();
}

inline bool
SignalIMUX::isI2C(void)
{
  bool ret;
  os.sched.critical.enter();
    {
      ret = (APP_CONFIG_IMUX_PORT & _BV(APP_CONFIG_IMUX_BIT)) != 0;
    }
  os.sched.critical.exit();
  return ret;
}

#endif /* defined(OS_CONFIG_BOARD_A0739) */

#endif /* SIGNALIMUX_H_ */
