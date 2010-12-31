/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CANLEDS_INLINES_H_
#define CANLEDS_INLINES_H_

inline void CANLedGreen::init(void)
  {
    OS_CONFIG_CANLEDS_GREEN_PORT &= ~_BV(OS_CONFIG_CANLEDS_GREEN_BIT);
    OS_CONFIG_CANLEDS_GREEN_PORT_INIT |= _BV(OS_CONFIG_CANLEDS_GREEN_BIT);
  }

inline void CANLedGreen::on(void)
  {
    OS_CONFIG_CANLEDS_GREEN_PORT |= _BV(OS_CONFIG_CANLEDS_GREEN_BIT);
  }

inline void CANLedGreen::off(void)
  {
    OS_CONFIG_CANLEDS_GREEN_PORT &= ~_BV(OS_CONFIG_CANLEDS_GREEN_BIT);
  }

inline void CANLedRed::init(void)
  {
    OS_CONFIG_CANLEDS_RED_PORT &= ~_BV(OS_CONFIG_CANLEDS_RED_BIT);
    OS_CONFIG_CANLEDS_RED_PORT_INIT |= _BV(OS_CONFIG_CANLEDS_RED_BIT);
  }

inline void CANLedRed::on(void)
  {
    OS_CONFIG_CANLEDS_RED_PORT |= _BV(OS_CONFIG_CANLEDS_RED_BIT);
  }

inline void CANLedRed::off(void)
  {
    OS_CONFIG_CANLEDS_RED_PORT &= ~_BV(OS_CONFIG_CANLEDS_RED_BIT);
  }

#endif /*CANLEDS_INLINES_H_*/
