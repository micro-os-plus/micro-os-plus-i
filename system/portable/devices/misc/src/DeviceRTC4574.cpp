/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICERTC4574)

#include "portable/devices/misc/include/DeviceRTC4574.h"

void DeviceRTC4574::init(void)
  {
    OSDeviceDebug::putString("DeviceRTC4574::init()");
    OSDeviceDebug::putNewLine();

    implSelectInit();
    fconInit();
    clkInit();
    dataIn();
  }

void DeviceRTC4574::delay(void)
  {
    for (int i=0; i < 10; ++i)
      OS::NOP();
  }

unsigned char DeviceRTC4574::readBytes(unsigned char addr, unsigned char *pBuf,
    unsigned char len)
  {
    unsigned char ret;
    ret = 0;
    
    implSelectEnable();
      {
        delay();
        dataOut();
        delay();
        writeByte((addr << 4) | 0x0C);
        delay();
        dataIn();
        delay();

        if (pBuf != 0 && len != 0)
          {
            for (unsigned char i = 0; i < len; ++i)
              {
                *pBuf++ = readByte();
              }
          }
        else
          {
            ret = readByte();
          }

        delay();
      }
    implSelectDisable();

    return ret;
  }

void DeviceRTC4574::writeBytes(unsigned char addr, unsigned char *pBuf,
    unsigned char len)
  {
    if (pBuf != 0 && len != 0)
      {
        implSelectEnable();
          {
            delay();
            dataOut();
            delay();
            writeByte((addr << 4) | 0x03);

            for (unsigned char i = 0; i < len; ++i)
              {
                writeByte(*pBuf++);
              }

            delay();
            dataIn();
            delay();
          }
        implSelectDisable();
      }
    return;
  }

void DeviceRTC4574::writeByte(unsigned char c)
  {
    for (unsigned char i = 0; i < 8; ++i)
      {
        clkLow();
        delay();
        if ((c & 0x01) != 0)
          dataHigh();
        else
          dataLow();
        c >>= 1;
        delay();

        clkHigh();
        delay();
        delay();
      }
  }

unsigned char DeviceRTC4574::readByte(void)
  {
    unsigned char c;
    c = 0;
    
    for (unsigned char i = 0; i < 8; ++i)
      {
        clkLow();
        delay();
        delay();
        c >>= 1;
        if (!dataIsLow())
          c |= 0x80;

        clkHigh();
        delay();
        delay();
      }

    return c;
  }

#endif /*OS_INCLUDE_DEVICERTC4574*/
