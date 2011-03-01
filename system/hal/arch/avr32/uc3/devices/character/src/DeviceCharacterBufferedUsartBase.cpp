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
DeviceCharacterBufferedUsartBase::setPortAddress(volatile avr32_usart_t* pPort)
{
  m_pPort = pPort;
}

void
DeviceCharacterBufferedUsartBase::interruptServiceRoutine(void)
{
  // check if the USART Transmit Shift Register and the USART Transmit
  //         Holding Register are free
  if ((m_pPort->csr & AVR32_USART_CSR_TXRDY_MASK) != 0)
    {
      interruptTxServiceRoutine();
    }
  // check if the USART Receive Holding Register is full
  if ((m_pPort->csr & AVR32_USART_CSR_RXRDY_MASK) != 0)
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
  m_pPort->ier = AVR32_USART_IER_TXRDY_MASK;
}

void
DeviceCharacterBufferedUsartBase::implInterruptTxDisable(void)
{
  m_pPort->idr = AVR32_USART_IER_TXRDY_MASK;
}

unsigned char
DeviceCharacterBufferedUsartBase::implPortRead(void)
{
  return (unsigned char) (m_pPort->rhr & AVR32_USART_RHR_RXCHR_MASK)
      >> AVR32_USART_RHR_RXCHR_OFFSET;
}

void
DeviceCharacterBufferedUsartBase::implPortWrite(unsigned char b)
{
  m_pPort->thr = (b << AVR32_USART_THR_TXCHR_OFFSET)
      & AVR32_USART_THR_TXCHR_MASK;
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

