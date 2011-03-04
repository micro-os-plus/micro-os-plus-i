/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT
#endif

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (8)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (16)
#endif

#define CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     ((CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ / OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)- 1UL)

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart1::implPortInit(void)
{
  OSScheduler::criticalEnter();
    {
      OSDeviceDebug::putString("Baud constant=");
      OSDeviceDebug::putDec(
          (unsigned short) CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT, 0);
      OSDeviceDebug::putNewLine();
    }
  OSScheduler::criticalExit();

  /* Set the baud rate. */
  UBRR1 = CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT;

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED)
  UCSR1A = _BV(U2X1);
#else
  UCSR1A = 0;
#endif
  /* Enable the Rx interrupt.  The Tx interrupt will be enabled
   later. Also enable the Rx and Tx. */
  UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1);

  /* Set 2-bit stop and the data bits to 8. */
  UCSR1C = ((1 << USBS1) | (3 << UCSZ10));

  return 0;
}

int
DeviceCharacterBufferedUsart1::implPortDisable(void)
{
  ; // TODO: Implement it
  return 0;
}

// ----- read -----------------------------------------------------------------

unsigned char
DeviceCharacterBufferedUsart1::implPortRead(void)
{
  return UDR1;
}


// ----- write ----------------------------------------------------------------

void
DeviceCharacterBufferedUsart1::implPortWrite(unsigned char b)
{
  UDR1 = b;
}

void
DeviceCharacterBufferedUsart1::implInterruptTxEnable(void)
{
// runs in critical section
  UCSR1B |= _BV(UDRIE1);
}

void
DeviceCharacterBufferedUsart1::implInterruptTxDisable(void)
{
// runs in ISR
  UCSR1B &= ~_BV(UDRIE1);
}


// ===== ISRs =================================================================

extern "C" void
USART1_UDRE_vect(void) __attribute__( ( signal, naked ) );

void
USART1_UDRE_vect(void)
{
  // interrupts disabled in here
  OSScheduler::interruptEnter();
    {
      ((DeviceCharacterBufferedUsart1 *) DeviceCharacterBufferedUsart1::ms_pThis)->interruptTxServiceRoutine();
    }
  OSScheduler::interruptExit();
  // interrupts enabled after this point
}

extern "C" void
USART1_RX_vect(void) __attribute__( ( signal, naked ) );

void
USART1_RX_vect(void)
{
  // interrupts disabled in here
  OSScheduler::interruptEnter();
    {
      ((DeviceCharacterBufferedUsart1 *) DeviceCharacterBufferedUsart1::ms_pThis)->interruptRxServiceRoutine();
    }
  OSScheduler::interruptExit();
  // interrupts enabled after this point
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1) */

#endif /* defined(OS_CONFIG_FAMILY_AVR8) */

