/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERUSART1_H_
#define DEVICECHARACTERUSART1_H_

#include "portable/devices/character/include/OSDeviceCharacterBuffered.h"

#ifndef OS_CONFIG_DEVICECHARACTERUSART1_BAUD_RATE
#define OS_CONFIG_DEVICECHARACTERUSART1_BAUD_RATE                   (9600)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART1_DOUBLE_SPEED
#define OS_CONFIG_DEVICECHARACTERUSART1_DOUBLE_SPEED                (1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERUSART1_TX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERUSART1_TX_CONTEXT_SWITCH          (1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERUSART1_RX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERUSART1_RX_CONTEXT_SWITCH          (1)
#endif

class DeviceCharacterUsart1 : public OSDeviceCharacterBuffered
{
public:
  DeviceCharacterUsart1(unsigned char* pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char* pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  DeviceCharacterUsart1(unsigned char* pRxBuf, unsigned short rxBufSize,
      unsigned char* pTxBuf, unsigned short txBufSize);

private:
  virtual int
  implPortInit(void);
  virtual void
  implInterruptTxEnable(void);
  virtual void
  implInterruptTxDisable(void);
  virtual unsigned char
  implPortRead(void);
  virtual void
  implPortWrite(unsigned char b);

public:
  static DeviceCharacterUsart1* ms_pThis;
};

#endif /* DEVICECHARACTERUSART1_H_ */
