/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

// DEPRECATED

#if false && defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART0)

#include "portable/devices/character/include/DeviceCharacterMultiUsart0.h"

// ----- static data ---------------------------------------------------------

DeviceCharacterMultiUsart0* DeviceCharacterMultiUsart0::ms_pThis;

// ----- constructors --------------------------------------------------------

// set custom high/low water marks
DeviceCharacterMultiUsart0::DeviceCharacterMultiUsart0(unsigned char* pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char* pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM) :
DeviceCharacterUsart(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf, rxBufSize,
    rxHWM, rxLWM)
  {
    OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterMultiUsart0"), this);

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
    ms_pThis = this;
#endif
  }

// use default 3/4 high and 1/4 low water marks
DeviceCharacterMultiUsart0::DeviceCharacterMultiUsart0(unsigned char* pTxBuf,
    unsigned short txBufSize, unsigned char* pRxBuf, unsigned short rxBufSize) :
DeviceCharacterUsart(pTxBuf, txBufSize, pRxBuf, rxBufSize)
  {
    OSDeviceDebug::putConstructor_P("DeviceCharacterMultiUsart0"), this);

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
    ms_pThis = this;
#endif
  }

#endif /* OS_INCLUDE_DEVICECHARACTERMULTIUSART0 */
