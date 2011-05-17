/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMEMCARDSPI_H_
#define DEVICEMEMCARDSPI_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/misc/include/DeviceSPIMaster.h"

class DeviceMemCardSPI : public DeviceSPIMaster
{
public:
  static void
  spiInitLowSpeed(void);
  static void
  spiInit(void);
  static unsigned char
  spiRead(void);
  static unsigned char
  spiReadWrite(unsigned char c);

protected:
  static void
  implSpiInitLowSpeed(void);
  static void
  implSpiInit(void);
  static unsigned char
  implSpiReadWrite(unsigned char c);
};

#include "hal/arch/avr8/at90usb/devices/block/include/DeviceMemCardSPI_Defines.h"

#include "hal/arch/avr8/devices/block/include/DeviceMemCardSPI_Inlines.h"

#endif /* DEVICEMEMCARDSPI_H_ */
