/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

// DEPRECATED

#if false && defined(OS_INCLUDE_DEVICECHARACTERUSART1)

#include "portable/devices/character/include/DeviceCharacterUsart1.h"

// ----- static data ---------------------------------------------------------

DeviceCharacterUsart1 * DeviceCharacterUsart1::ms_pThis;

// ----- constructors --------------------------------------------------------

// set custom high/low water marks
DeviceCharacterUsart1::DeviceCharacterUsart1(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
OSDeviceCharacterBuffered(pRxBuf, rxBufSize, rxHWM, rxLWM, pTxBuf, txBufSize,
    txHWM, txLWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("DeviceCharacterUsart1()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    ms_pThis = this;
  }

// use default 3/4 high and 1/4 low water marks
DeviceCharacterUsart1::DeviceCharacterUsart1(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned char *pTxBuf, unsigned short txBufSize) :
OSDeviceCharacterBuffered(pRxBuf, rxBufSize, pTxBuf, txBufSize)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("DeviceCharacterUsart1()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    ms_pThis = this;
  }

#endif /*OS_INCLUDE_DEVICECHARACTERUSART1*/
