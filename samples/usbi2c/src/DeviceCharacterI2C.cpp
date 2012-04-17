/*
 *      Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "DeviceCharacterI2C.h"
#include <ctype.h>

// set custom high/low water marks
DeviceCharacterI2C::DeviceCharacterI2C(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char *pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM) :
    OSDeviceCharacterBuffered(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf,
        rxBufSize, rxHWM, rxLWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterI2C"), this);

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  m_type = DEVICECHARACTER_USART;
#endif

  m_timeoutCounterTicks = 0;

  pDevice = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterI2C::DeviceCharacterI2C(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned char *pRxBuf, unsigned short rxBufSize) :
    OSDeviceCharacterBuffered(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterI2C"), this);

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  m_type = DEVICECHARACTER_I2C;
#endif

  m_timeoutCounterTicks = 0;

  pDevice = this;
}

DeviceCharacterI2C::~DeviceCharacterI2C()
{
  OSDeviceDebug::putDestructor_P(PSTR("DeviceCharacterI2C"), this);
}

// --------

// port implementation routines
int
DeviceCharacterI2C::implPortInit(void)
{
  //OSDeviceDebug::putString_P(PSTR("DeviceCharacterI2C::implPortInit"));

  m_timeoutCounterTicks = 0;

  TWBR = 2; // 400000 at 8MHz
  TWSR = 0; // prescaller = 1

  TWAR = (0x28 << 1); // slave address
  TWAMR = 0; // no mask

  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWIE);

  return OSReturn::OS_OK;
}
// called from implOpen()

int
DeviceCharacterI2C::implPortDisable(void)
{
  //OSDeviceDebug::putString_P(PSTR("DeviceCharacterI2C::implPortDisable"));

  // Disable timeout checking
  m_timeoutCounterTicks = 0;

  TWCR = 0; // disable interface

  // leave bits as input
  DDRD &= ~(_BV(0) | _BV(1));

  return OSReturn::OS_OK;
}
// called from implClose()

unsigned char
DeviceCharacterI2C::implPortRead(void)
{
  return TWDR;
}
// called from interruptRxServiceRoutine()

void
DeviceCharacterI2C::implPortWrite(unsigned char b)
{
  TWDR = b;
}

void
DeviceCharacterI2C::implInterruptTxEnable(void)
{
  ; // TODO: implement
}
// called from implWriteByte() in critical section
// called from implFlush() in critical section

void
DeviceCharacterI2C::implInterruptTxDisable(void)
{
  ; // TODO: implement
}

void
DeviceCharacterI2C::implResumeReception(void)
{
  OSDeviceDebug::putChar('*');

  TWCR |= _BV(TWEA); // ACK
}

// ----------------------------------------------------------------------------

void
DeviceCharacterI2C::interruptServiceRoutine(void)
{
  unsigned char ch;

#if false
  OSDeviceDebug::putChar('#');
#endif

  OSEvent_t readEvent;
  readEvent = getReadEvent();

  unsigned char status;
  // Get rid of the prescaler bits
  status = TWSR & (~0x03);

  // See Table 20.5 Status Codes for Slave Receiver Mode
  switch (status)
    {
  // Warning: the SLA is not protected by timeout

  case 0x60:
    // Own SLA+W has been received; ACK has been returned

    // Start timeout counting
    m_timeoutCounterTicks = 1;

    if (!m_rxBuf.isFull())
      {
        TWCR |= _BV(TWEA); // ACK
        m_isReceiving = true;
      }
    else
      {
        TWCR &= ~_BV(TWEA); // NOT ACK
        m_isReceiving = false;
        os.sched.eventNotify(readEvent);
        OSDeviceDebug::putChar('A');
      }
    break;

  case 0x80:
    // Previously addressed with own SLA+W; data has been received;
    // ACK has been returned

    // Start timeout counting
    m_timeoutCounterTicks = 1;

    ch = TWDR;

#if false && defined(DEBUG)
    OSDeviceDebug::putHex(ch);
#endif

    if (!m_rxBuf.isFull())
      {
        m_rxBuf.put(ch);
#if false
        if (isprint(ch))
          {
            OSDeviceDebug::putChar(ch);
          }
        else
          {
            OSDeviceDebug::putHex(ch);
          }
#endif
        if (m_rxBuf.isAboveHighWM())
          {
            TWCR &= ~_BV(TWEA); // NOT ACK
            m_isReceiving = false;

            os.sched.eventNotify(readEvent);
            OSDeviceDebug::putChar('H');
          }
        else
          {
            TWCR |= _BV(TWEA); // ACK
            m_isReceiving = true;
          }
      }
    else
      {
        // Char lost!
        TWCR &= ~_BV(TWEA); // NOT ACK
        m_isReceiving = false;
        os.sched.eventNotify(readEvent);
        OSDeviceDebug::putChar('N');
      }
    break;

  case 0x88:
    // Previously addressed with own SLA+W; data has been received;
    // NOT ACK has been returned

    ch = TWDR; // The manual says "Read data byte", so we comply

#if false && defined(DEBUG)
    OSDeviceDebug::putHex(status);
    OSDeviceDebug::putChar(' ');
    OSDeviceDebug::putHex(ch);
    OSDeviceDebug::putChar('|');
#endif

    if (m_rxBuf.isBelowLowWM())
      {
        TWCR |= _BV(TWEA); // ACK
        m_isReceiving = true;
      }
    else
      {
        TWCR &= ~_BV(TWEA); // NOT ACK
        m_isReceiving = false;
        os.sched.eventNotify(readEvent);
      }

    // Switched to the not addressed Slave mode;
    // own SLA will be recognised;
    break;

  case 0xA0:
    // A STOP condition or repeated START condition has been received
    // while still addressed as Slave

    // Disable timeout checking
    m_timeoutCounterTicks = 0;

    // Notify readers
    os.sched.eventNotify(readEvent);

    TWCR |= _BV(TWEA); // ACK
    m_isReceiving = true;

    // Switched to the not addressed Slave mode;
    // own SLA will be recognised;
    break;

  default:
#if defined(DEBUG)
    OSDeviceDebug::putChar('I');
    OSDeviceDebug::putHex(status);
    OSDeviceDebug::putChar(' ');
    OSDeviceDebug::putHex(TWCR);
    OSDeviceDebug::putChar(' ');
    OSDeviceDebug::putHex(TWDR);
    OSDeviceDebug::putChar('|');
#endif
    break;
    }

  TWCR |= _BV(TWINT); // clear interrupt
}

// ----- static data ----------------------------------------------------------

DeviceCharacterI2C * DeviceCharacterI2C::pDevice;

//-----------------------------------------------------------------------------

void
DeviceCharacterI2C::interruptTimerTick(void)
{
  if (m_timeoutCounterTicks > 0)
    {
      m_timeoutCounterTicks++;

      if (m_timeoutCounterTicks > OS_CFGINT_TICK_RATE_HZ * 2)
        {
#if true
          os.sched.eventNotify(getReadEvent(), OSEventWaitReturn::OS_TIMEOUT);
          OSDeviceDebug::putChar('T');
#endif
        }
    }
}

// ===== ISRs =================================================================

#if (OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH)

extern "C" void
TWI_vect(void) __attribute__( ( signal, naked ) );

void TWI_vect(void)
{
  OSScheduler::interruptEnter();
    {
      DeviceCharacterI2C::pDevice->interruptServiceRoutine();
    }
  OSScheduler::interruptExit();
}

#else

extern "C"
void TWI_vect( void ) __attribute__( ( signal ) );

void TWI_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterI2C::pDevice->interruptServiceRoutine();
  }

#endif

