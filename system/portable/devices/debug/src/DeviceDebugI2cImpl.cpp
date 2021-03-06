/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(DEBUG) && defined(OS_CONFIG_DEBUG_DEVICE_I2C)

#include "portable/devices/debug/include/DeviceDebugI2cImpl.h"
#include "portable/kernel/include/OSCPUImpl.h"

#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

#if !defined(OS_CFGINT_DEVICEDEBUGI2CIMPL_BUFFER_SIZE)
#define OS_CFGINT_DEVICEDEBUGI2CIMPL_BUFFER_SIZE 80
#endif

char DeviceDebugI2cImpl::ms_buffer[OS_CFGINT_DEVICEDEBUGI2CIMPL_BUFFER_SIZE];
DeviceDebugI2cImpl::CircularBuffer DeviceDebugI2cImpl::ms_circularBuffer;
DeviceDebugI2cImpl::Lock DeviceDebugI2cImpl::ms_lock;

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

void
DeviceDebugI2cImpl::implEarlyInit(void)
{
#if true
    {
      unsigned int _i, _j;
      for (_i = 0; _i < OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT; ++_i)
        for (_j = 0; _j < OS_CFGINT_BUSYWAIT_CALIBRATION; ++_j)
          implWDReset();
    }
#endif

#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

  ms_lock.initialise();
  ms_circularBuffer.initialise(ms_buffer, sizeof(ms_buffer));

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

  masterInit();

  masterSclInit();
  masterSdaInit();
}

#if defined(OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE)
void
DeviceDebugI2cImpl::implPutByte(unsigned char c)
  {
    for (;;) // loop until we succeed to transmit

      {
        masterStart();
        masterPutAddress(OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR, I2C_WRITE);
        if (!masterGetAck())
          {
            masterCleanup();
            goto retry;
          }

        masterPutByte(c);
        if (!masterGetAck())
          {
            masterCleanup();
            goto retry;
          }

        masterStop();
        break;

        retry: masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT * 10);
      }
  }
#endif

int
DeviceDebugI2cImpl::implPutBytes(const char* s, unsigned int n)
{
#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

  // First append all chars to display at the end of the internal buffer
#if false

  unsigned int i;
  register OSStack_t mask;
  mask = criticalEnter();
    {
      for (i = 0; i < n; ++i)
      ms_circularBuffer.put(*s++);
    }
  criticalExit(mask);

#else

  n = ms_circularBuffer.putBytes(s, n);

#endif

  if (OSCPUImpl::isInInterruptMode() || OSCPUImpl::isInCriticalSection())
    {
      return n;
    }

  implFlush();

  return n;

#else

  unsigned int i;
#if defined(OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE)
  for (i = 0; i < n; ++i)
  implPutByte(s[i]);
#else
  for (i = 0; i < n;)
    { // loop until we succeed to transmit
      masterStart();
      masterPutAddress(OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR, I2C_WRITE);
      if (!masterGetAck())
        {
          masterCleanup();
          goto retry;
        }

      for (; i < n; ++i)
        {
          masterPutByte(s[i]);
          if (!masterGetAck())
            {
              masterCleanup();
              goto retry;
            }
        }
      masterStop();

#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_STOP_COUNT ((OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT+1) * 10)

      // To avoid NAK for fast sequences
      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_STOP_COUNT);

      break;

      retry: masterDelay((OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT+1) * 10);
    }
#endif

  return n;

#endif /* !defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */
}

void
DeviceDebugI2cImpl::implFlush(void)
{
#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

  if (ms_lock.aquire())
    {
      unsigned char ch;
      ch = '?';
      bool shouldRetry;
      shouldRetry = false;

      for (; shouldRetry || !ms_circularBuffer.isEmpty();)
        {
          // loop until we succeed to transmit
          masterStart();
          masterPutAddress(OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR, I2C_WRITE);

          // Check ACK for address
          if (masterGetAck())
            {
              // Address ACK ok, continue to send data
              for (; shouldRetry || !ms_circularBuffer.isEmpty();)
                {
                  if (!shouldRetry)
                    {
                      ch = ms_circularBuffer.get();
                    }
                  masterPutByte(ch);
                  if (!masterGetAck())
                    {
                      shouldRetry = true;
                      goto retry;
                    }
                  else
                    {
                      shouldRetry = false;
                    }
                }
              masterStop();
              break;
            }
          else
            {
              retry: //
              masterCleanup();
              masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT * 10);
            }
        }
      ms_lock.release();
    }

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */
}

// ----- private methods ------------------------------------------------------

