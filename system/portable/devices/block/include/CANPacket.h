/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef CANPACKET_H_
#define CANPACKET_H_

class CANPacket
  {
public:
  unsigned short id;
  unsigned char len;
  unsigned char data[8];

  void dump(void);

  };

#if !defined(DEBUG)
inline void CANPacket::dump(void)
  {
    ;
  }
#endif

#endif /*CANPACKET_H_*/
