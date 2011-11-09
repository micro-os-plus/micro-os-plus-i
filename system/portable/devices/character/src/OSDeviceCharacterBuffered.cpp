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

OSDeviceCharacterBuffered::OSDeviceCharacterBuffered(unsigned char* pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    unsigned char* pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
  m_rxBuf(pRxBuf, rxBufSize, rxHWM, rxLWM),
      m_txBuf(pTxBuf, txBufSize, txHWM, txLWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceCharacterBuffered"), this);

  m_sending = false;
}

OSDeviceCharacterBuffered::OSDeviceCharacterBuffered(unsigned char* pRxBuf,
    unsigned short rxBufSize, unsigned char* pTxBuf, unsigned short txBufSize) :
  m_rxBuf(pRxBuf, rxBufSize, rxBufSize * 3 / 4, rxBufSize / 4),
      m_txBuf(pTxBuf, txBufSize, txBufSize * 3 / 4, txBufSize / 4)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceCharacterBuffered"), this);

  m_sending = false;
}

void
OSDeviceCharacterBuffered::init(unsigned char* pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char* pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM)
{
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)
  m_type = OS_DEVICECHARACTER_USART;
#endif

  m_txBuf.initialise(pTxBuf, txBufSize, txHWM, txLWM);
  m_rxBuf.initialise(pRxBuf, rxBufSize, rxHWM, rxLWM);
}

// ----- open/close -----------------------------------------------------------

int
OSDeviceCharacterBuffered::implOpen(void)
{
#if defined(DEBUG)
  // TODO : eventually define and use lockEnter()/lockExit()
  OSDeviceDebug::putString_P(PSTR(" OSDeviceCharacterBuffered::implOpen() "));
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
  OSDeviceDebug::putString_P(PSTR(" OSDeviceCharacterBuffered::implClose() "));
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
  OSCriticalSection::enter();
    {
      f = m_rxBuf.isEmpty();
    }
  OSCriticalSection::exit();

  return !f;
}

int
OSDeviceCharacterBuffered::implAvailableRead()
{
  int r;
  OSCriticalSection::enter();
    {
      r = m_rxBuf.length();
    }
  OSCriticalSection::exit();

  return r;
}

int
OSDeviceCharacterBuffered::implReadByte(void)
{
  int r;
  OSCriticalSection::enter();
    {
      r = m_rxBuf.get();
    }
  OSCriticalSection::exit();

  return r;
}

int
OSDeviceCharacterBuffered::implReadBytes(unsigned char* pBuf, int size)
{
  int r;
  OSCriticalSection::enter();
    {
      r = m_rxBuf.getBytes(pBuf, size);
    }
  OSCriticalSection::exit();

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

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  if ((m_pReadMatchArray == 0) || m_rxBuf.isAboveHighWM() || (m_countToRead
          == 0))
    {
      // if the match array is not set or buffer is near full or single char read, notify every character
      m_pReadMatchArray = 0;
      m_countToRead = 0;
      OSScheduler::eventNotify(getReadEvent());
    }
  else
    {
      if (m_pReadMatchArray != 0)
        {
          // if there is enough space, check if the character is
          // in the given array
          unsigned char* p;
          for (p = m_pReadMatchArray; *p != '\0'; ++p)
            {
              if (*p == c)
                {
                  // if we have a match, notify
                  m_pReadMatchArray = 0;
                  m_countToRead = 0;
                  OSScheduler::eventNotify(getReadEvent());
                  break;
                }
            }
        }

      if ((m_countToRead != 0) && (m_rxBuf.length() >= m_countToRead))
        {
          m_pReadMatchArray = 0;
          m_countToRead = 0;
          OSScheduler::eventNotify(getReadEvent());
        }

    }
#else /* !defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
  OSScheduler::eventNotify(getReadEvent());
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
}

// ----- write ---------------------------------------------------------------

bool
OSDeviceCharacterBuffered::implCanWrite(void)
{
  //OSDeviceDebug::putString("OSDeviceCharacterBuffered::implCanWrite() ");
  //OSDeviceDebug::putNewLine();
  bool f;
  OSCriticalSection::enter();
    {
      f = !m_txBuf.isFull();
    }
  OSCriticalSection::exit();

  return f;
}

int
OSDeviceCharacterBuffered::implWriteByte(unsigned char b)
{
  OSCriticalSection::enter();
    {
      m_txBuf.put(b);

      if (m_txBuf.isAboveHighWM())
        {
          implInterruptTxEnable(); // start transmission
          m_sending = true;
        }
    }
  OSCriticalSection::exit();

  return b;
}

int
OSDeviceCharacterBuffered::implWriteBytes(const unsigned char* buf, int len)
{
  int i;
  OSCriticalSection::enter();
    {
      i = m_txBuf.putBytes(buf, len);

      if (m_txBuf.isAboveHighWM())
        {
          implInterruptTxEnable(); // start transmission
          m_sending = true;
        }
    }
  OSCriticalSection::exit();

  return i;
}

int
OSDeviceCharacterBuffered::implFlush(void)
{
#if defined(OS_DEBUG_OSDEVICECHARACTERBUFFERED_IMPLFLUSH)
  OSDeviceDebug::putChar('f');
#endif
  OSCriticalSection::enter();
    {
      if (!m_txBuf.isEmpty())
        {
          implInterruptTxEnable();
        }
    }
  OSCriticalSection::exit();

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
      m_sending = false;
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

bool
OSDeviceCharacterBuffered::implIsSending(void)
{
  return m_sending;
}

#endif /* OS_INCLUDE_OSDEVICECHARACTERBUFFERED */

