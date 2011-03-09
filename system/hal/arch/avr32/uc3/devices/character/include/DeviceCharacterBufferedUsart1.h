/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERBUFFEREDUSART1_H_
#define HAL_DEVICECHARACTERBUFFEREDUSART1_H_

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsartBase.h"

// ----------------------------------------------------------------------------

// Provide reasonable defaults

#if !defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE               (9600)
#endif /* !define(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE) */

#if !defined(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_RX_PIN)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_RX_PIN                  (5)
#endif /* !defined(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_RX_PIN) */

#if !defined(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_TX_PIN)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_TX_PIN                  (6)
#endif /* !defined(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_TX_PIN) */

#if !defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION           (0)
#endif /* !defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION) */

#if !defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_IRQ_PRIORITY)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_IRQ_PRIORITY            (0)
#endif /* !defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_IRQ_PRIORITY) */

// ----------------------------------------------------------------------------

class DeviceCharacterBufferedUsart1 : public DeviceCharacterBufferedUsartBase
{
public:
  DeviceCharacterBufferedUsart1(unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
      unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM);

      DeviceCharacterBufferedUsart1(unsigned char *pRxBuf,
          unsigned short rxBufSize, unsigned char *pTxBuf,
          unsigned short txBufSize);

public:
  virtual int
  implPortInit(void);

  static void
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)*/
  contextHandler(void);

  static DeviceCharacterBufferedUsart1 * ms_pThis;

};

#endif /* HAL_DEVICECHARACTERBUFFEREDUSART1_H_ */
