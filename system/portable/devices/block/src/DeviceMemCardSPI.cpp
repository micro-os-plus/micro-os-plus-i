/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_MEMCARD)

#include "portable/devices/block/include/DeviceMemCardSPI.h"

void DeviceMemCardSPI::spiInitLowSpeed(void)
  {
    implSpiInitLowSpeed();
  }

void DeviceMemCardSPI::spiInit(void)
  {
    implSpiInit();
  }

unsigned char DeviceMemCardSPI::spiRead(void)
  {
    return implSpiReadWrite(0xFF);
  }

unsigned char DeviceMemCardSPI::spiReadWrite(unsigned char c)
  {
    return implSpiReadWrite(c);
  }

#endif
