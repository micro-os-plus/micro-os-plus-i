/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICECHARACTER_H_
#define OSDEVICECHARACTER_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
#include "portable/stdlib/include/streambuf"
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)
// info to identify device type
typedef enum DeviceCharacterType_e
{
  OS_DEVICECHARACTER_USART, OS_DEVICECHARACTER_USB_CDC, OS_DEVICECHARACTER_NET
} DeviceCharacterType_t;
#endif

// TODO: methods for buffered read/write operations must be added.
class OSDeviceCharacter
#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
    : public std::streambuf
#endif

{
public:
  OSDeviceCharacter();

  virtual
  ~OSDeviceCharacter();

  // open device, blocking if necessary
  OSReturn_t
  open(void);

  // close device
  OSReturn_t
  close(void);

  // test if device is opened
  bool
  isOpened(void) const;

  // test if device is in connected state
  bool
  isConnected(void);

#if defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE)
  // set the baud rate
  void
  setBaudRate(unsigned long baud);
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE) */

  // get the current baud rate
  virtual unsigned long
  getBaudRate(void);

  bool
  isSending(void);

  // blocking byte read.
  // return byte or negative (OSReturn::OS_DISCONNECTED, OSReturn::OS_TIMEOUT)
  OSReturn_t
  readByte(void);

  // blocking multiple byte read
  // return number of the bytes read or error
  // if not enough bytes, return those available
  OSReturn_t
  readBytes(unsigned char* pBuf, int bufSize, int* count);

  // Check if there is available data to be read
  // and if it is return the size of the data.
  OSReturn_t
  availableRead(void);

  // blocking byte write. does not flush, unless buffer full.
  // return byte or negative (OSReturn::OS_DISCONNECTED, OSReturn::OS_TIMEOUT)
  int
  writeByte(unsigned char b);

  // blocking multiple byte write. does not flush, unless buffer full.
  // return number of  bytes or negative (OSReturn::OS_DISCONNECTED, OSReturn::OS_TIMEOUT)
  int
  writeBytes(unsigned char* pBuf, int len);

  // flush existing buffer
  OSReturn_t
  flush(void);

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)
  // get device type
  DeviceCharacterType_t
  getDeviceType(void) const;
#endif

  // set/get the thread event to be used by read
  void
  setReadEvent(OSEvent_t event);
  OSEvent_t
  getReadEvent(void) const;

  // set/get the thread event to be used by write
  void
  setWriteEvent(OSEvent_t event);
  OSEvent_t
  getWriteEvent(void) const;

  // set/get the thread event to be used by open
  void
  setOpenEvent(OSEvent_t event);
  OSEvent_t
  getOpenEvent(void) const;

  void
  cancelRead(void);

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  // set the match array
  void
  setReadMatchArray(unsigned char* match);
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

#if !defined(OS_EXCLUDE_MULTITASKING)
  // OSTimeout::OS_NEVER = no timeout (default)
  // OSTimeout::IMMEDIATELLY = return WOULD_BLOCK if necessary

  // set/get the open timeout()
  void
  setOpenTimeout(OSTimerTicks_t t);
  OSTimerTicks_t
  getOpenTimeout(void) const;

  // set/get the read timeout()
  void
  setReadTimeout(OSTimerTicks_t t);
  OSTimerTicks_t
  getReadTimeout(void) const;

  // set/get the write timeout()
  void
  setWriteTimeout(OSTimerTicks_t t);
  OSTimerTicks_t
  getWriteTimeout(void) const;

  // Set timer to be used by the open function.
  // If the operation doesn't succeed after the open-timeout
  // (set with setOpenTimeout), this timer is used to
  // wake-up the thread and return with an error code.
  void
  setOpenTimer(OSTimer *pTimer);
  OSTimer *
  getOpenTimer(void) const;

  // Set timer to be used by the read functions.
  // If the operation doesn't succeed after the read-timeout
  // (set with setReadTimeout), this timer is used to
  // wake-up the thread and return with an error code.
  void
  setReadTimer(OSTimer *pTimer);
  OSTimer *
  getReadTimer(void) const;

  // Set timer to be used by the write functions.
  // If the operation doesn't succeed after the write-timeout
  // (set with setWriteTimeout), this timer is used to
  // wake-up the thread and return with an error code.
  void
  setWriteTimer(OSTimer *pTimer);
  OSTimer *
  getWriteTimer(void) const;

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

protected:
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)
  // info to identify device class
  DeviceCharacterType_t m_type;
