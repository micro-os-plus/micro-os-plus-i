/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICESPIUSART)

#include "portable/devices/misc/include/DeviceSPIUsart.h"

void DeviceSPIUsart::spiInit(unsigned short baud)
  {
    implSpiInit(baud);
  }

unsigned char DeviceSPIUsart::spiRead(void)
  {
    return implSpiReadWrite(0xFF);
  }

unsigned char DeviceSPIUsart::spiReadWrite(unsigned char c)
  {
    return implSpiReadWrite(c);
  }

#endif /*OS_INCLUDE_DEVICESPIUSART*/

