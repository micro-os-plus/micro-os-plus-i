/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERUSART_H_
#define DEVICECHARACTERUSART_H_

#include "portable/devices/character/include/OSDeviceCharacter.h"
#include "portable/misc/include/CircularByteBuffer.h"

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)

#ifndef OS_CONFIG_DEVICECHARACTERUSART_TXBUF_SIZE
#define OS_CONFIG_DEVICECHARACTERUSART_TXBUF_SIZE			(16)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART_RXBUF_SIZE
#define OS_CONFIG_DEVICECHARACTERUSART_RXBUF_SIZE			(16)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART_TXBUF_HIGH_WATER_MARK
#define OS_CONFIG_DEVICECHARACTERUSART_TXBUF_HIGH_WATER_MARK	(OS_CONFIG_DEVICECHARACTERUSART_TXBUF_SIZE*3/4)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART_TXBUF_LOW_WATER_MARK
#define OS_CONFIG_DEVICECHARACTERUSART_TXBUF_LOW_WATER_MARK		(OS_CONFIG_DEVICECHARACTERUSART_TXBUF_SIZE*1/4)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART_BAUD_RATE
#define OS_CONFIG_DEVICECHARACTERUSART_BAUD_RATE			(9600)
#endif

#ifndef OS_CONFIG_DEVICECHARACTERUSART_DOUBLE_SPEED
#define OS_CONFIG_DEVICECHARACTERUSART_DOUBLE_SPEED			(1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERUSART_TX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERUSART_TX_CONTEXT_SWITCH		(1)
#endif

#ifndef OS_CFGBOOL_DEVICECHARACTERUSART_RX_CONTEXT_SWITCH
#define OS_CFGBOOL_DEVICECHARACTERUSART_RX_CONTEXT_SWITCH		(1)
#endif

#endif /*OS_INCLUDE_DEVICECHARACTERMULTIUSART*/

class DeviceCharacterUsart : public OSDeviceCharacter
  {
public:
  DeviceCharacterUsart(void);

  void init(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned short txHWM, unsigned short txLWM, unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM);
  
  DeviceCharacterUsart(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned short txHWM, unsigned short txLWM, unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM);

  DeviceCharacterUsart(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned char *pRxBuf, unsigned short rxBufSize);

  //static DeviceCharacterUsart *ms_pThis;

  void interruptTxServiceRoutine(void);
  void interruptRxServiceRoutine(void);

protected:
  CircularByteBuffer m_txBuf;
  CircularByteBuffer m_rxBuf;

private:
  virtual int implOpen(void);
  virtual OSEvent_t implGetOpenEvent(void);

  virtual int implClose(void);

  virtual bool implIsConnected(void) const;

  virtual bool implCanWrite(void);
  virtual OSEvent_t implGetWriteEvent(void);
  virtual int implWriteByte(unsigned char b);

  virtual int implFlush(void);

  virtual bool implCanRead(void);
  virtual int implAvailableRead(void);
  virtual OSEvent_t implGetReadEvent(void);
  virtual int implReadByte(void);

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
  // actual port implementation routines
  void implPortInit(void);
  inline static void implInterruptTxEnable(void);
  inline static void implInterruptTxDisable(void);
  inline unsigned char implPortRead(void);
  inline void implPortWrite(unsigned char b);
  
public:
  static DeviceCharacterUsart *ms_pThis;

#else
  // forward port implementation routines to children class
  virtual void implPortInit(void) = 0;
  virtual void implInterruptTxEnable(void) = 0;
  virtual void implInterruptTxDisable(void) = 0;
  virtual unsigned char implPortRead(void) = 0;
  virtual void implPortWrite(unsigned char b) = 0;
#endif /*OS_INCLUDE_DEVICECHARACTERMULTIUSART*/
  };

#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)

#if defined(OS_CONFIG_ARCH_AVR)
#include "hal/arch/avr8/devices/character/include/DeviceCharacterUsart_Inlines.h"
#else
#error "Missing OS_CONFIG_ARCH_* definition"

#endif

#endif /*OS_INCLUDE_DEVICECHARACTERMULTIUSART*/

#endif /*DEVICECHARACTERUSART_H_*/
