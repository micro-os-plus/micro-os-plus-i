/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CIRCULARBYTEBUFFER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/CircularByteBuffer.h"

CircularByteBuffer::CircularByteBuffer()
{
  OSDeviceDebug::putConstructor_P(PSTR("CircularByteBuffer"), this);
}

CircularByteBuffer::CircularByteBuffer(unsigned char* pBuf,
    unsigned short size, unsigned short highWM, unsigned short lowWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("CircularByteBuffer"), this);

  init(pBuf, size, highWM, lowWM);
}

void
CircularByteBuffer::init(unsigned char* pBuf, unsigned short size,
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

void
CircularByteBuffer::clear(void)
{
  m_pPut = m_pGet = m_pBuf;
  m_len = 0;
}

void
CircularByteBuffer::put(unsigned char c)
{
  *m_pPut++ = c;
  if ((unsigned short) (m_pPut - m_pBuf) >= m_sz)
    m_pPut = m_pBuf;
  m_len++;
}

// return the actual number of bytes, if not enough space for all
unsigned short
CircularByteBuffer::putBytes(const unsigned char* pBuf, unsigned short count)
{
  unsigned short endSz, sz;

  sz = count;
  if (count > (m_sz - m_len))
    sz = m_sz - m_len;

  endSz = (unsigned short) (m_sz - (m_pPut - m_pBuf));
  if (endSz >= sz)
    {
      memcpy(m_pPut, pBuf, sz);
      m_pPut += sz;
      if ((unsigned short) (m_pPut - m_pBuf) >= m_sz)
        m_pPut = m_pBuf;
      m_len += sz;
    }
  else
    {
      memcpy(m_pPut, pBuf, endSz);
      m_pPut = m_pBuf;
      memcpy(m_pPut, pBuf + endSz, sz - endSz);
      m_pPut += (sz - endSz);
      m_len += sz;
    }
  return sz;
}

unsigned short
CircularByteBuffer::getBytes(unsigned char* pBuf, unsigned short size)
{
  unsigned short ln, endSz;

  endSz = m_sz - (unsigned short) (m_pGet - m_pBuf);
  ln = m_len;
  if (ln > size)
    ln = size;
  if (endSz >= ln)
    {
      memcpy(pBuf, m_pGet, ln);
      m_pGet += ln;
      if ((unsigned short) (m_pGet - m_pBuf) >= m_sz)
        m_pGet = m_pBuf;
      m_len -= ln;
    }
  else
    {
      memcpy(pBuf, m_pGet, endSz);
      m_pGet = m_pBuf;
      memcpy(pBuf + endSz, m_pGet, ln - endSz);
      m_pGet += (ln - endSz);
      m_len -= ln;
    }
  return ln;
}

unsigned char
CircularByteBuffer::get(void)
{
  unsigned char c;
  c = *m_pGet++;
  if ((unsigned short) (m_pGet - m_pBuf) >= m_sz)
    m_pGet = m_pBuf;
  m_len--;
  return c;
}

#if defined(OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP)

void
CircularByteBuffer::dump(void)
{
  OSDeviceDebug::putString_P(PSTR("CircularByteBuffer *"));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putString_P(PSTR("=("));
  OSDeviceDebug::putPtr(m_pBuf);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putHex((unsigned short) m_len);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putHex((unsigned short) m_sz);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putHex((unsigned short) m_highWM);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putHex((unsigned short) m_lowWM);
  OSDeviceDebug::putChar(')');
  OSDeviceDebug::putNewLine();
}

#endif /* OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP */

#endif
