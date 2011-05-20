/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "DeviceCharacterI2C.h"

// set custom high/low water marks
DeviceCharacterI2C::DeviceCharacterI2C(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM,
    unsigned char *pRxBuf, unsigned short rxBufSize, unsigned short rxHWM,
    unsigned short rxLWM) :
  m_txBuf(pTxBuf, txBufSize, txHWM, txLWM), m_rxBuf(pRxBuf, rxBufSize, rxHWM,
      rxLWM)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterI2C"), this);

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  m_type = DEVICECHARACTER_USART;
#endif

  pDevice = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterI2C::DeviceCharacterI2C(unsigned char *pTxBuf,
    unsigned short txBufSize, unsigned char *pRxBuf, unsigned short rxBufSize) :
  m_txBuf(pTxBuf, txBufSize, txBufSize * 3 / 4, txBufSize / 4), m_rxBuf(pRxBuf,
      rxBufSize, rxBufSize * 3 / 4, rxBufSize / 4)
{
  OSDeviceDebug::putConstructor_P(PSTR("DeviceCharacterI2C"), this);

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)
  m_type = DEVICECHARACTER_USART;
#endif

  pDevice = this;
}

DeviceCharacterI2C::~DeviceCharacterI2C()
{
  OSDeviceDebug::putDestructor_P(PSTR("DeviceCharacterI2C"), this);
}

int
DeviceCharacterI2C::implOpen(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString("DeviceCharacterI2C::implOpen()");
  OSDeviceDebug::putNewLine();
#endif

  TWBR = 2; // 400000 at 8MHz
  TWSR = 0; // prescaller = 1

  TWAR = (0x28 << 1); // slave address
  TWAMR = 0; // no mask

  TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWIE);

  m_rxBuf.clear();

  return 0;
}

OSEvent_t
DeviceCharacterI2C::implGetOpenEvent(void)
{
  return OSEvent::OS_NONE;
}

int
DeviceCharacterI2C::implClose(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString("DeviceCharacterI2C::implClose()");
  OSDeviceDebug::putNewLine();
#endif

  TWCR = 0; // disable interface

  // leave bits as input
  DDRD &= ~(_BV(0) | _BV(1));

  return 0;
}

bool
DeviceCharacterI2C::implIsConnected() const
{
  return true;
}

// ----- write ----------------------------------------------------------------

bool
DeviceCharacterI2C::implCanWrite(void)
{
  return !m_txBuf.isFull();
}

OSEvent_t
DeviceCharacterI2C::implGetWriteEvent(void)
{
  return (OSEvent_t) &m_txBuf;
}

int
DeviceCharacterI2C::implWriteByte(unsigned char b)
{
  m_txBuf.put(b);

  if (m_txBuf.isAboveHighWM())
    interruptTxEnable(); // start transmission

  return b;
}

int
DeviceCharacterI2C::implFlush(void)
{
  //OSDeviceDebug::putChar('f');
  if (!m_txBuf.isEmpty())
    interruptTxEnable();

  return 0;
}

void
DeviceCharacterI2C::interruptServiceRoutine(void)
{
  //OSDeviceDebug::putChar('!');
  unsigned char status;

  status = TWSR & ~0x03;

  switch (status)
    {
  case 0x60:
    // Own SLA+W received, ACK returned
    if (!m_rxBuf.isFull())
      TWCR |= _BV(TWEA); // ACK
    else
      TWCR &= ~_BV(TWEA); // NOT ACK

    break;

  case 0x80:
    // Data received
    if (!m_rxBuf.isFull())
      {
        m_rxBuf.put(portRead());
        os.sched.eventNotify(implGetReadEvent());
        if (!m_rxBuf.isFull())
          TWCR |= _BV(TWEA); // ACK
        else
          TWCR &= ~_BV(TWEA); // NOT ACK

      }
    else
      {
        // Normally should not reach this point
        // since character is lost...
        TWCR &= ~_BV(TWEA); // NOT ACK
      }
    break;

  case 0x88:
    //TWCR &= ~ ( _BV(TWSTO)); // clear stop
    TWCR |= _BV(TWEA); // ACK
    break;

  case 0xA0:
    // STOP or repeated START received
    TWCR |= _BV(TWEA); // ACK
    break;

  default:
#if defined(DEBUG)
    OSDeviceDebug::putHex(status);
    OSDeviceDebug::putChar(' ');
    OSDeviceDebug::putHex(TWDR);
    OSDeviceDebug::putChar('|');
#endif
    ;
    }

  TWCR |= _BV(TWINT); // clear interrupt
}

// ----- read -----------------------------------------------------------------

bool
DeviceCharacterI2C::implCanRead()
{
  return !m_rxBuf.isEmpty();
}

int
DeviceCharacterI2C::implAvailableRead()
{
  return !m_rxBuf.length();
}

OSEvent_t
DeviceCharacterI2C::implGetReadEvent(void)
{
  return (OSEvent_t) &m_rxBuf;
}

int
DeviceCharacterI2C::implReadByte(void)
{
  return m_rxBuf.get();
}

// ----- static data ----------------------------------------------------------

DeviceCharacterI2C * DeviceCharacterI2C::pDevice;

//-----------------------------------------------------------------------------

inline void
DeviceCharacterI2C::interruptTxEnable(void)
{
  ;
}

inline void
DeviceCharacterI2C::interruptTxDisable(void)
{
  ;
}

inline unsigned char
DeviceCharacterI2C::portRead(void)
{
  return TWDR;
}

inline void
DeviceCharacterI2C::portWrite(unsigned char b)
{
  TWDR = b;
}

// ===== ISRs =================================================================

#if (OS_CFGINT_DEVICECHARACTERI2C_CONTEXT_SWITCH)

extern "C" void
TWI_vect(void) __attribute__( ( signal, naked ) );

void
TWI_vect(void)
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

