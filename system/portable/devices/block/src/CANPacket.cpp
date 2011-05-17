/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICECAN)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/CANPacket.h"

#if defined(DEBUG)
void CANPacket::dump(void)
  {
    unsigned char i;
    unsigned char* p;

    i = len & 0x0F;
    OSDeviceDebug::putHex(id);
    OSDeviceDebug::putChar(' ');
    OSDeviceDebug::putDec((unsigned short)i, 0);
    for (p = data; i != 0; --i)
      {
        OSDeviceDebug::putChar(' ');
        OSDeviceDebug::putHex(*p++);
      }
    OSDeviceDebug::putNewLine();
  }
#endif /* defined(DEBUG) */

#endif /* defined(OS_INCLUDE_OSDEVICECAN) */
