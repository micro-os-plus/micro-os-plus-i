/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CIRCULARBYTEBUFFER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/CircularByteBuffer.h"

CircularByteBuffer::CircularByteBuffer()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CircularByteBuffer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

CircularByteBuffer::CircularByteBuffer(unsigned char *pBuf,
    unsigned short size, unsigned short highWM, unsigned short lowWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CircularByteBuffer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    init(pBuf, size, highWM, lowWM);
  }

void CircularByteBuffer::init(unsigned char *pBuf, unsigned short size,
    unsigned short highWM, unsigned short lowWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CIRCULARBYTEBUFFER_INIT)
    OSDeviceDebug::putString_P(PSTR("CircularByteBuffer::init("));
    OSDeviceDebug::putHex((unsigned short)pBuf);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)size);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)highWM);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)lowWM);
    OSDeviceDebug::putChar(')');
    OSDeviceDebug::putNewLine();
#endif
    
    m_pBuf = pBuf;
    m_sz = size;
    m_highWM = highWM;
    m_lowWM = lowWM;

    clear();
  }

void CircularByteBuffer::clear(void)
  {
    m_pPut = m_pGet = m_pBuf;
    m_len = 0;
  }

void CircularByteBuffer::put(unsigned char c)
  {
    *m_pPut++ = c;
    if ( ( unsigned short ) (m_pPut - m_pBuf ) >= m_sz)
      m_pPut = m_pBuf;
    m_len++;
  }

unsigned char CircularByteBuffer::get(void)
  {
    unsigned char c;
    c = *m_pGet++;
    if ( ( unsigned short ) (m_pGet - m_pBuf ) >= m_sz)
      m_pGet = m_pBuf;
    m_len--;
    return c;
  }

#if defined(OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP)

void CircularByteBuffer::dump(void)
  {
    OSDeviceDebug::putString_P(PSTR("CircularByteBuffer *"));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putString_P(PSTR("=("));
    OSDeviceDebug::putPtr(m_pBuf);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)m_len);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)m_sz);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)m_highWM);
    OSDeviceDebug::putChar(',');
    OSDeviceDebug::putHex((unsigned short)m_lowWM);
    OSDeviceDebug::putChar(')');
    OSDeviceDebug::putNewLine();
  }

#endif /*OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP*/

#endif
