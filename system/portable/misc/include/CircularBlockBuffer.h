/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef CIRCULARBLOCKBUFFER_H_
#define CIRCULARBLOCKBUFFER_H_

#include "portable/kernel/include/OS.h"

class CircularBlockBuffer
  {
public:
  CircularBlockBuffer(void);
  CircularBlockBuffer(unsigned char *pc, unsigned short blklen,
      unsigned short blocks, unsigned short highWM, unsigned short lowWM);

  void init(unsigned char *pc, unsigned short blklen, unsigned short blocks,
      unsigned short highWM, unsigned short lowWM);

  void clear(void);

  void put(unsigned char *pc);
  unsigned char *get(unsigned char *pc);
  bool isEmpty(void);
  bool isFull(void);
  bool isAboveHighWM(void);
  bool isBelowLowWM(void);

  unsigned short length(void);
  unsigned short size(void);

private:
  unsigned char *m_pBuf;
  unsigned short m_blkLen;
  unsigned short m_szBlks;
  unsigned short m_len;
  unsigned short m_highWM;
  unsigned short m_lowWM;
  unsigned char *m_pPut;
  unsigned char *m_pGet;
  };

inline bool CircularBlockBuffer::isEmpty(void)
  {
    return (m_len == 0);
  }

inline bool CircularBlockBuffer::isFull(void)
  {
    return (m_len == m_szBlks);
  }

inline bool CircularBlockBuffer::isAboveHighWM(void)
  {
    return (m_len > m_highWM);
  }

inline bool CircularBlockBuffer::isBelowLowWM(void)
  {
    return (m_len < m_lowWM);
  }

inline unsigned short CircularBlockBuffer::length(void)
  {
    return m_len;
  }

inline unsigned short CircularBlockBuffer::size(void)
  {
    return m_szBlks;
  }

#endif /*CIRCULARBLOCKBUFFER_H_ */
