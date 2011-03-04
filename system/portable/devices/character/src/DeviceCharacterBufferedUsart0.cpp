/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart0.h"


// ----- static data ----------------------------------------------------------

DeviceCharacterBufferedUsart0 * DeviceCharacterBufferedUsart0::ms_pThis;

// ----- constructors ---------------------------------------------------------

DeviceCharacterBufferedUsart0::DeviceCharacterBufferedUsart0(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart0()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

// set custom high/low water marks
DeviceCharacterBufferedUsart0::DeviceCharacterBufferedUsart0(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM) :
  DeviceCharacterBufferedUsartBase(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf,
      rxBufSize, rxHWM, rxLWM)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart0()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterBufferedUsart0::DeviceCharacterBufferedUsart0(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned char *pRxBuf,
    unsigned short rxBufSize) :
  DeviceCharacterBufferedUsartBase(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart0()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// ----- inits ---------------------------------------------------------------

// DeviceCharacterBufferedUsart0::implPortInit(void)
// must be implemented in hal/...

// ===== ISRs =================================================================

// Usart0_[RX|TX]_contextHandler(void)
// must implemented in hal/...

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) */
