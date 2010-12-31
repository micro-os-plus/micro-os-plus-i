/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEDEBUGUSART_INLINES_H_
#define HAL_DEVICEDEBUGUSART_INLINES_H_

#include "avr/io.h"
#include "avr/wdt.h"

// ---------------------------------------------------------------------------

#if defined(OS_CONFIG_DEBUG_USART_BAUD_CONSTANT)

#define DEBUG_USART_BAUD_CONSTANT	OS_CONFIG_DEBUG_USART_BAUD_CONSTANT

#else

#ifndef OS_CONFIG_DEBUG_USART_BAUD_RATE
#define OS_CONFIG_DEBUG_USART_BAUD_RATE			(19200)
#endif

#if (OS_CONFIG_DEBUG_USART_DOUBLE_SPEED)
#define DEBUG_USART_PRESCALLER	(8)
#else
#define DEBUG_USART_PRESCALLER	(16)
#endif

#define DEBUG_USART_CLOCK_HZ		(OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / DEBUG_USART_PRESCALLER)
#define DEBUG_USART_BAUD_CONSTANT	((DEBUG_USART_CLOCK_HZ / OS_CONFIG_DEBUG_USART_BAUD_RATE)- 1UL)

#endif

inline void OSDeviceDebug::implEarlyInit(void)
  {
    /* Set the baud rate. */
    UBRR1 = DEBUG_USART_BAUD_CONSTANT;

#if (OS_CONFIG_DEBUG_USART_DOUBLE_SPEED)

    UCSR1A = _BV( U2X1 );
#else

    UCSR1A = 0;
#endif
    /* Enable the Rx and Tx. */
    UCSR1B = _BV( TXEN1 );

    /* Set 2-bit stop and the data bits to 8. */
    UCSR1C = ( ( 1 << USBS1 ) | ( 3 << UCSZ10 ) );

  }

inline bool OSDeviceDebug::implDevicePresent(void)
  {
    return true;
  }

inline bool OSDeviceDebug::implCanTransmit(void)
  {
    return ( (UCSR1A & _BV( UDRE1 ) ) != 0 );
  }

inline void OSDeviceDebug::implPutByte(unsigned char b)
  {
    UDR1 = b;
  }

inline void OSDeviceDebug::implWDReset(void)
  {
    wdt_reset()
;}

#endif /*HAL_DEVICEDEBUGUSART_INLINES_H_*/
