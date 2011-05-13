/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE)

#include "portable/devices/character/include/DeviceCharacterBufferedUsartBase.h"

// ----- constructors ---------------------------------------------------------

// set custom high/low water marks
DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM) :
  OSDeviceCharacterBuffered(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf, rxBufSize,
      rxHWM, rxLWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterBufferedUsartBase"), this);
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned char *pRxBuf,
    unsigned short rxBufSize) :
  OSDeviceCharacterBuffered(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterBufferedUsartBase"), this);
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

