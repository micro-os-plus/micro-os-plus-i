/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef SIGNALIMUXEN_H_
#define SIGNALIMUXEN_H_

// Deprecated
#include "portable/kernel/include/uOS.h"

#if defined(OS_CONFIG_BOARD_A0739)

class SignalIMUXEN
{
public:
  SignalIMUXEN();

  static void
  init(void);
  static void
  enable(void);
  static void
  disable(void);
};

inline void
SignalIMUXEN::init(void)
{
  // init led port as output, value 0
  APP_CONFIG_IMUXEN_PORT_INIT |= _BV(APP_CONFIG_IMUXEN_BIT);
  APP_CONFIG_IMUXEN_PORT &= ~_BV(APP_CONFIG_IMUXEN_BIT);
}

inline void
SignalIMUXEN::enable(void)
{
  os.sched.criticalEnter();
    {
      // set bit to 1
      APP_CONFIG_IMUXEN_PORT |= _BV(APP_CONFIG_IMUXEN_BIT);
    }
  os.sched.criticalExit();
}

inline void
SignalIMUXEN::disable(void)
{
  os.sched.criticalEnter();
    {
      // set bit to 0
      APP_CONFIG_IMUXEN_PORT &= ~_BV(APP_CONFIG_IMUXEN_BIT);
    }
  os.sched.criticalExit();
}

#endif /* defined(OS_CONFIG_BOARD_A0739) */

#endif /* SIGNALIMUXEN_H_ */