#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

void
DeviceDebugI2cImpl::CircularBuffer::initialise(char* pc, unsigned short size)
{
  CircularByteBuffer::initialise((unsigned char*) pc, size);
}

#if true

bool
DeviceDebugI2cImpl::CircularBuffer::isEmpty()
{
  bool flag;
  register OSStack_t mask;
  mask = criticalEnter();
    {
      flag = CircularByteBuffer::isEmpty();
    }
  criticalExit(mask);

  return flag;
}

unsigned short
DeviceDebugI2cImpl::CircularBuffer::putBytes(const char* pc,
    unsigned short size)
{
  unsigned short nBytes;
  register OSStack_t mask;
  mask = criticalEnter();
    {
      if (CircularByteBuffer::length() + size > CircularByteBuffer::size())
        {
          CircularByteBuffer::put('?');
          nBytes = 0;
        }
      else
        {
          nBytes = CircularByteBuffer::putBytes((unsigned char*) pc, size);
        }
    }
  criticalExit(mask);

  return nBytes;
}

unsigned char
DeviceDebugI2cImpl::CircularBuffer::get(void)
{
  unsigned char ch;

  register OSStack_t mask;
  mask = criticalEnter();
    {
      ch = CircularByteBuffer::get();
    }
  criticalExit(mask);

  return ch;
}

#endif

void
DeviceDebugI2cImpl::Lock::initialise(void)
{
  m_isAquired = false;
}

bool
DeviceDebugI2cImpl::Lock::aquire(void)
{
  bool ret;

  register OSStack_t mask;
  mask = criticalEnter();
    {
      ret = m_isAquired;

      if (!ret)
        {
          m_isAquired = true;
        }
    }
  criticalExit(mask);

  return !ret;
}

void
DeviceDebugI2cImpl::Lock::release(void)
{
  register OSStack_t mask;
  mask = criticalEnter();
    {
      m_isAquired = false;
    }
  criticalExit(mask);
}

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

#if false
unsigned char
DeviceDebugI2cImpl::masterGetByte(void)
  {
    int i;
    unsigned char b = 0;

    masterSdaHigh();
    for (i = 8; i != 0; --i)
      {
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        masterSclHigh();
        masterSclSynchronize();
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        b <<= 1;
        if (!masterSdaIsLow())
          {
            b |= 1;
          }
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        masterSclLow();
        masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
        ;
      }
    return b;
  }

// Extra half delay at end
void
DeviceDebugI2cImpl::masterPutAck(bool b)
  {
    if (b)
    masterSdaHigh();
    else
    masterSdaLow();

    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    masterSclHigh();
    masterSclSynchronize();
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
    masterSclLow();
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  }
#endif

// Extra half delay at end
bool
DeviceDebugI2cImpl::masterGetAck(void)
{
  bool r;

  masterSdaHigh();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclHigh();
  masterSclSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  r = masterSdaIsLow();

  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);

  // Extra delay
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT * 2);

  return r;
}

void
DeviceDebugI2cImpl::masterCleanup(void)
{
  masterSclHigh();
  masterSclSynchronize();
  masterSdaHigh();
  masterSdaSynchronize();
}

// No arbitration implemented yet

void
DeviceDebugI2cImpl::masterStart(void)
{
  masterSdaHigh();
  masterSdaSynchronize();
  masterSclHigh();
  masterSclSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSdaLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
  masterSclLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
}

void
DeviceDebugI2cImpl::masterPutAddress(unsigned char addr, unsigned char mode)
{
  masterPutByte((addr << 1) | (mode & 1));
}

// Also wait for SDA to go high to confirm stop condition
void
DeviceDebugI2cImpl::masterStop(void)
{
  masterSdaLow();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
  masterSclHigh();
  masterSclSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
  masterSdaHigh();
  masterSdaSynchronize();
  masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
}

void
DeviceDebugI2cImpl::masterDelay(int n)
{
  implWDReset();
  for (++n; --n != 0;)
    {
      implWDReset();
    }
}

void
DeviceDebugI2cImpl::masterPutByte(unsigned char b)
{
  for (int i = 8; i != 0; --i, b <<= 1)
    {
      if ((b & 0x80) != 0)
        masterSdaHigh();
      else
        masterSdaLow();

      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
      masterSclHigh();
      masterSclSynchronize();
      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT);
      masterSclLow();
      masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT / 2);
    }
  masterSdaHigh();
}

#endif /* defined(DEBUG) && defined(OS_CONFIG_DEBUG_DEVICE_I2C) */

