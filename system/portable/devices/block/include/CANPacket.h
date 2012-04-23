/*
 *      Copyright (C) 2008-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CANPACKET_H_
#define CANPACKET_H_

#include "portable/kernel/include/OS.h"

class CANPacket
{
public:
  uint16_t id;
  unsigned char len;
  unsigned char data[8];

  void
  dump(void);

#if defined(OS_INCLUDE_CANPACKET_TIMESTAMP_UPTIME_SECONDS)
  unsigned long uptimeSeconds;
  OSTimerTicks_t uptimeTicks;
#elif defined(OS_INCLUDE_CANPACKET_TIMESTAMP_UPTIME_TICKS)
  uint32_t uptimeTicks;
#endif

};

#if !defined(DEBUG)
inline void CANPacket::dump(void)
  {
    ;
  }
#endif

#endif /* CANPACKET_H_ */
