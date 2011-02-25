/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICECAN_H_
#define OSDEVICECAN_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/CANPacket.h"
#include "portable/misc/include/CircularBlockBuffer.h"

class OSDeviceCAN
{
public:
  static const unsigned char MODE_ENABLED = 1;
  static const unsigned char MODE_STANDBY = 2;
  static const unsigned char MODE_LISTEN = 3;

  OSDeviceCAN(CANPacket *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, CANPacket *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  void
  init(unsigned char mode);

  bool
  isConnected(void);
  // blocking read.
  // return OK or negative(OS_ERROR_DISCONNECTED, OS_ERROR_TIMEOUT)
  int
  readPacket(CANPacket *p);

  int
  availableRead(void);

  // blocking write.
  // return OK or negative(OS_ERROR_DISCONNECTED, OS_ERROR_TIMEOUT)
  int
  writePacket(CANPacket *p);

  OSEvent_t
  getReadEvent(void);
  OSEvent_t
  getWriteEvent(void);

  bool
  isPowered(void);
  void
  setPowered(bool flag);

protected:

  virtual void
  implInit(unsigned char mode) = 0;
  //virtual bool implIsConnected(void) = 0;

  virtual bool
  implCanWrite(void) = 0;
  virtual int
  implWritePacket(CANPacket *p) = 0;

  virtual bool
  implCanRead(void) = 0;
  virtual int
  implAvailableRead(void) = 0;
  virtual int
  implReadPacket(CANPacket *p) = 0;

  virtual void
  implTransmit(void) = 0;

  unsigned char m_mode;
  static bool m_isConnected;
  static bool m_isPowered;
  static CircularBlockBuffer m_rxBuf;
  static CircularBlockBuffer m_txBuf;
};

inline OSEvent_t
OSDeviceCAN::getReadEvent(void)
{
  return (OSEvent_t) &m_rxBuf;
}

inline OSEvent_t
OSDeviceCAN::getWriteEvent(void)
{
  return (OSEvent_t) &m_txBuf;
}

inline bool
OSDeviceCAN::isConnected(void)
{
  return m_isConnected;
}

inline bool
OSDeviceCAN::isPowered(void)
{
  return m_isPowered;
}

inline void
OSDeviceCAN::setPowered(bool flag)
{
  m_isPowered = flag;
}

#endif /*OSDEVICECAN_H_*/
