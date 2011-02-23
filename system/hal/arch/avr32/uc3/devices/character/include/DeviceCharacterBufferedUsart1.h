/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERBUFFEREDUSART1_H_
#define DEVICECHARACTERBUFFEREDUSART1_H_

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsartBase.h"

class DeviceCharacterBufferedUsart1 : public DeviceCharacterBufferedUsartBase
{
public:
  DeviceCharacterBufferedUsart1();

  DeviceCharacterBufferedUsart1(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, unsigned char *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  DeviceCharacterBufferedUsart1(unsigned char *pRxBuf, unsigned short rxBufSize,
      unsigned char *pTxBuf, unsigned short txBufSize);

public:
  static DeviceCharacterBufferedUsart1 * ms_pThis;

};

#endif /* DEVICECHARACTERBUFFEREDUSART1_H_ */
