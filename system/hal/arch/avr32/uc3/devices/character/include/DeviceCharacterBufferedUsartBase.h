/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_
#define HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_

#include "portable/devices/character/include/OSDeviceCharacterBuffered.h"

#include <avr32/io.h>

class DeviceCharacterBufferedUsartBase : public OSDeviceCharacterBuffered
{
public:
  DeviceCharacterBufferedUsartBase(void);

  DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
      unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
      unsigned short txLWM);

      DeviceCharacterBufferedUsartBase(unsigned char *pRxBuf,
          unsigned short rxBufSize, unsigned char *pTxBuf,
          unsigned short txBufSize);

  // Used to set actual port address
  void
  setPortAddress(volatile avr32_usart_t* pPort);

  void
  interruptServiceRoutine(void);

private:

  // actual port implementation routines

  virtual int
  implPortInit(void) = 0;
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
  volatile avr32_usart_t* m_pPort;

};
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline void usart_int_tx_enable(volatile avr32_usart_t *usart)
{
  usart->ier =  AVR32_USART_IER_TXRDY_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline void usart_int_tx_disable(volatile avr32_usart_t *usart)
{
  usart->idr =  AVR32_USART_IDR_TXRDY_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline void usart_int_rx_enable(volatile avr32_usart_t *usart)
{
  usart->ier =  AVR32_USART_IER_RXRDY_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline void usart_int_rx_disable(volatile avr32_usart_t *usart)
{
  usart->idr =  AVR32_USART_IDR_RXRDY_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline unsigned char usart_basic_read_char(volatile avr32_usart_t *usart)
{
 return (unsigned char)(usart->rhr & AVR32_USART_RHR_RXCHR_MASK) >> AVR32_USART_RHR_RXCHR_OFFSET;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
inline void usart_basic_write_char(volatile avr32_usart_t *usart, int c)
{
  usart->thr = (c << AVR32_USART_THR_TXCHR_OFFSET) & AVR32_USART_THR_TXCHR_MASK;
}

#endif /* HAL_DEVICECHARACTERBUFFEREDSARTBASE_H_ */
