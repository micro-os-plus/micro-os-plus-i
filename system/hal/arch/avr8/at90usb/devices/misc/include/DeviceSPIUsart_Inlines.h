/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICESPIUSART_INLINES_H_
#define DEVICESPIUSART_INLINES_H_

inline void DeviceSPIUsart::implSpiInit(unsigned short baud)
  {
    UBRR1 = 0; // should be set after transmitter enabled

    DDRD |= _BV(PD5); // set clock to output

    // SPI mode, MSB first, mode 0
    UCSR1C = _BV(UMSEL11) | _BV(UMSEL10);

    // RX/TX enable
    UCSR1B = _BV(RXEN1) | _BV(TXEN1);

    // set baud rate at Fosc/(2*(x+1))
    UBRR1 = baud; // must be set after transmitter enabled
  }

inline unsigned char DeviceSPIUsart::implSpiReadWrite(unsigned char c)
  {
    while ((UCSR1A & _BV(UDRE1)) == 0)
      ; // wait for empty transmit buffer

    UDR1 = c;
    while ((UCSR1A & _BV(RXC1)) == 0)
      ; // wait for data to be received

    return UDR1;
  }

#endif /* DEVICESPIUSART_INLINES_H_ */
