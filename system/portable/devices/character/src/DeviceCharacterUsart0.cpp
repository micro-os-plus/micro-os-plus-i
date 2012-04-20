/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

// DEPRECATED

#if defined(OS_INCLUDE_DEVICECHARACTERUSART0)

#include "portable/devices/character/include/DeviceCharacterUsart0.h"

// ----- static data ---------------------------------------------------------

DeviceCharacterUsart0* DeviceCharacterUsart0::ms_pThis;

// ----- constructors --------------------------------------------------------

// set custom high/low water marks
DeviceCharacterUsart0::DeviceCharacterUsart0(unsigned char* pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    unsigned char* pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
    OSDeviceCharacterBuffered(pRxBuf, rxBufSize, rxHWM, rxLWM, pTxBuf,
        txBufSize, txHWM, txLWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterUsart0"), this);

  ms_pThis = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterUsart0::DeviceCharacterUsart0(unsigned char* pRxBuf,
    unsigned short rxBufSize, unsigned char* pTxBuf, unsigned short txBufSize) :
    OSDeviceCharacterBuffered(pRxBuf, rxBufSize, pTxBuf, txBufSize)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterUsart0"), this);

  ms_pThis = this;
}

DeviceCharacterUsart0::~DeviceCharacterUsart0()
{
  OSDeviceDebug::putDestructor_P(PSTR("DeviceCharacterUsart0"), this);
}

#endif /* OS_INCLUDE_DEVICECHARACTERUSART0 */
