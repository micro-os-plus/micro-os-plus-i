/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICESPIMASTER_H_
#define DEVICESPIMASTER_H_

#include "portable/kernel/include/OS.h"

class DeviceSPIMaster
{
public:
  static void
  spiInit(unsigned short baud);
  static unsigned char
  spiRead(void);
  static unsigned char
  spiReadWrite(unsigned char c);

protected:
  static void
  implSpiInit(unsigned short baud);
  static unsigned char
  implSpiReadWrite(unsigned char c);
};

#endif /*DEVICESPIMASTER_H_*/
