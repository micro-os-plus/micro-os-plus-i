/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef CIRCULARBYTEBUFFER_H_
#define CIRCULARBYTEBUFFER_H_

#include "portable/kernel/include/OS_Defines.h"

class CircularByteBuffer
{
public:
  CircularByteBuffer();
  CircularByteBuffer(unsigned char* pc, unsigned short size,
      unsigned short highWM, unsigned short lowWM);

  void
  initialise(unsigned char* pc, unsigned short size, unsigned short highWM,
      unsigned short lowWM);

  void
  initialise(unsigned char* pc, unsigned short size);

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
  isEmpty(void) const;
  bool
  isFull(void) const;
  bool
  isAboveHighWM(void) const;
  bool
  isBelowLowWM(void) const;

  unsigned short
  length(void) const;
  unsigned short
  size(void) const;

  void
  dump(void);

private:
  unsigned char* m_pBuf;
  unsigned short m_sz;
  unsigned short volatile m_len;
  unsigned short m_highWM;
  unsigned short m_lowWM;
  unsigned char* volatile m_pPut;
  unsigned char* volatile m_pGet;
};

inline bool
CircularByteBuffer::isEmpty(void) const
{
  return (m_len == 0);
}

inline bool
CircularByteBuffer::isFull(void) const
{
  return (m_len == m_sz);
}

inline bool
CircularByteBuffer::isAboveHighWM(void) const
{
  return (m_len > m_highWM);
}

inline bool
CircularByteBuffer::isBelowLowWM(void) const
{
  return (m_len < m_lowWM);
}

inline unsigned short
CircularByteBuffer::length(void) const
{
  return m_len;
}

inline unsigned short
CircularByteBuffer::size(void) const
{
  return m_sz;
}

#endif /* CIRCULARBYTEBUFFER_H_ */
