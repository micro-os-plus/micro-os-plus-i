/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"

// ----- constructors --------------------------------------------------------

#if defined(OS_INCLUDE_OSDEVICECHARACTER)

OSDeviceCharacter::OSDeviceCharacter()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSDeviceCharacter"), this);

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
  m_pOpenTimer = &OSScheduler::timerTicks;
  m_pReadTimer = &OSScheduler::timerTicks;
  m_pWriteTimer = &OSScheduler::timerTicks;

  m_readTimeout = OSTimeout::OS_NEVER;
  m_writeTimeout = OSTimeout::OS_NEVER;
  m_openTimeout = OSTimeout::OS_NEVER;
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

  m_countToRead = 0;

  // initialise events with default values
  // will be set to the implementation values at open
  m_readEvent = OSEvent::OS_NONE;
  m_writeEvent = OSEvent::OS_NONE;
  m_openEvent = OSEvent::OS_NONE;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE)
  m_baudRate = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE) */

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
OSDeviceCharacter::isConnected(void)
{
  bool flag;
  flag = implIsConnected();
  return flag;
}

bool
OSDeviceCharacter::implIsConnected(void)
{
  // must implement it in children classes
  return true;
}

// OSReturn::OS_NOT_OPENED

int
OSDeviceCharacter::open(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
  OSSchedulerLock::enter();
    {
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::open("));
      OSDeviceDebug::putPtr(this);
      OSDeviceDebug::putChar(')');
      OSDeviceDebug::putNewLine();
    }
  OSSchedulerLock::exit();
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

  for (;;)
    {
      OSEvent_t event;
      event = getOpenEvent(); // default value

      bool isConnected;
      bool doWait;
      doWait = false;
      OSCriticalSection::enter();
        {
          isConnected = implIsConnected();
          if (!isConnected)
            {
              event = getOpenEvent();
              // Must be done in critical section
              doWait = OSScheduler::eventWaitPrepare(event);
            }
        }
      OSCriticalSection::exit();
      if (isConnected)
        break;

      // eventWait already prepared properly

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getOpenTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
          OSScheduler::eventWaitClear();
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pOpenTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
      OSSchedulerLock::enter();
        {
          OSDeviceDebug::putString_P(PSTR("eventWait("));
          OSDeviceDebug::putHex((unsigned int) event);
          OSDeviceDebug::putChar(')');
          OSDeviceDebug::putNewLine();
        }
      OSSchedulerLock::exit();
#endif
      if (doWait)
        OSScheduler::eventWaitPerform();

      OSEventWaitReturn_t ret;
      ret = OSScheduler::getEventWaitReturn();

      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_OPEN)
          OSDeviceDebug::putString_P(
              PSTR("OSDeviceCharacter::open() return timeout"));
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
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */
        }
      //OSDeviceDebug::putChar('!1');
    }

  // If everything is ok, mark that the device is opened
  m_isOpened = true;

  m_doCancelRead = false;

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
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_CLOSE)
  OSDeviceDebug::putString("OSDeviceCharacter::close()");
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_CLOSE) */

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

unsigned long
OSDeviceCharacter::getBaudRate(void)
{
#if defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE)
  return m_baudRate;
#else
  return 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE) */
}

