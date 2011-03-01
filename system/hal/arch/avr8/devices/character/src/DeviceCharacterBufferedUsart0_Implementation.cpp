/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart0.h"

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_CONSTANT)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_CONSTANT
#endif

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_DOUBLE_SPEED)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (8)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (16)
#endif

#define CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     ((CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ / OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_RATE)- 1UL)

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart0::implPortInit(void)
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
  UBRR0 = CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT;

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_DOUBLE_SPEED)
  UCSR0A = _BV(U2X1);
#else
  UCSR0A = 0;
#endif
  /* Enable the Rx interrupt.  The Tx interrupt will be enabled
   later. Also enable the Rx and Tx. */
  UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);

  /* Set 2-bit stop and the data bits to 8. */
  UCSR0C = ((1 << USBS1) | (3 << UCSZ10));

  return 0;
}

int
DeviceCharacterBufferedUsart0::implPortDisable(void)
{
  ; // TODO: Implement it
  return 0;
}

// ----- read -----------------------------------------------------------------

unsigned char
DeviceCharacterBufferedUsart0::implPortRead(void)
{
  return UDR0;
}


// ----- write ----------------------------------------------------------------

void
DeviceCharacterBufferedUsart0::implPortWrite(unsigned char b)
{
  UDR0 = b;
}

void
DeviceCharacterBufferedUsart0::implInterruptTxEnable(void)
{
  UCSR0B |= _BV(UDRIE0);
}

void
DeviceCharacterBufferedUsart0::implInterruptTxDisable(void)
{
  UCSR0B &= ~_BV(UDRIE0);
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
      ((DeviceCharacterBufferedUsart0 *) DeviceCharacterBufferedUsart0::ms_pThis)->interruptTxServiceRoutine();
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
      ((DeviceCharacterBufferedUsart0 *) DeviceCharacterBufferedUsart0::ms_pThis)->interruptRxServiceRoutine();
    }
  OSScheduler::interruptExit();
  // interrupts enabled after this point
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) */

#endif /* defined(OS_CONFIG_FAMILY_AVR8) */

