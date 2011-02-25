/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_SDI12SENSOR_INLINES_H_
#define HAL_BOARD_SDI12SENSOR_INLINES_H_

inline bool SDI12Sensor::usartWriteAvailable(void)
  {
    return ((UCSR0A & _BV(UDRE0)) != 0);
  }

inline bool SDI12Sensor::usartWriteComplete(void)
  {
    return ((UCSR0A & _BV(TXC0)) != 0);
  }

inline void SDI12Sensor::usartWriteByte(unsigned char ch)
  {
    UDR0 = ch;
  }

inline bool SDI12Sensor::usartReadAvailable(void)
  {
    return ((UCSR0A & _BV(RXC0)) != 0);
  }

inline unsigned char SDI12Sensor::usartReadByte(void)
  {
    return UDR0;
  }

inline bool SDI12Sensor::usartIsFramingError(void)
  {
    return ((UCSR0A & _BV(FE0)) != 0);
  }

inline bool SDI12Sensor::usartIsParityError(void)
  {
    return ((UCSR0A & _BV(UPE0)) != 0);
  }

inline void SDI12Sensor::usartRxEnable(void)
  {
    UCSR0B |= _BV(RXEN1);
  }

inline void SDI12Sensor::usartRxDisable(void)
  {
    UCSR0B &= ~_BV(RXEN1);
  }

#if defined(OS_INCLUDE_SDI12SENSOR)

inline void SDI12Sensor::usartTxEnable(void)
  {
    UCSR0B |= _BV(UDRIE0);
  }

inline void SDI12Sensor::usartTxDisable(void)
  {
    UCSR0B &= ~_BV(UDRIE0);
  }

#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)

inline void SDI12Sensor::usartTxEnable(void)
  {
    UCSR0A |= _BV(TXC0);
    UCSR0B |= _BV(TXEN0);
  }

inline void SDI12Sensor::usartTxDisable(void)
  {
    UCSR0B &= ~_BV(TXEN0);
  }

inline void SDI12Sensor::timerInit(void)
  {
    TCCR2A = 0; // Normal (counter) mode
    TCCR2B = 3; // clk/32 = 1024Hz
    ASSR = _BV( AS2 ); // Timer2 clocked from crystal
    TIMSK2 = 0; // no interrupts enabled   
  }

inline unsigned char SDI12Sensor::timerRead(void)
  {
    // wait while busy
    while((ASSR & _BV(TCN2UB)) != 0)
    ;
    return TCNT2;
  }

#endif

inline void SDI12Sensor::rs485Init(void)
  {
    DDRB |= _BV(DDB0);
    PORTB &= _BV(PB0);
    DDRB |= _BV(DDB1);
  }

inline void SDI12Sensor::rs485Receive(void)
  {
    PORTB &= ~_BV(PB1);
  }

inline void SDI12Sensor::rs485Transmit(void)
  {
    PORTB |= _BV(PB1);
  }

inline void SDI12Sensor::pinInit(void)
  {
    DDRD &= ~_BV(DDD0);
  }

inline bool SDI12Sensor::pinChangedIsHigh(void)
  {
    return ((PIND & _BV(PIND0)) != 0);
  }

#if defined(OS_INCLUDE_SDI12SENSOR)

inline bool SDI12Sensor::pinChanged(unsigned char crt, unsigned char prev)
  {
    return ( ((crt ^ prev) & _BV(PIND0) ) != 0);
  }

inline bool SDI12Sensor::pinChangedIsHigh(unsigned char crt)
  {
    return ((crt & _BV(PIND0)) != 0);
  }

inline void SDI12Sensor::interruptPinChangeEnable(void)
  {
    // RXD0
    PCMSK3 |= _BV(PCINT24);
  }

inline void SDI12Sensor::interruptPinChangeDisable(void)
  {
    PCMSK3 &= ~_BV(PCINT24);
  }

inline void SDI12Sensor::interruptTxCompleteEnable(void)
  {
    UCSR0B |= _BV(TXCIE0);
  }

inline void SDI12Sensor::interruptTxCompleteDisable(void)
  {
    UCSR0B &= ~_BV(TXCIE0);
  }
#endif

#endif /*HAL_BOARD_SDI12SENSOR_INLINES_H_*/
