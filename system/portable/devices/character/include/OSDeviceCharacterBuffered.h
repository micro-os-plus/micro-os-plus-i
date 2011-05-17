/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICECHARACTERUBUFFERED_H_
#define OSDEVICECHARACTERUBUFFERED_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"
#include "portable/misc/include/CircularByteBuffer.h"

class OSDeviceCharacterBuffered : public OSDeviceCharacter
{
public:
  OSDeviceCharacterBuffered(unsigned char* pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char* pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  OSDeviceCharacterBuffered(unsigned char* pRxBuf, unsigned short rxBufSize,
      unsigned char* pTxBuf, unsigned short txBufSize);

  void
  init(unsigned char* pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM, unsigned char* pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM);

  void
  interruptRxServiceRoutine(void);
  void
  interruptTxServiceRoutine(void);

  bool
  isSending(void);

protected:
  CircularByteBuffer m_rxBuf;
  CircularByteBuffer m_txBuf;

  bool m_sending;

private:
  // open/close
  virtual int
  implOpen(void);
  virtual int
  implClose(void);
  virtual bool
  implIsConnected(void) const;

  virtual bool
  implIsSending(void);

  // read
  virtual bool
  implCanRead(void);
  virtual int
  implAvailableRead(void);
  virtual int
  implReadByte(void);
  virtual int
  implReadBytes(unsigned char* pBuf, int size);

  // write
  virtual bool
  implCanWrite(void);
  virtual int
  implWriteByte(unsigned char b);
  virtual int
  implWriteBytes(const unsigned char* buf, int len);
  virtual int
  implFlush(void);

  // port implementation routines
  virtual int
  implPortInit(void) = 0;
  // called from implOpen()

  virtual int
  implPortDisable(void);
  // called from implClose()

  virtual void
  implInterruptTxEnable(void) = 0;
  // called from implWriteByte() in critical section
  // called from implFlush() in critical section

  virtual void
  implInterruptTxDisable(void) = 0;
  // called from interruptTxServiceRoutine()

  virtual unsigned char
  implPortRead(void) = 0;
  // called from interruptRxServiceRoutine()

  virtual void
  implPortWrite(unsigned char b) = 0;
  // called from interruptTxServiceRoutine()
};

#endif /* OSDEVICECHARACTERUBUFFERED_H_ */
