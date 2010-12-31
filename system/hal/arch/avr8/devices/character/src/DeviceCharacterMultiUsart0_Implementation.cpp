/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART0)

#include "portable/devices/character/include/DeviceCharacterMultiUsart0.h"

#if defined(OS_CONFIG_DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT)
#define DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT     OS_CONFIG_DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT
#else

#if (OS_CONFIG_DEVICECHARACTERMULTIUSART0_DOUBLE_SPEED)
#define DEVICECHARACTERMULTIUSART0_PRESCALLER        (8)
#else
#define DEVICECHARACTERMULTIUSART0_PRESCALLER        (16)
#endif

#define DEVICECHARACTERMULTIUSART0_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / DEVICECHARACTERMULTIUSART0_PRESCALLER)
#define DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT     ((DEVICECHARACTERMULTIUSART0_CLOCK_HZ / OS_CONFIG_DEVICECHARACTERMULTIUSART0_BAUD_RATE)- 1UL)

#endif

// ---------------------------------------------------------------------------

void DeviceCharacterMultiUsart0::implPortInit(void)
  {
    OSScheduler::criticalEnter();
      {
        OSDeviceDebug::putString("Baud constant=");
        OSDeviceDebug::putDec( ( unsigned short ) DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT, 0);
        OSDeviceDebug::putNewLine();
      }
    OSScheduler::criticalExit();

    /* Set the baud rate. */
    UBRR0 = DEVICECHARACTERMULTIUSART0_BAUD_CONSTANT;

#if (OS_CONFIG_DEVICECHARACTERMULTIUSART0_DOUBLE_SPEED)
    UCSR0A = _BV(U2X1);
#else
    UCSR0A = 0;
#endif
    /* Enable the Rx interrupt.  The Tx interrupt will be enabled
     later. Also enable the Rx and Tx. */
    UCSR0B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1);

    /* Set 2-bit stop and the data bits to 8. */
    UCSR0C = ( ( 1 << USBS1 ) | ( 3 << UCSZ10 ));
  }

unsigned char DeviceCharacterMultiUsart0::implPortRead(void)
  {
    return UDR0;
  }

void DeviceCharacterMultiUsart0::implPortWrite(unsigned char b)
  {
    UDR0 = b;
  }

void DeviceCharacterMultiUsart0::implInterruptTxEnable(void)
  {
    UCSR0B |= _BV(UDRIE1);
  }

void DeviceCharacterMultiUsart0::implInterruptTxDisable(void)
  {
    UCSR0B &= ~_BV(UDRIE1);
  }

// ===== ISRs ================================================================

#if (OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_TX_CONTEXT_SWITCH)

extern "C" void USART0_UDRE_vect( void ) __attribute__( ( signal, naked ) );

void
USART0_UDRE_vect( void )
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterMultiUsart0::ms_pThis->interruptTxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART0_UDRE_vect( void ) __attribute__( ( signal ) );

void USART0_UDRE_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterMultiUsart0::ms_pThis->interruptTxServiceRoutine();
  }

#endif /*OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_TX_CONTEXT_SWITCH*/

#if (OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_RX_CONTEXT_SWITCH)

extern "C" void USART0_RX_vect( void ) __attribute__( ( signal, naked ) );

void
USART0_RX_vect( void )
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterMultiUsart0::ms_pThis->interruptRxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART0_RX_vect( void ) __attribute__( ( signal ) );

void USART0_RX_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterMultiUsart0::ms_pThis->interruptRxServiceRoutine();
  }

#endif /*OS_CFGBOOL_DEVICECHARACTERMULTIUSART0_RX_CONTEXT_SWITCH*/


#endif /*OS_INCLUDE_DEVICECHARACTERMULTIUSART0*/
