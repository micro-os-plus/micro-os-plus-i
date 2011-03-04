/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

// DEPRECATED

#if false && defined(OS_INCLUDE_DEVICECHARACTERUSART)

#include "portable/devices/character/include/DeviceCharacterUsart.h"

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)

// ----- static data ---------------------------------------------------------

DeviceCharacterUsart * DeviceCharacterUsart::ms_pThis;

#endif

// ----- constructors --------------------------------------------------------

DeviceCharacterUsart::DeviceCharacterUsart(void)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("DeviceCharacterUsart()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

void DeviceCharacterUsart::init(unsigned char *pTxBuf,
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

// set custom high/low water marks
DeviceCharacterUsart::DeviceCharacterUsart(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char *pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM) :
m_txBuf(pTxBuf, txBufSize, txHWM, txLWM), m_rxBuf(pRxBuf, rxBufSize, rxHWM,
    rxLWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("DeviceCharacterUsart()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
    ms_pThis = this;
#endif
  }

// use default 3/4 high and 1/4 low water marks
DeviceCharacterUsart::DeviceCharacterUsart(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned char *pRxBuf, unsigned short rxBufSize) :
m_txBuf(pTxBuf, txBufSize, txBufSize * 3/ 4, txBufSize / 4), m_rxBuf(pRxBuf,
    rxBufSize, rxBufSize * 3/ 4, rxBufSize / 4)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("DeviceCharacterUsart()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
    ms_pThis = this;
#endif
  }

int DeviceCharacterUsart::implOpen(void)
  {
    OSDeviceDebug::putString("DeviceCharacterUsart::implOpen()");
    OSDeviceDebug::putNewLine();

    //assert(ms_pThis == this);

    implPortInit();
    return 0;
  }

OSEvent_t DeviceCharacterUsart::implGetOpenEvent(void)
  {
    return OSEvent::OS_NONE;
  }

int DeviceCharacterUsart::implClose(void)
  {
    // TODO: probably should wait queue to drain and disable port

    return 0;
  }

bool DeviceCharacterUsart::implIsConnected() const
  {
    return true;
  }

// ----- write ---------------------------------------------------------------

bool DeviceCharacterUsart::implCanWrite(void)
  {
    //OSDeviceDebug::putString("DeviceCharacterUsart::implCanWrite() ");
    //OSDeviceDebug::putNewLine();

    return !m_txBuf.isFull();
  }

OSEvent_t DeviceCharacterUsart::implGetWriteEvent(void)
  {
    return (OSEvent_t)&m_txBuf;
  }

int DeviceCharacterUsart::implWriteByte(unsigned char b)
  {
    m_txBuf.put(b);

    if (m_txBuf.isAboveHighWM())
      {
        implInterruptTxEnable(); // start transmission
      }

    return b;
  }

int DeviceCharacterUsart::implFlush(void)
  {
    //OSDeviceDebug::putChar('f');
    if (!m_txBuf.isEmpty())
      {
        implInterruptTxEnable();
      }
    return 0;
  }

void DeviceCharacterUsart::interruptTxServiceRoutine(void)
  {
#if 0
    OSDeviceDebug::putChar('!');
    OSDeviceDebug::putPtr(ms_pThis);
    OSDeviceDebug::putChar(' ');
    ms_pThis->m_txBuf.dump();
#endif

    if (m_txBuf.isEmpty())
      {
        implInterruptTxDisable();
      }
    else
      {
        unsigned char c;
        c = m_txBuf.get();
        //OSDeviceDebug::putChar(c);
        implPortWrite(c);

        if (m_txBuf.isBelowLowWM())
          {
            OSScheduler::eventNotify(getWriteEvent());
            //OSDeviceDebug::putChar('n');
          }
      }
  }

// ----- read ----------------------------------------------------------------

bool DeviceCharacterUsart::implCanRead()
  {
    return !m_rxBuf.isEmpty();
  }

int DeviceCharacterUsart::implAvailableRead()
  {
    return m_rxBuf.length();
  }

OSEvent_t DeviceCharacterUsart::implGetReadEvent(void)
  {
    return (OSEvent_t)&m_rxBuf;
  }

int DeviceCharacterUsart::implReadByte(void)
  {
    return m_rxBuf.get();
  }

void DeviceCharacterUsart::interruptRxServiceRoutine(void)
  {
#if 0
    OSDeviceDebug::putChar('#');
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putChar(' ');
    m_rxBuf.dump();
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
      }
#if 0
    OSDeviceDebug::putHex((unsigned short)getReadEvent());
    OSDeviceDebug::putNewLine();
#endif

    OSScheduler::eventNotify(getReadEvent());
  }

#endif /*OS_INCLUDE_DEVICECHARACTERUSART*/

