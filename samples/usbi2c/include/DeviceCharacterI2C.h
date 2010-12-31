/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERI2C_H_
#define DEVICECHARACTERI2C_H_

#include "portable/kernel/include/uOS.h"

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE                 (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_RXBUF_SIZE
#define OS_CFGINT_DEVICECHARACTERI2C_RXBUF_SIZE                 (16)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_HIGH_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_HIGH_WATER_MARK      (OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE*3/4)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_TXBUF_LOW_WATER_MARK
#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_LOW_WATER_MARK       (OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE*1/4)
#endif

#ifndef OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH
#define OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH             (true)
#endif

class DeviceCharacterI2C : public OSDeviceCharacter
  {
public:
  DeviceCharacterI2C(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned short txHWM, unsigned short txLWM, unsigned char *pRxBuf,
      unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM);

  DeviceCharacterI2C(unsigned char *pTxBuf, unsigned short txBufSize,
      unsigned char *pRxBuf, unsigned short rxBufSize);

  void interruptServiceRoutine(void);

  virtual ~DeviceCharacterI2C();

  static DeviceCharacterI2C *pDevice;

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

  //void init(void);
  inline static void interruptTxEnable(void);
  inline static void interruptTxDisable(void);
  inline unsigned char portRead(void);
  inline void portWrite(unsigned char b);

  CircularByteBuffer m_txBuf;
  CircularByteBuffer m_rxBuf;
  };

#endif /*DEVICECHARACTERI2C_H_*/
