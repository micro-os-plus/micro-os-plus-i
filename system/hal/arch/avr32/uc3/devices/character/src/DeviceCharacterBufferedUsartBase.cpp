/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsartBase.h"

#include "hal/arch/avr32/uc3/lib/include/usart.h"

// ----- constructors ---------------------------------------------------------

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
DeviceCharacterBufferedUsartBase::setPortAddress( volatile avr32_usart_t* pPort)
  {
    m_pPort = pPort;
  }

void
DeviceCharacterBufferedUsartBase::interruptServiceRoutine(void)
  {
    int rc;
    rc = usart_tx_ready(m_pPort);
    if(rc)
      {
        interruptTxServiceRoutine();
      }
    rc = usart_test_hit(m_pPort);
    if(rc)
      {
        interruptRxServiceRoutine();
      }
  }

int
DeviceCharacterBufferedUsartBase::implPortDisable(void)
  {
    usart_reset(m_pPort);
    return 0;
  }

void
DeviceCharacterBufferedUsartBase::implInterruptTxEnable(void)
  {
    usart_int_tx_enable(m_pPort);
  }

void
DeviceCharacterBufferedUsartBase::implInterruptTxDisable(void)
  {
    usart_int_tx_disable(m_pPort);
  }

unsigned char
DeviceCharacterBufferedUsartBase::implPortRead(void)
  {
   return usart_basic_read_char(m_pPort);
  }

void
DeviceCharacterBufferedUsartBase::implPortWrite(unsigned char b)
  {
    usart_basic_write_char(m_pPort,b);
  }

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