#endif

  bool m_isOpened;

  bool m_doCancelRead;

  unsigned long volatile m_baudRate;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  unsigned char* m_pReadMatchArray;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

  //int m_countToRead;

protected:

  // Implementation for different methods.

  // The next three methods are called at open(). If not redefined
  // by implementation, they return unique addresses anyway.
  virtual OSEvent_t
  implGetOpenEvent(void);
  virtual OSEvent_t
  implGetReadEvent(void);
  virtual OSEvent_t
  implGetWriteEvent(void);

  virtual OSReturn_t
  implOpen(void) = 0;
  virtual OSReturn_t
  implClose(void) = 0;
  virtual bool
  implIsConnected(void);

  virtual bool
  implCanRead(void) = 0;
  virtual int
  implAvailableRead(void) = 0;
  virtual int
  implReadByte(void) = 0;
  virtual int
  implReadBytes(unsigned char* pBuf, int size);

  virtual bool
  implCanWrite(void) = 0;
  virtual int
  implWriteByte(unsigned char b) = 0;
  virtual int
  implWriteBytes(const unsigned char* buf, int len);

  virtual bool
  implIsSending(void);

  virtual int
  implFlush(void) = 0;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)

  OSTimer *m_pOpenTimer;
  OSTimer *m_pReadTimer;
  OSTimer *m_pWriteTimer;

  OSTimerTicks_t m_openTimeout;
  OSTimerTicks_t m_readTimeout;
  OSTimerTicks_t m_writeTimeout;

#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

  // Events for open/read/write methods. By default they contain
  // exactly their addresses.
  OSEvent_t m_openEvent;
  OSEvent_t m_readEvent;
  OSEvent_t m_writeEvent;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
  // streambuf definitions
  virtual int
  overflow(int c = std::traits::eof());
  virtual int
  showmanyc();
  virtual int
  underflow();
  virtual int
  sync();
#endif

};

// ----- inline implementations ----------------------------------------------

inline void
OSDeviceCharacter::setOpenEvent(OSEvent_t event)
{
  m_openEvent = event;
}

inline OSEvent_t
OSDeviceCharacter::getOpenEvent(void) const
{
  return m_openEvent;
}

#if defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE)
inline void
OSDeviceCharacter::setBaudRate(unsigned long baud)
  {
    m_baudRate = baud;
  }
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE) */

inline void
OSDeviceCharacter::setReadEvent(OSEvent_t event)
{
  m_readEvent = event;
}

inline OSEvent_t
OSDeviceCharacter::getReadEvent(void) const
{
  return m_readEvent;
}

inline void
OSDeviceCharacter::setWriteEvent(OSEvent_t event)
{
  m_writeEvent = event;
}

inline OSEvent_t
OSDeviceCharacter::getWriteEvent(void) const
{
  return m_writeEvent;
}

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)

inline void
OSDeviceCharacter::setOpenTimeout(OSTimerTicks_t t)
  {
    m_openTimeout = t;
  }

inline OSTimerTicks_t
OSDeviceCharacter::getOpenTimeout(void) const
  {
    return m_openTimeout;
  }

inline void
OSDeviceCharacter::setReadTimeout(OSTimerTicks_t t)
  {
    m_readTimeout = t;
  }

inline OSTimerTicks_t
OSDeviceCharacter::getReadTimeout(void) const
  {
    return m_readTimeout;
  }

inline void
OSDeviceCharacter::setWriteTimeout(OSTimerTicks_t t)
  {
    m_writeTimeout = t;
  }

inline OSTimerTicks_t
OSDeviceCharacter::getWriteTimeout(void) const
  {
    return m_writeTimeout;
  }

inline void
OSDeviceCharacter::setOpenTimer(OSTimer *pTimer)
  {
    m_pOpenTimer = pTimer;
  }

inline OSTimer *
OSDeviceCharacter::getOpenTimer(void) const
  {
    return m_pOpenTimer;
  }

inline void
OSDeviceCharacter::setReadTimer(OSTimer *pTimer)
  {
    m_pReadTimer = pTimer;
  }

inline OSTimer *
OSDeviceCharacter::getReadTimer(void) const
  {
    return m_pReadTimer;
  }

inline void
OSDeviceCharacter::setWriteTimer(OSTimer *pTimer)
  {
    m_pWriteTimer = pTimer;
  }

inline OSTimer *
OSDeviceCharacter::getWriteTimer(void) const
  {
    return m_pWriteTimer;
  }

#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)
inline DeviceCharacterType_t
OSDeviceCharacter::getDeviceType(void) const
{
  return m_type;
}
#endif

#endif /* OSDEVICECHARACTER_H_ */
