/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CIRCULARBLOCKBUFFER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/CircularBlockBuffer.h"
#include <string.h>

CircularBlockBuffer::CircularBlockBuffer()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CircularBlockBuffer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

CircularBlockBuffer::CircularBlockBuffer(unsigned char *pBuf,
    unsigned short blklen, unsigned short blocks, unsigned short highWM,
    unsigned short lowWM)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CircularBlockBuffer()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    init(pBuf, blklen, blocks, highWM, lowWM);
  }

void CircularBlockBuffer::init(unsigned char *pBuf, unsigned short blklen,
    unsigned short blocks, unsigned short highWM, unsigned short lowWM)
  {
    m_pBuf = pBuf;
    m_blkLen = blklen;
    m_szBlks = blocks;
    m_highWM = highWM;
    m_lowWM = lowWM;

    clear();
  }

void CircularBlockBuffer::clear(void)
  {
    m_pPut = m_pGet = m_pBuf;
    m_len = 0;
  }

void CircularBlockBuffer::put(unsigned char *pc)
  {
    memcpy(m_pPut, pc, m_blkLen);
    m_pPut += m_blkLen;

    if (((unsigned short)(m_pPut - m_pBuf))/m_blkLen >= m_szBlks)
      m_pPut = m_pBuf;

    m_len++;
  }

unsigned char *CircularBlockBuffer::get(unsigned char *pc)
  {
    memcpy(pc, m_pGet, m_blkLen);
    m_pGet += m_blkLen;

    if (((unsigned short)(m_pGet - m_pBuf))/m_blkLen >= m_szBlks)
      m_pGet = m_pBuf;

    m_len--;

    return pc;
  }

#endif

