/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER          (8)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER          (16)
#endif

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_RATE           (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_RATE           (9600)
#endif

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT       OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ            (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT       ((CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ / CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_RATE)- 1UL)
#endif

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart1::implPortInit(void)
{
  OSCriticalSection::enter();
    {
    OSDeviceDebug::putString("Baud rate=");
    OSDeviceDebug::putDec(
        (unsigned short) CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_RATE, 0);
    OSDeviceDebug::putNewLine();

      OSDeviceDebug::putString("Baud scale/constant=");
      OSDeviceDebug::putDec(
          (unsigned short) CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER, 0);
      OSDeviceDebug::putChar('/');
      OSDeviceDebug::putDec(
          (unsigned short) CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT, 0);
      OSDeviceDebug::putNewLine();
    }
  OSCriticalSection::exit();

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
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION)
__attribute__((naked))
#else
__attribute__((signal))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION)*/
USART1_UDRE_vect(void);

void
USART1_UDRE_vect(void)
{
  // interrupts disabled in here
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION)
  OSScheduler::interruptEnter();
#else
  OSScheduler::ISR_ledActiveOn();
#endif /* !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION) */
    {
      DeviceCharacterBufferedUsart1::ms_pThis->interruptTxServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION)
  OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_TX_ISR_PREEMPTION) */
  // interrupts enabled after this point
}

extern "C" void
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION)
__attribute__((naked))
#else
__attribute__((signal))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION)*/
USART1_RX_vect(void);

void
USART1_RX_vect(void)
{
  // interrupts disabled in here
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION)
  OSScheduler::interruptEnter();
#else
  OSScheduler::ISR_ledActiveOn();
#endif /* !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION) */
    {
      DeviceCharacterBufferedUsart1::ms_pThis->interruptRxServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION)
  OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_RX_ISR_PREEMPTION) */
  // interrupts enabled after this point
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1) */

#endif /* defined(OS_CONFIG_FAMILY_AVR8) */