bool
OSDeviceCharacter::isSending(void)
{
  return implIsSending();
}

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

  bool canWrite;
  for (;;)
    {
      bool doWait;
      doWait = false;
      OSEvent_t event;
      OSCriticalSection::enter();
        {
          canWrite = implCanWrite() || !implIsConnected();
          if (!canWrite)
            {
              event = getWriteEvent();
              // Must be done in critical section
              doWait = OSScheduler::eventWaitPrepare(event);
            }
        }
      OSCriticalSection::exit();

      if (canWrite)
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
          OSScheduler::eventWaitClear();
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pWriteTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() wait "));
      OSDeviceDebug::putHex((unsigned short)event);
      OSDeviceDebug::putNewLine();
#endif
      if (doWait)
        OSScheduler::eventWaitPerform();

      OSEventWaitReturn_t ret;
      ret = OSScheduler::getEventWaitReturn();
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
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */
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
OSDeviceCharacter::writeBytes(unsigned char* buf, int len)
{

  if (!m_isOpened)
    return OSReturn::OS_NOT_OPENED;

  bool canWrite;
  int i;
  i = 0;
  for (;;)
    {
      bool doWait;
      doWait = false;
      OSEvent_t event;
      OSCriticalSection::enter();
        {
          canWrite = implCanWrite() || !implIsConnected();
          if (!canWrite)
            {
              event = getWriteEvent();
              // Must be done in critical section
              doWait = OSScheduler::eventWaitPrepare(event);
            }
        }
      OSCriticalSection::exit();

      if (canWrite)
        {
          i += implWriteBytes(buf + i, len - i);
          if (i == len)
            break;
        }

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getWriteTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() would block"));
          OSDeviceDebug::putNewLine();
#endif
          OSScheduler::eventWaitClear();
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pWriteTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
      OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeByte() wait "));
      OSDeviceDebug::putHex((unsigned short)event);
      OSDeviceDebug::putNewLine();
#endif
      if (doWait)
        OSScheduler::eventWaitPerform();

      OSEventWaitReturn_t ret;
      ret = OSScheduler::getEventWaitReturn();
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
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */
        }
    }

  if (!isConnected())
    return OSReturn::OS_DISCONNECTED;

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::writeBytes() return"));
  OSDeviceDebug::putNewLine();
#endif
  return i;
}
// TODO implWriteBytes() as default implementation
int
OSDeviceCharacter::implWriteBytes(const unsigned char* buf, int len)
{
  int i;

  i = 0;
  while (implCanWrite() && (i < len))
    {
      implWriteByte(buf[i]);
      i++;
    }
  return i;
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

void
OSDeviceCharacter::cancelRead(void)
{
  m_doCancelRead = true;

  OSScheduler::eventNotify(getReadEvent(), OSEventWaitReturn::OS_CANCELLED);
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
    {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
      m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
      return OSReturn::OS_NOT_OPENED;
    }

  if(m_doCancelRead)
    {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
      m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
      return OSReturn::OS_CANCELLED;
    }

  bool canRead;
  for (;;)
    {
      bool doWait;
      doWait = false;
      OSEvent_t event;
      OSCriticalSection::enter();
        {
          // when not connected, do not wait, to return error
          canRead = implCanRead() || !implIsConnected();
          if (!canRead)
            {
              event = getReadEvent();
              // Must be done in critical section
              doWait = OSScheduler::eventWaitPrepare(event);
            }
        }
      OSCriticalSection::exit();

      if (canRead)
        break;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getReadTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
          OSScheduler::eventWaitClear();
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
          m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

      //OSDeviceDebug::putHex((unsigned short)event);
      //OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pReadTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

      if (doWait)
        OSScheduler::eventWaitPerform();

      OSEventWaitReturn_t ret;
      ret = OSScheduler::getEventWaitReturn();

      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() return timeout"));
          OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
          m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
          return OSReturn::OS_TIMEOUT;
        }
      else if (ret == OSEventWaitReturn::OS_CANCELLED)
        {
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() cancelled"));
          OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
          m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
          return OSReturn::OS_CANCELLED;
        }
      else
        {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
          if (timeout != OSTimeout::OS_NEVER)
            {
              m_pReadTimer->eventRemove(event);
            }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */
        }
    }

  if (!isConnected())
    {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
      m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
      return OSReturn::OS_DISCONNECTED;
    }
  int c;
  c = implReadByte();

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() returns "));
  OSDeviceDebug::putHex((unsigned short)c);
  OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

  return c;
}

// OSReturn::OS_NOT_OPENED
// OSReturn::OS_WOULD_BLOCK
// OSReturn::OS_TIMEOUT
// OSReturn::OS_DISCONNECTED

