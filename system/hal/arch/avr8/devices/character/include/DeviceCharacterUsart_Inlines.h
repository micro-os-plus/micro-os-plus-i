/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICECHARACTERUSART_INLINES_H_
#define HAL_DEVICECHARACTERUSART_INLINES_H_

#include "avr/io.h"

#if defined(OS_CONFIG_DEVICECHARACTERUSART_BAUD_CONSTANT)
#define DEVICECHARACTERUSART_BAUD_CONSTANT     OS_CONFIG_DEVICECHARACTERUSART_BAUD_CONSTANT
#else

#if (OS_CONFIG_DEVICECHARACTERUSART_DOUBLE_SPEED)
#define DEVICECHARACTERUSART_PRESCALLER        (8)
#else
#define DEVICECHARACTERUSART_PRESCALLER        (16)
#endif

#define DEVICECHARACTERUSART_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / DEVICECHARACTERUSART_PRESCALLER)
#define DEVICECHARACTERUSART_BAUD_CONSTANT     ((DEVICECHARACTERUSART_CLOCK_HZ / OS_CONFIG_DEVICECHARACTERUSART_BAUD_RATE)- 1UL)

#endif

inline void DeviceCharacterUsart::implPortInit(void)
  {
    OSScheduler::criticalEnter();
      {
        OSDeviceDebug::putString("Baud constant=");
        OSDeviceDebug::putDec( ( unsigned short ) DEVICECHARACTERUSART_BAUD_CONSTANT, 0);
        OSDeviceDebug::putNewLine();
      }
    OSScheduler::criticalExit();

    /* Set the baud rate. */
    UBRR1 = DEVICECHARACTERUSART_BAUD_CONSTANT;

#if (OS_CONFIG_DEVICECHARACTERUSART_DOUBLE_SPEED)
    UCSR1A = _BV(U2X1);
#else
    UCSR1A = 0;
#endif
    /* Enable the Rx interrupt.  The Tx interrupt will be enabled
     later. Also enable the Rx and Tx. */
    UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1);

    /* Set 2-bit stop and the data bits to 8. */
    UCSR1C = ( ( 1 << USBS1 ) | ( 3 << UCSZ10 ));
  }

inline unsigned char DeviceCharacterUsart::implPortRead(void)
  {
    return UDR1;
  }

inline void DeviceCharacterUsart::implPortWrite(unsigned char b)
  {
    UDR1 = b;
  }

inline void DeviceCharacterUsart::implInterruptTxEnable(void)
  {
    UCSR1B |= _BV(UDRIE1);
  }

inline void DeviceCharacterUsart::implInterruptTxDisable(void)
  {
    UCSR1B &= ~_BV(UDRIE1);
  }

#endif /*HAL_DEVICECHARACTERUSART_INLINES_H_*/
