/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICELM74)

#include "portable/devices/misc/include/DeviceLM74.h"

void DeviceLM74::init(void)
  {
    OSDeviceDebug::putString_P(PSTR("DeviceLM74::init()"));
    OSDeviceDebug::putNewLine();

    implSelectInit();
    clkInit();
    dataIn();
  }

signed short DeviceLM74::read(void)
  {
    int i;
    signed short ret;
    ret = 0;
    implSelectEnable();
      {
        for (i = 0; i < 16; ++i)
          {
            clkHigh();
            ret <<= 1;
            if (!dataIsLow())
              ret |= 1;
            clkLow();
          }
      }
    implSelectDisable();

    return (ret >> 3);
  }

#endif /*OS_INCLUDE_DEVICELM74*/

