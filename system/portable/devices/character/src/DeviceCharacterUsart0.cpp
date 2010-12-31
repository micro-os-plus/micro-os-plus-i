/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERUSART0)

#include "portable/devices/character/include/DeviceCharacterUsart0.h"

// ----- static data ---------------------------------------------------------

DeviceCharacterUsart0 * DeviceCharacterUsart0::ms_pThis;

// ----- constructors --------------------------------------------------------

// set custom high/low water marks
DeviceCharacterUsart0::DeviceCharacterUsart0(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
  OSDeviceCharacterBuffered(pRxBuf, rxBufSize, rxHWM, rxLWM, pTxBuf, txBufSize,
      txHWM, txLWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("DeviceCharacterUsart0()="));
    OSDeviceDebug::putHex((unsigned short)this);
    OSDeviceDebug::putNewLine();
#endif

    ms_pThis = this;
  }

// use default 3/4 high and 1/4 low water marks
DeviceCharacterUsart0::DeviceCharacterUsart0(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned char *pTxBuf, unsigned short txBufSize) :
  OSDeviceCharacterBuffered(pRxBuf, rxBufSize, pTxBuf, txBufSize)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("DeviceCharacterUsart0()="));
    OSDeviceDebug::putHex((unsigned short)this);
    OSDeviceDebug::putNewLine();
#endif

    ms_pThis = this;
  }

#endif /*OS_INCLUDE_DEVICECHARACTERUSART0*/
