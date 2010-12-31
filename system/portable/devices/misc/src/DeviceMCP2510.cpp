/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICEMCP2510)

#include "portable/devices/misc/include/DeviceMCP2510.h"

//#define OS_DEBUG_DEVICEMCP2510_CMDS

void DeviceMCP2510::init()
  {
    OSDeviceDebug::putString("DeviceMCP2510::init()");
    OSDeviceDebug::putNewLine();

    implSelectInit();

    // set baud rate at Fosc/(2*(x+1))
    // 4 MHz at 16 MHz, the mcp2510 allows max 5MHz
    spiInit(1);

    // be sure we wait the 128 Osc cycles
    for (unsigned int i = (128); i > 0; --i)
      OS::NOP();
    //cmdReset();
  }

void DeviceMCP2510::cmdReset(void)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdReset()");
    OSDeviceDebug::putNewLine();
#endif

    implSelectEnable();
      {
        spiReadWrite(RESET_CMD);
      }
    implSelectDisable();

    // be sure we wait the 128 Osc cycles
    for (unsigned int i = (128); i > 0; --i)
      OS::NOP();
  }

void DeviceMCP2510::cmdWrite(unsigned char addr, unsigned char data)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdWrite(");
    OSDeviceDebug::putHex(addr);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex(data);
    OSDeviceDebug::putString(")");
    OSDeviceDebug::putNewLine();
#endif

    implSelectEnable();
      {
        spiReadWrite(WRITE_CMD);

        spiReadWrite(addr);

        spiReadWrite(data);
      }
    implSelectDisable();
  }

unsigned char DeviceMCP2510::cmdRead(unsigned char addr)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdRead(");
    OSDeviceDebug::putHex(addr);
    OSDeviceDebug::putString(")=");
#endif

    unsigned char ret;
    implSelectEnable();
      {
        spiReadWrite(READ_CMD);

        spiReadWrite(addr);

        ret = spiRead();
      }
    implSelectDisable();

#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putHex(ret);
    OSDeviceDebug::putNewLine();
#endif

    return ret;
  }

void DeviceMCP2510::cmdBitsModify(unsigned char addr, unsigned char mask,
    unsigned char data)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdBitsModify(");
    OSDeviceDebug::putHex(addr);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex(mask);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex(data);
    OSDeviceDebug::putString(")");
    OSDeviceDebug::putNewLine();
#endif

    implSelectEnable();
      {
        spiReadWrite(BIT_MODIFY_CMD);

        spiReadWrite(addr);

        spiReadWrite(mask);

        spiReadWrite(data);
      }
    implSelectDisable();
  }

unsigned char DeviceMCP2510::cmdReadStatus(void)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdReadStatus()=");
#endif

    unsigned char ret;
    implSelectEnable();
      {
        spiReadWrite(READ_STATUS_CMD);
        ret = spiRead();
        spiRead(); // ignore repeat data
      }
    implSelectDisable();

#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putHex(ret);
    OSDeviceDebug::putNewLine();
#endif

    return ret;
  }

void DeviceMCP2510::cmdRts(unsigned char channel)
  {
#if defined(DEBUG) && defined(OS_DEBUG_DEVICEMCP2510_CMDS)
    OSDeviceDebug::putString("DeviceMCP2510::cmdRts(");
    OSDeviceDebug::putHex(channel);
    OSDeviceDebug::putString(")");
    OSDeviceDebug::putNewLine();
#endif

    implSelectEnable();
      {
        spiReadWrite(RTS_CMD | (channel&0x7));
      }
    implSelectDisable();
  }

#endif /*OS_INCLUDE_DEVICEMCP2510*/

