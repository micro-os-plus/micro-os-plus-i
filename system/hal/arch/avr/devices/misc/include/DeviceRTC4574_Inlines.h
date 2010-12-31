/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICERTC4574_INLINES_H_
#define DEVICERTC4574_INLINES_H_

inline void DeviceRTC4574::clkInit(void)
  {
    clkLow();
    OS_CONFIG_RTC4574_CLK_PORT_INIT |= _BV(OS_CONFIG_RTC4574_CLK_BIT);
  }

inline void DeviceRTC4574::clkHigh(void)
  {
    OS_CONFIG_RTC4574_CLK_PORT |= _BV(OS_CONFIG_RTC4574_CLK_BIT);
  }

inline void DeviceRTC4574::clkLow(void)
  {
    OS_CONFIG_RTC4574_CLK_PORT &= ~_BV(OS_CONFIG_RTC4574_CLK_BIT);
  }

inline void DeviceRTC4574::dataIn(void)
  {
    OS_CONFIG_RTC4574_DATA_PORT_INIT &= ~_BV(OS_CONFIG_RTC4574_DATA_BIT);
  }

inline void DeviceRTC4574::dataOut(void)
  {
    OS_CONFIG_RTC4574_DATA_PORT_INIT |= _BV(OS_CONFIG_RTC4574_DATA_BIT);
  }

inline void DeviceRTC4574::dataHigh(void)
  {
    OS_CONFIG_RTC4574_DATA_PORT |= _BV(OS_CONFIG_RTC4574_DATA_BIT);
  }

inline void DeviceRTC4574::dataLow(void)
  {
    OS_CONFIG_RTC4574_DATA_PORT &= ~_BV(OS_CONFIG_RTC4574_DATA_BIT);
  }

inline bool DeviceRTC4574::dataIsLow(void)
  {
    return ((OS_CONFIG_RTC4574_DATA_PORT_READ & _BV(OS_CONFIG_RTC4574_DATA_BIT)) == 0);
  }

inline void DeviceRTC4574::fconInit(void)
  {
    fconHigh();
    OS_CONFIG_RTC4574_FCON_PORT_INIT |= _BV(OS_CONFIG_RTC4574_FCON_BIT);
  }

inline void DeviceRTC4574::fconHigh(void)
  {
    OS_CONFIG_RTC4574_FCON_PORT |= _BV(OS_CONFIG_RTC4574_FCON_BIT);
  }

inline void DeviceRTC4574::fconLow(void)
  {
    OS_CONFIG_RTC4574_FCON_PORT &= ~_BV(OS_CONFIG_RTC4574_FCON_BIT);
  }


#endif /*DEVICERTC4574_INLINES_H_*/
