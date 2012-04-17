/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERI2C_H_
#define DEVICECHARACTERI2C_H_

#include "portable/kernel/include/uOS.h"

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE                 (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_RXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERI2C_RXBUF_SIZE                 (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_HIGH_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_HIGH_WATER_MARK      (OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE*3/4)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_LOW_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_LOW_WATER_MARK       (OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE*1/4)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH
#define OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH             (true)
#endif

class DeviceCharacterI2C : public OSDeviceCharacterBuffered
{
public:
  DeviceCharacterI2C(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned short txHWM, unsigned short txLWM, unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM);

  DeviceCharacterI2C(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned char *pRxBuf, unsigned short rxBufSize);

  void
  interruptServiceRoutine(void);

  virtual
  ~DeviceCharacterI2C();

  void
  interruptTimerTick(void);

  static DeviceCharacterI2C *pDevice;

private:

  // port implementation routines
  virtual int
  implPortInit(void);
  // called from implOpen()

  virtual int
  implPortDisable(void);
  // called from implClose()

  virtual unsigned char
  implPortRead(void);
  // called from interruptRxServiceRoutine()

  virtual void
  implPortWrite(unsigned char b);
  // called from interruptTxServiceRoutine()

  virtual void
  implResumeReception(void);

  virtual void
  implInterruptTxEnable(void);
  // called from implWriteByte() in critical section
  // called from implFlush() in critical section

  virtual void
  implInterruptTxDisable(void);

  unsigned short m_timeoutCounterTicks;
};



#endif /*DEVICECHARACTERI2C_H_*/
