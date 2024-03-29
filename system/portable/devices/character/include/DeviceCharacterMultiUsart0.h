/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERMULTIUSART0_H_
#define DEVICECHARACTERMULTIUSART0_H_

#include "portable/devices/character/include/DeviceCharacterUsart.h"

#ifndef OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_SIZE                  (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERMULTIUSART0_RXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERMULTIUSART0_RXBUF_SIZE                  (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_HIGH_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_HIGH_WATER_MARK       (OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_SIZE*3/4)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_LOW_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_LOW_WATER_MARK        (OS_CFGINT_DEVICECHARACTERMULTIUSART0_TXBUF_SIZE*1/4)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERMULTIUSART0_BAUD_RATE
#define OS_CONFIG_DEVICECHARACTERMULTIUSART0_BAUD_RATE                   (9600)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERMULTIUSART0_DOUBLE_SPEED
#define OS_CONFIG_DEVICECHARACTERMULTIUSART0_DOUBLE_SPEED                (1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_TX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_TX_CONTEXT_SWITCH          (1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_RX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_RX_CONTEXT_SWITCH          (1)
#endif

class DeviceCharacterMultiUsart0 : public DeviceCharacterUsart
{
public:
  DeviceCharacterMultiUsart0(unsigned char* pTxBuf, unsigned short txBufSize,
      unsigned short txHWM, unsigned short txLWM, unsigned char* pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM);

  DeviceCharacterMultiUsart0(unsigned char* pTxBuf, unsigned short txBufSize,
      unsigned char* pRxBuf, unsigned short rxBufSize);

private:
  virtual void
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
  static DeviceCharacterMultiUsart0* ms_pThis;
};

#endif /* DEVICECHARACTERMULTIUSART0_H_ */
