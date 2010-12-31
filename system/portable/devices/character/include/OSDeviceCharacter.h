/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSDEVICECHARACTER_H_
#define OSDEVICECHARACTER_H_

#include "portable/kernel/include/OS.h"
#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
#include "portable/stdlib/include/streambuf"
#endif

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
// info to identify device type
typedef enum
  { OS_DEVICECHARACTER_USART, OS_DEVICECHARACTER_USB_CDC}
deviceCharacterType_t;
#endif

class OSDeviceCharacter
#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
: public streambuf
#endif

  {
public:
  OSDeviceCharacter();

  // open device, blocking if necessary
  OSReturn_t open(void);

  // close device
  OSReturn_t close(void);

  // test if device in opened
  bool isOpened(void) const;

  // test if device in connected state
  bool isConnected(void) const;

  // blocking read.
  // return byte or negative (OSReturn::OS_DISCONNECTED, OSReturn::OS_TIMEOUT)
  OSReturn_t readByte(void);

  OSReturn_t availableRead(void);

  // blocking write. does not flush, unless buffer full.
  // return byte or negative (OSReturn::OS_DISCONNECTED, OSReturn::OS_TIMEOUT)
  int writeByte(unsigned char b);

  // flush existing buffer
  OSReturn_t flush(void);

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  // get device type
  deviceCharacterType_t getDeviceType(void);
#endif

  // set/get the task event to be used by read
  void setReadEvent(OSEvent_t event);
  OSEvent_t getReadEvent(void);

  // set/get the task event to be used by write
  void setWriteEvent(OSEvent_t event);
  OSEvent_t getWriteEvent(void);

  // set/get the task event to be used by open
  void setOpenEvent(OSEvent_t event);
  OSEvent_t getOpenEvent(void);

#if 1
  // OSTimeout::OS_NEVER = no timeout (default)
  // OSTimeout::IMMEDIATELLY = return WOULD_BLOCK if necessary

  // set/get the open timeout()
  void setOpenTimeout(OSTimerTicks_t t);
  OSTimerTicks_t getOpenTimeout(void);

  // set/get the read timeout()
  void setReadTimeout(OSTimerTicks_t t);
  OSTimerTicks_t getReadTimeout(void);

  // set/get the write timeout()
  void setWriteTimeout(OSTimerTicks_t t);
  OSTimerTicks_t getWriteTimeout(void);

  void setOpenTimer(OSTimer *pTimer);
  OSTimer *getOpenTimer(void);

  void setReadTimer(OSTimer *pTimer);
  OSTimer *getReadTimer(void);

  void setWriteTimer(OSTimer *pTimer);
  OSTimer *getWriteTimer(void);

#endif

protected:
#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  // info to identify device class
  deviceCharacterType_t m_type;
#endif

  bool m_isOpened;

private:
  virtual OSEvent_t implGetOpenEvent(void);
  virtual OSEvent_t implGetReadEvent(void);
  virtual OSEvent_t implGetWriteEvent(void);

  virtual OSReturn_t implOpen(void) = 0;
  virtual OSReturn_t implClose(void) = 0;
  virtual bool implIsConnected(void) const;

  virtual bool implCanRead(void) = 0;
  virtual int implAvailableRead(void) = 0;
  virtual int implReadByte(void) = 0;

  virtual bool implCanWrite(void) = 0;
  virtual int implWriteByte(unsigned char b) = 0;

  virtual int implFlush(void) = 0;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)

  OSTimer *m_pOpenTimer;
  OSTimer *m_pReadTimer;
  OSTimer *m_pWriteTimer;

  OSTimerTicks_t m_openTimeout;
  OSTimerTicks_t m_readTimeout;
  OSTimerTicks_t m_writeTimeout;

#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

  OSEvent_t m_openEvent;
  OSEvent_t m_readEvent;
  OSEvent_t m_writeEvent;


#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)
  // streambuf definitions
  virtual int overflow(int c = traits::eof());
  virtual int showmanyc();
  virtual int underflow();
  virtual int sync();
#endif

  };

// ----- inline implementations ----------------------------------------------

inline void OSDeviceCharacter::setOpenEvent(OSEvent_t event)
  {
    m_openEvent = event;
  }

inline OSEvent_t OSDeviceCharacter::getOpenEvent(void)
  {
    return m_openEvent;
  }

inline void OSDeviceCharacter::setReadEvent(OSEvent_t event)
  {
    m_readEvent = event;
  }

inline OSEvent_t OSDeviceCharacter::getReadEvent(void)
  {
    return m_readEvent;
  }

inline void OSDeviceCharacter::setWriteEvent(OSEvent_t event)
  {
    m_writeEvent = event;
  }

inline OSEvent_t OSDeviceCharacter::getWriteEvent(void)
  {
    return m_writeEvent;
  }

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)

inline void OSDeviceCharacter::setOpenTimeout(OSTimerTicks_t t)
  {
    m_openTimeout = t;
  }

inline OSTimerTicks_t OSDeviceCharacter::getOpenTimeout(void)
  {
    return m_openTimeout;
  }

inline void OSDeviceCharacter::setReadTimeout(OSTimerTicks_t t)
  {
    m_readTimeout = t;
  }

inline OSTimerTicks_t OSDeviceCharacter::getReadTimeout(void)
  {
    return m_readTimeout;
  }

inline void OSDeviceCharacter::setWriteTimeout(OSTimerTicks_t t)
  {
    m_writeTimeout = t;
  }

inline OSTimerTicks_t OSDeviceCharacter::getWriteTimeout(void)
  {
    return m_writeTimeout;
  }

inline void OSDeviceCharacter::setOpenTimer(OSTimer *pTimer)
  {
    m_pOpenTimer = pTimer;
  }

inline OSTimer *OSDeviceCharacter::getOpenTimer(void)
  {
    return m_pOpenTimer;
  }

inline void OSDeviceCharacter::setReadTimer(OSTimer *pTimer)
  {
    m_pReadTimer = pTimer;
  }

inline OSTimer *OSDeviceCharacter::getReadTimer(void)
  {
    return m_pReadTimer;
  }

inline void OSDeviceCharacter::setWriteTimer(OSTimer *pTimer)
  {
    m_pWriteTimer = pTimer;
  }

inline OSTimer *OSDeviceCharacter::getWriteTimer(void)
  {
    return m_pWriteTimer;
  }

#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
inline deviceCharacterType_t
OSDeviceCharacter::getDeviceType( void )
  {
    return m_type;
  }
#endif

#endif /*OSDEVICECHARACTER_H_*/
