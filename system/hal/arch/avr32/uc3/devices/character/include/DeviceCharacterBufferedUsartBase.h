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
  DeviceCharacterBufferedUsartBase(void);

  DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned char *pTxBuf, unsigned short txBufSize);

  // Used to set actual port address
  void setPortAddress(void *pPort);


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

private:
  void * m_pPort;
  
};

#endif /* HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_ */
