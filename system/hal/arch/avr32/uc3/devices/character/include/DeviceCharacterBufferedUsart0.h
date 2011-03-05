/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERBUFFEREDUSART0_H_
#define HAL_DEVICECHARACTERBUFFEREDUSART0_H_

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsartBase.h"

class DeviceCharacterBufferedUsart0 : public DeviceCharacterBufferedUsartBase
{
public:
  DeviceCharacterBufferedUsart0(unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
      unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM);

      DeviceCharacterBufferedUsart0(unsigned char *pRxBuf,
          unsigned short rxBufSize, unsigned char *pTxBuf,
          unsigned short txBufSize);

public:
  virtual int
  implPortInit(void);

  static DeviceCharacterBufferedUsart0 * ms_pThis;

};

#endif /* HAL_DEVICECHARACTERBUFFEREDUSART0_H_ */
