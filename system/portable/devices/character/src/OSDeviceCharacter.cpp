/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"

// ----- constructors --------------------------------------------------------

#if defined(OS_INCLUDE_OSDEVICECHARACTER)

OSDeviceCharacter::OSDeviceCharacter()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter()="));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
  m_pOpenTimer = &OSScheduler::timerTicks;
  m_pReadTimer = &OSScheduler::timerTicks;
  m_pWriteTimer = &OSScheduler::timerTicks;

  m_readTimeout = OSTimeout::OS_NEVER;
  m_writeTimeout = OSTimeout::OS_NEVER;
  m_openTimeout = OSTimeout::OS_NEVER;
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

  // init events with default values
  // will be set to the implentation values at open
  m_readEvent = OSEvent::OS_NONE;
  m_writeEvent = OSEvent::OS_NONE;
  m_openEvent = OSEvent::OS_NONE;

  m_isOpened = false;
}

// Defaults for event codes; return their addresses,
// guaranteed to be unique.
OSEvent_t
OSDeviceCharacter::implGetOpenEvent(void)
{
  return (OSEvent_t) &m_openEvent;
}

OSEvent_t
OSDeviceCharacter::implGetWriteEvent(void)
{
  return (OSEvent_t) &m_writeEvent;
}

OSEvent_t
OSDeviceCharacter::implGetReadEvent(void)
{
  return (OSEvent_t) &m_openEvent;
}

bool
OSDeviceCharacter::isConnected(void) const
{
  bool flag;
  flag = implIsConnected();
  return flag;
}

bool
OSDeviceCharacter::implIsConnected(void) const
{
  // implement it in children classes
  return true;
}

// OSReturn::OS_NOT_OPENED

int
OSDeviceCharacter::open(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::open("));
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putChar(')');
  OSDeviceDebug::putNewLine();
#endif

  int r;
  if (m_isOpened)
    return OSReturn::OS_ALREADY_OPENED;

  m_openEvent = implGetOpenEvent();
  m_readEvent = implGetReadEvent();
  m_writeEvent = implGetWriteEvent();

  r = implOpen();
  if (r < 0)
    return r;

  m_isOpened = true;

  for (;;)
    {
      bool flag;
      flag = implIsConnected();
      if (flag)
        break;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getOpenTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
      return OSReturn::OS_WOULD_BLOCK;
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

      OSEvent_t event;
      event = getOpenEvent();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pOpenTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
      OSDeviceDebug::putString_P(PSTR("eventWait("));
      OSDeviceDebug::putHex((unsigned short)event);
      OSDeviceDebug::putChar(')');
      OSDeviceDebug::putNewLine();
#endif
      OSEventWaitReturn_t ret;
      ret = OSScheduler::eventWait(event);
      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::open() return timeout"));
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_TIMEOUT;
        }
      else
        {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
          if (timeout != OSTimeout::OS_NEVER)
            {
              m_pOpenTimer->eventRemove(event);
            }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/
        }
      //OSDeviceDebug::putChar( '!' );
    }

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::open() return OK"));
  OSDeviceDebug::putNewLine();
#endif
  return OSReturn::OS_OK;
}

// OSReturn::OS_NOT_OPENED

int
OSDeviceCharacter::close(void)
{
  //OSDeviceDebug::putString( "OSDeviceCharacter::close()" );
  //OSDeviceDebug::putNewLine();

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  int r;
  r = implClose();

  m_isOpened = false;
  return r;
}

bool
OSDeviceCharacter::isOpened(void) const
{
  return m_isOpened;
}

// OSReturn::OS_NOT_OPENED
// OSReturn::OS_WOULD_BLOCK
// OSReturn::OS_TIMEOUT
// OSReturn::OS_DISCONNECTED

int
OSDeviceCharacter::writeByte(unsigned char b)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte("));
  OSDeviceDebug::putHex(b);
  OSDeviceDebug::putChar(')');
  OSDeviceDebug::putNewLine();
#endif

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  bool flag;
  for (;;)
    {
      flag = implCanWrite() || !implIsConnected();
      if (flag)
        break;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getWriteTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() would block"));
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

      OSEvent_t event;
      event = getWriteEvent();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pWriteTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() wait "));
      OSDeviceDebug::putHex((unsigned short)event);
      OSDeviceDebug::putNewLine();
#endif
      OSEventWaitReturn_t ret;
      ret = OSScheduler::eventWait(event);
      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() return timeout"));
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_TIMEOUT;
        }
      else
        {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
          if (timeout != OSTimeout::OS_NEVER)
            {
              m_pWriteTimer->eventRemove(event);
            }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/
        }
    }

  if (!isConnected())
    return OSReturn::OS_DISCONNECTED;

  implWriteByte(b);

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() return"));
  OSDeviceDebug::putNewLine();
#endif
  return b;
}

int
OSDeviceCharacter::flush(void)
{
  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  int r;
  r = implFlush();
  return r;
}

// OSReturn::OS_NOT_OPENED
// OSReturn::OS_WOULD_BLOCK
// OSReturn::OS_TIMEOUT
// OSReturn::OS_DISCONNECTED

int
OSDeviceCharacter::readByte(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte()"));
  OSDeviceDebug::putNewLine();
#endif

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  bool flag;
  for (;;)
    {
      flag = implCanRead() || !implIsConnected();
      if (flag)
        break;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getReadTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

      OSEvent_t event;
      event = getReadEvent();
      //OSDeviceDebug::putHex((unsigned short)event);
      //OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pReadTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/

      OSEventWaitReturn_t ret;
      ret = OSScheduler::eventWait(event);
      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() return timeout"));
          OSDeviceDebug::putNewLine();
#endif
          return OSReturn::OS_TIMEOUT;
        }
      else
        {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
          if (timeout != OSTimeout::OS_NEVER)
            {
              m_pReadTimer->eventRemove(event);
            }
#endif /*OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS*/
        }
    }

  if (!isConnected())
    return OSReturn::OS_DISCONNECTED;

  int c;
  c = implReadByte();

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() returns "));
  OSDeviceDebug::putHex((unsigned short)c);
  OSDeviceDebug::putNewLine();
#endif
  return c;
}

int
OSDeviceCharacter::availableRead(void)
{
  int r;
  r = implAvailableRead();
  return r;
}

#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)

// ----- streambuf virtuals --------------------------------------------------

int
OSDeviceCharacter::overflow(int c)
{

  int r;
  r = writeByte((unsigned char) c);
  if (r < 0)
    return traits::eof();
  else
    return r;
}

int
OSDeviceCharacter::showmanyc(void)
{
  return availableRead();
}

int
OSDeviceCharacter::underflow(void)
{
  int r;
  r = readByte();

  //OSDeviceDebug::putChar(( unsigned char )r );

  if (r < 0)
    return traits::eof();
  else
    return r;
}

int
OSDeviceCharacter::sync(void)
{
  return flush();
}

#endif /*OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF*/

#endif /*OS_INCLUDE_OSDEVICECHARACTER*/
