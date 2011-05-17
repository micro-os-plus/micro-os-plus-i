/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef CIRCULARBYTEBUFFER_H_
#define CIRCULARBYTEBUFFER_H_

#include "portable/kernel/include/OS.h"

class CircularByteBuffer
{
public:
  CircularByteBuffer();
  CircularByteBuffer(unsigned char* pc, unsigned short size,
      unsigned short highWM, unsigned short lowWM);

  void
  init(unsigned char* pc, unsigned short size, unsigned short highWM,
      unsigned short lowWM);

  void
  clear(void);

  void
  put(unsigned char c);
  unsigned char
  get(void);

  unsigned short
  putBytes(const unsigned char* pBuf, unsigned short count);
  unsigned short
  getBytes(unsigned char* pBuf, unsigned short size);

  bool
  isEmpty(void);
  bool
  isFull(void);
  bool
  isAboveHighWM(void);
  bool
  isBelowLowWM(void);

  unsigned short
  length(void);
  unsigned short
  size(void);

  void
  dump(void);

private:
  unsigned char* m_pBuf;
  unsigned short m_sz;
  unsigned short m_len;
  unsigned short m_highWM;
  unsigned short m_lowWM;
  unsigned char* m_pPut;
  unsigned char* m_pGet;
};

inline bool
CircularByteBuffer::isEmpty(void)
{
  return (m_len == 0);
}

inline bool
CircularByteBuffer::isFull(void)
{
  return (m_len == m_sz);
}

inline bool
CircularByteBuffer::isAboveHighWM(void)
{
  return (m_len > m_highWM);
}

inline bool
CircularByteBuffer::isBelowLowWM(void)
{
  return (m_len < m_lowWM);
}

inline unsigned short
CircularByteBuffer::length(void)
{
  return m_len;
}

inline unsigned short
CircularByteBuffer::size(void)
{
  return m_sz;
}

#endif /* CIRCULARBYTEBUFFER_H_ */
