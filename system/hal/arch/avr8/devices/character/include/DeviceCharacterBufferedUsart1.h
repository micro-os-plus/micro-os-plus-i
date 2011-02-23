/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERBUFFEREDSART1_H_
#define HAL_DEVICECHARACTERBUFFEREDSART1_H_

#include "portable/devices/character/include/OSDeviceCharacterBuffered.h"

#include "avr/io.h"

class DeviceCharacterBufferedUsart1 : public OSDeviceCharacterBuffered
{
public:
  DeviceCharacterBufferedUsart1(void);

  DeviceCharacterBufferedUsart1(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  DeviceCharacterBufferedUsart1(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned char *pTxBuf, unsigned short txBufSize);


private:

  // actual port implementation routines

  virtual int
  implPortInit(void);
  virtual int
  implPortDisable(void);

  virtual void
  implInterruptTxEnable(void);
  virtual void
  implInterruptTxDisable(void);

  virtual unsigned char
  implPortRead(void);

  virtual void
  implPortWrite(unsigned char b);

public:
  static DeviceCharacterBufferedUsart1 * ms_pThis;
  
};

#endif /* HAL_DEVICECHARACTERBUFFEREDSART1_H_ */
