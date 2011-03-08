/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICECHARACTERBUFFERED)

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacterBuffered.h"

// ----- constructors ---------------------------------------------------------

OSDeviceCharacterBuffered::OSDeviceCharacterBuffered(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
  m_rxBuf(pRxBuf, rxBufSize, rxHWM, rxLWM), m_txBuf(pTxBuf, txBufSize, txHWM,
      txLWM)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacterBuffered()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

OSDeviceCharacterBuffered::OSDeviceCharacterBuffered(unsigned char *pRxBuf,
    unsigned short rxBufSize, unsigned char *pTxBuf, unsigned short txBufSize) :
  m_rxBuf(pRxBuf, rxBufSize, rxBufSize * 3 / 4, rxBufSize / 4), m_txBuf(pTxBuf,
      txBufSize, txBufSize * 3 / 4, txBufSize / 4)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacterBuffered()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

void
OSDeviceCharacterBuffered::init(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char *pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM)
{
#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  m_type = OS_DEVICECHARACTER_USART;
#endif

  m_txBuf.init(pTxBuf, txBufSize, txHWM, txLWM);
  m_rxBuf.init(pRxBuf, rxBufSize, rxHWM, rxLWM);
}

// ----- open/close -----------------------------------------------------------

int
OSDeviceCharacterBuffered::implOpen(void)
{
#if defined(DEBUG)
  OSScheduler::criticalEnter();
    {
      // TODO : eventually define and use lockEnter()/lockExit()
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacterBuffered::implOpen()"));
      OSDeviceDebug::putNewLine();
    }
  OSScheduler::criticalExit();
#endif /* defined(DEBUG) */

  //assert(ms_pThis == this);

  implPortInit();

  return 0;
}

int
OSDeviceCharacterBuffered::implClose(void)
{
#if defined(DEBUG)
  // TODO : eventually define and use lockEnter()/lockExit()
  OSScheduler::criticalEnter();
    {
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacterBuffered::implClose()"));
      OSDeviceDebug::putNewLine();
    }
  OSScheduler::criticalExit();
#endif /* defined(DEBUG) */

  // TODO: probably should wait queue to drain and disable port
  implPortDisable();

  return 0;
}

bool
OSDeviceCharacterBuffered::implIsConnected() const
{
  // TODO: implement connection mechanisms
  return true;
}

int
OSDeviceCharacterBuffered::implPortDisable(void)
{
  return OSReturn::OS_OK;
}

// ----- read -----------------------------------------------------------------

bool
OSDeviceCharacterBuffered::implCanRead()
{
  bool f;
  OSScheduler::criticalEnter();
    {
      f = m_rxBuf.isEmpty();
    }
  OSScheduler::criticalExit();

  return !f;
}

int
OSDeviceCharacterBuffered::implAvailableRead()
{
  int r;
  OSScheduler::criticalEnter();
    {
      r = m_rxBuf.length();
    }
  OSScheduler::criticalExit();

  return r;
}

int
OSDeviceCharacterBuffered::implReadByte(void)
{
  int r;
  OSScheduler::criticalEnter();
    {
      r = m_rxBuf.get();
    }
  OSScheduler::criticalExit();

  return r;
}

void
OSDeviceCharacterBuffered::interruptRxServiceRoutine(void)
{
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_RX_ISR) && false
  OSDeviceDebug::putChar('#');
  // OSDeviceDebug::putPtr(this);
  // OSDeviceDebug::putChar(' ');
  //m_rxBuf.dump();
#endif

  unsigned char c;

  c = implPortRead();

  if (!m_rxBuf.isFull())
    {
      //OSDeviceDebug::putChar(c);
      m_rxBuf.put(c);
    }
  else
    {
      ; // character is lost, no place to store it in the receive queue
      OSDeviceDebug::putChar('-');
    }
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_RX_ISR_EVENT) && false
  OSDeviceDebug::putHex((unsigned short)getReadEvent());
  OSDeviceDebug::putNewLine();
#endif

  OSScheduler::eventNotify(getReadEvent());
}

// ----- write ---------------------------------------------------------------

bool
OSDeviceCharacterBuffered::implCanWrite(void)
{
  //OSDeviceDebug::putString("OSDeviceCharacterBuffered::implCanWrite() ");
  //OSDeviceDebug::putNewLine();
  bool f;
  OSScheduler::criticalEnter();
    {
      f = !m_txBuf.isFull();
    }
  OSScheduler::criticalExit();

  return f;
}

int
OSDeviceCharacterBuffered::implWriteByte(unsigned char b)
{
  OSScheduler::criticalEnter();
    {
      m_txBuf.put(b);

      if (m_txBuf.isAboveHighWM())
        {
          implInterruptTxEnable(); // start transmission
        }
    }
  OSScheduler::criticalExit();

  return b;
}

int
OSDeviceCharacterBuffered::implFlush(void)
{
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_IMPLFLUSH)
  OSDeviceDebug::putChar('f');
#endif
  OSScheduler::criticalEnter();
    {
      if (!m_txBuf.isEmpty())
        {
          implInterruptTxEnable();
        }
    }
  OSScheduler::criticalExit();

  return 0;
}

void
OSDeviceCharacterBuffered::interruptTxServiceRoutine(void)
{
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_TX_ISR)
  OSDeviceDebug::putChar('|');
#if false
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putChar(' ');
  m_txBuf.dump();
#endif
#endif

  if (m_txBuf.isEmpty())
    {
      implInterruptTxDisable();
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_TX_ISR)
      OSDeviceDebug::putChar('<');
      OSDeviceDebug::putNewLine();
#endif
    }
  else
    {
      unsigned char c;
      c = m_txBuf.get();
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_TX_ISR)
      OSDeviceDebug::putHex(c);
#endif
      implPortWrite(c);

      if (m_txBuf.isBelowLowWM())
        {
          OSScheduler::eventNotify(getWriteEvent());
          //OSDeviceDebug::putChar('n');
        }
    }
}

#endif /* OS_INCLUDE_OSDEVICECHARACTERBUFFERED */

