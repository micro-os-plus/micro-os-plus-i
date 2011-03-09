/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_
#define HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_

#include "portable/devices/character/include/OSDeviceCharacterBuffered.h"

class DeviceCharacterBufferedUsartBase : public OSDeviceCharacterBuffered
{
public:
  DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
      unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM);

      DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf,
          unsigned short rxBufSize, unsigned char *pTxBuf,
          unsigned short txBufSize);

private:

  // actual port implementation routines

  virtual int
  implPortInit(void) = 0;
  virtual int
  implPortDisable(void) = 0;

  virtual void
  implInterruptTxEnable(void) = 0;
  virtual void
  implInterruptTxDisable(void) = 0;

  virtual unsigned char
  implPortRead(void) = 0;

  virtual void
  implPortWrite(unsigned char b) = 0;

private:

};

#endif /* HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_ */