int
OSDeviceCharacter::readBytes(unsigned char* pBuf, int bufSize, int* count)
{
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte()"));
  OSDeviceDebug::putNewLine();
#endif

  if (!m_isOpened)
    {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
      m_pReadMatchArray = 0;
      *count = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
      return OSReturn::OS_NOT_OPENED;
    }

  int av;

  av = implAvailableRead();
  if (av >= bufSize)
    {
      *count = implReadBytes(pBuf, bufSize);
      return OSReturn::OS_OK;
    }

  m_countToRead = bufSize;
  *count = 0;

  bool canRead;
  for (;;)
    {
      bool doWait;
      doWait = false;
      OSEvent_t event;
      OSCriticalSection::enter();
        {
          canRead = implCanRead() || !implIsConnected();
          if (!canRead)
            {
              event = getReadEvent();
              // Must be done in critical section
              doWait = OSScheduler::eventWaitPrepare(event);
            }
        }
      OSCriticalSection::exit();

      if (canRead)
        break;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      OSTimerTicks_t timeout;
      timeout = getReadTimeout();

      if (timeout == OSTimeout::OS_IMMEDIATELY)
        {
          OSScheduler::eventWaitClear();
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
          m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */
          av = implAvailableRead();
          if (av > 0)
          *count = implReadBytes(pBuf, bufSize);
          m_countToRead = 0;
          return OSReturn::OS_WOULD_BLOCK;
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

      //OSDeviceDebug::putHex((unsigned short)event);
      //OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
      if (timeout != OSTimeout::OS_NEVER)
        {
          m_pReadTimer->eventNotify(timeout, event,
              OSEventWaitReturn::OS_TIMEOUT);
        }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */

      if (doWait)
        OSScheduler::eventWaitPerform();

      OSEventWaitReturn_t ret;
      ret = OSScheduler::getEventWaitReturn();

      //OSDeviceDebug::putHex((unsigned short)ret);
      //OSDeviceDebug::putNewLine();

      if (ret == OSEventWaitReturn::OS_TIMEOUT)
        {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
          OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() return timeout"));
          OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
          m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

          av = implAvailableRead();
          if (av > 0)
            *count = implReadBytes(pBuf, bufSize);
          m_countToRead = 0;

          return OSReturn::OS_TIMEOUT;
        }
      else
        {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)
          if (timeout != OSTimeout::OS_NEVER)
            {
              m_pReadTimer->eventRemove(event);
            }
#endif /* OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS */
        }
    }

  if (!isConnected())
    {
#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
      m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

      av = implAvailableRead();
      if (av > 0)
        *count = implReadBytes(pBuf, bufSize);
      m_countToRead = 0;

      return OSReturn::OS_DISCONNECTED;
    }
  av = implAvailableRead();
  if (av > 0)
    *count = implReadBytes(pBuf, bufSize);
  m_countToRead = 0;

#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
  OSDeviceDebug::putString_P(PSTR("OSDeviceCharacter::readByte() returns "));
  OSDeviceDebug::putHex((unsigned short)c);
  OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
  m_pReadMatchArray = 0;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

  return OSReturn::OS_OK;
}

int
OSDeviceCharacter::implReadBytes(unsigned char* pBuf, int size)
{
  int r;
  r = 0;
  while ((availableRead() > 0) && (r < size))
    {
      *pBuf++ = implReadByte();
      r++;
    }
  return r;
}

int
OSDeviceCharacter::availableRead(void)
{
  int r;
  r = implAvailableRead();
  return r;
}

#if defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH)
void
OSDeviceCharacter::setReadMatchArray(unsigned char* match)
  {
    m_pReadMatchArray = match;
  }
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_READMATCH) */

#if defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF)

// ----- streambuf virtuals --------------------------------------------------

int
OSDeviceCharacter::overflow(int c)
{

  int r;
  r = writeByte((unsigned char) c);
  if (r < 0)
    return std::traits::eof();
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

  //OSDeviceDebug::putChar((unsigned char)r);

  if (r < 0)
    return std::traits::eof();
  else
    return r;
}

int
OSDeviceCharacter::sync(void)
{
  return flush();
}

#endif /* OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF */

bool
OSDeviceCharacter::implIsSending(void)
{
  return false;
}

#endif /* OS_INCLUDE_OSDEVICECHARACTER */
