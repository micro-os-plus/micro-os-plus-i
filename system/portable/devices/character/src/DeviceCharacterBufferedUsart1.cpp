/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"

// ----- static data ----------------------------------------------------------

DeviceCharacterBufferedUsart1 * DeviceCharacterBufferedUsart1::ms_pThis;

// ----- constructors ---------------------------------------------------------

// set custom high/low water marks
DeviceCharacterBufferedUsart1::DeviceCharacterBufferedUsart1(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM) :
  DeviceCharacterBufferedUsartBase(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf,
      rxBufSize, rxHWM, rxLWM)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart1()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterBufferedUsart1::DeviceCharacterBufferedUsart1(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned char *pRxBuf,
    unsigned short rxBufSize) :
  DeviceCharacterBufferedUsartBase(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart1()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// ----- inits ---------------------------------------------------------------

// DeviceCharacterBufferedUsart1::implPortInit(void)
// must be implemented in hal/...

// ===== ISRs =================================================================

// Usart1_[RX|TX]_contextHandler(void)
// must implemented in hal/...

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1) */
