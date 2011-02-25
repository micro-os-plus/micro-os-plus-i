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
  OSDeviceCharacterBuffered();

  OSDeviceCharacterBuffered(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  OSDeviceCharacterBuffered(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned char *pTxBuf, unsigned short txBufSize);

  void
  init(unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM);

  void
  interruptRxServiceRoutine(void);
  void
  interruptTxServiceRoutine(void);

protected:
  CircularByteBuffer m_rxBuf;
  CircularByteBuffer m_txBuf;

private:
  // open/close
  virtual int
  implOpen(void);
  virtual int
  implClose(void);
  virtual bool
  implIsConnected(void) const;

  // read
  virtual bool
  implCanRead(void);
  virtual int
  implAvailableRead(void);
  virtual int
  implReadByte(void);

  // write
  virtual bool
  implCanWrite(void);
  virtual int
  implWriteByte(unsigned char b);
  virtual int
  implFlush(void);

  // port implementation routines
  virtual int
  implPortInit(void) = 0;
  virtual int
  implPortDisable(void);

  virtual void
  implInterruptTxEnable(void) = 0;
  virtual void
  implInterruptTxDisable(void) = 0;

  virtual unsigned char
  implPortRead(void) = 0;
  virtual void
  implPortWrite(unsigned char b) = 0;
};

#endif /* OSDEVICECHARACTERUBUFFERED_H_ */
