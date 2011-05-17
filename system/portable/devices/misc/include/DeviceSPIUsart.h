/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICESPIUSART_H_
#define DEVICESPIUSART_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/misc/include/DeviceSPIMaster.h"

class DeviceSPIUsart : public DeviceSPIMaster
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

#include "hal/arch/avr8/at90usb/devices/misc/include/DeviceSPIUsart_Inlines.h"

#endif /* DEVICESPIUSART_H_ */
