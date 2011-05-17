/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICELM74_INLINES_H_
#define DEVICELM74_INLINES_H_

inline void DeviceLM74::clkInit(void)
  {
    OS_CONFIG_LM74_CLK_PORT &= ~_BV(OS_CONFIG_LM74_CLK_BIT);
    OS_CONFIG_LM74_CLK_PORT_INIT |= _BV(OS_CONFIG_LM74_CLK_BIT);
  }

inline void DeviceLM74::clkHigh(void)
  {
    OS_CONFIG_LM74_CLK_PORT |= _BV(OS_CONFIG_LM74_CLK_BIT);
  }

inline void DeviceLM74::clkLow(void)
  {
    OS_CONFIG_LM74_CLK_PORT &= ~_BV(OS_CONFIG_LM74_CLK_BIT);
  }

inline void DeviceLM74::dataIn(void)
  {
    OS_CONFIG_LM74_DATA_PORT_INIT &= ~_BV(OS_CONFIG_LM74_DATA_BIT);
  }

inline void DeviceLM74::dataOut(void)
  {
    OS_CONFIG_LM74_DATA_PORT_INIT |= _BV(OS_CONFIG_LM74_DATA_BIT);
  }

inline void DeviceLM74::dataHigh(void)
  {
    OS_CONFIG_LM74_DATA_PORT |= _BV(OS_CONFIG_LM74_DATA_BIT);
  }

inline void DeviceLM74::dataLow(void)
  {
    OS_CONFIG_LM74_DATA_PORT &= ~_BV(OS_CONFIG_LM74_DATA_BIT);
  }

inline bool DeviceLM74::dataIsLow(void)
  {
    return ((OS_CONFIG_LM74_DATA_PORT_READ & _BV(OS_CONFIG_LM74_DATA_BIT)) == 0);
  }

#endif /* DEVICELM74_INLINES_H_ */
