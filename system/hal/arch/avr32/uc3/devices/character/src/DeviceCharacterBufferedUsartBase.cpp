/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsartBase.h"

// ----- constructors --------------------------------------------------------

DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsartBase()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

// set custom high/low water marks
DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM) :
    OSDeviceCharacterBuffered(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf, rxBufSize,
      rxHWM, rxLWM)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsartBase()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned char *pRxBuf,
    unsigned short rxBufSize) :
    OSDeviceCharacterBuffered(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsartBase()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

void
DeviceCharacterBufferedUsartBase::setPortAddress(void *pPort)
{
  m_pPort = pPort;
}

int
DeviceCharacterBufferedUsartBase::implPortInit(void)
{
  ; // TODO: Implement it (for the moment in Usart1, not here)
  return 0;
}

int
DeviceCharacterBufferedUsartBase::implPortDisable(void)
{
  ; // TODO: Implement it (probably not needed)
  return 0;
}

void
DeviceCharacterBufferedUsartBase::implInterruptTxEnable(void)
{
  ; // TODO: Implement it
}

void
DeviceCharacterBufferedUsartBase::implInterruptTxDisable(void)
{
  ; // TODO: Implement it
}

unsigned char
DeviceCharacterBufferedUsartBase::implPortRead(void)
{
  ; // TODO: Implement it
  return 0;
}

void
DeviceCharacterBufferedUsartBase::implPortWrite(unsigned char b)
{
  ; // TODO: Implement it
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

