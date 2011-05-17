/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICERTC4574SELECT_INLINES_H_
#define DEVICERTC4574SELECT_INLINES_H_

inline void DeviceRTC4574Select::implSelectInit(void)
  {
    OS_CONFIG_RTC4574_SELECT_PORT &= ~_BV(OS_CONFIG_RTC4574_SELECT_BIT);
    OS_CONFIG_RTC4574_SELECT_PORT_INIT |= _BV(OS_CONFIG_RTC4574_SELECT_BIT);
  }

inline void DeviceRTC4574Select::implSelectEnable(void)
  {
    OS_CONFIG_RTC4574_SELECT_PORT |= _BV(OS_CONFIG_RTC4574_SELECT_BIT);
  }

inline void DeviceRTC4574Select::implSelectDisable(void)
  {
    OS_CONFIG_RTC4574_SELECT_PORT &= ~_BV(OS_CONFIG_RTC4574_SELECT_BIT);
  }

#endif /* DEVICERTC4574SELECT_INLINES_H_ */
