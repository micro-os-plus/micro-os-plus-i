/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECANTRANSCEIVER_INLINES_H_
#define DEVICECANTRANSCEIVER_INLINES_H_

inline void DeviceCANTransceiver::transceiverInit(void)
  {
    OS_CONFIG_DEVICECANTRANSCEIVER_RS_PORT |= _BV(OS_CONFIG_DEVICECANTRANSCEIVER_RS_BIT);
    OS_CONFIG_DEVICECANTRANSCEIVER_RS_PORT_INIT |= _BV(OS_CONFIG_DEVICECANTRANSCEIVER_RS_BIT);
  }

inline void DeviceCANTransceiver::transceiverHighSpeed(void)
  {
    OS_CONFIG_DEVICECANTRANSCEIVER_RS_PORT &= ~_BV(OS_CONFIG_DEVICECANTRANSCEIVER_RS_BIT);
  }

inline void DeviceCANTransceiver::transceiverStandby(void)
  {
    OS_CONFIG_DEVICECANTRANSCEIVER_RS_PORT |= _BV(OS_CONFIG_DEVICECANTRANSCEIVER_RS_BIT);
  }

#endif /* DEVICECANTRANSCEIVER_INLINES_H_ */
