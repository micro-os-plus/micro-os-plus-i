/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMCP2510SPI_H_
#define DEVICEMCP2510SPI_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_BOARD_AVI_A07527)

#include "portable/devices/misc/include/DeviceSPIUsart.h"

class DeviceMCP2510SPI : public DeviceSPIUsart
  {
  public:
  };

#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#endif /* DEVICEMCP2510SPI_H_ */
