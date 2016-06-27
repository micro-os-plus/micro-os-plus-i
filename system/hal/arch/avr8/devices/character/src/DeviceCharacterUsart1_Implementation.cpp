/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

// DEPRECATED

#if  defined(OS_INCLUDE_DEVICECHARACTERUSART1)

#include "portable/devices/character/include/DeviceCharacterUsart1.h"

#if defined(OS_CONFIG_DEVICECHARACTERUSART1_BAUD_CONSTANT)
#define DEVICECHARACTERUSART1_BAUD_CONSTANT     OS_CONFIG_DEVICECHARACTERUSART1_BAUD_CONSTANT
#else

#if (OS_CONFIG_DEVICECHARACTERUSART1_DOUBLE_SPEED)
#define DEVICECHARACTERUSART1_PRESCALLER        (8)
#else
#define DEVICECHARACTERUSART1_PRESCALLER        (16)
#endif

#define DEVICECHARACTERUSART1_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / DEVICECHARACTERUSART1_PRESCALLER)
#define DEVICECHARACTERUSART1_BAUD_CONSTANT     ((DEVICECHARACTERUSART1_CLOCK_HZ / OS_CONFIG_DEVICECHARACTERUSART1_BAUD_RATE)- 1UL)

#endif

// ---------------------------------------------------------------------------

int DeviceCharacterUsart1::implPortInit(void)
  {
    OSCriticalSection::enter();
      {
        OSDeviceDebug::putString_P(PSTR("Baud constant="));
        OSDeviceDebug::putDec(
            (unsigned short) DEVICECHARACTERUSART1_BAUD_CONSTANT, 0);
        OSDeviceDebug::putNewLine();
      }
    OSCriticalSection::exit();

    /* Set the baud rate. */
    UBRR1 = DEVICECHARACTERUSART1_BAUD_CONSTANT;

#if (OS_CONFIG_DEVICECHARACTERUSART1_DOUBLE_SPEED)
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

unsigned char DeviceCharacterUsart1::implPortRead(void)
  {
    return UDR1;
  }

void DeviceCharacterUsart1::implPortWrite(unsigned char b)
  {
    UDR1 = b;
  }

void DeviceCharacterUsart1::implInterruptTxEnable(void)
  {
    UCSR1B |= _BV(UDRIE1);
  }

void DeviceCharacterUsart1::implInterruptTxDisable(void)
  {
    UCSR1B &= ~_BV(UDRIE1);
  }

// ===== ISRs ================================================================

#if (OS_CFGBOOL_DEVICECHARACTERUSART1_TX_CONTEXT_SWITCH)

extern "C"
void  USART1_UDRE_vect(void) __attribute__((signal, naked));

void USART1_UDRE_vect(void)
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterUsart1::ms_pThis->interruptTxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART1_UDRE_vect(void) __attribute__((signal));

void USART1_UDRE_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterUsart1::ms_pThis->interruptTxServiceRoutine();
  }

#endif /* OS_CFGBOOL_DEVICECHARACTERUSART1_TX_CONTEXT_SWITCH */

#if (OS_CFGBOOL_DEVICECHARACTERUSART1_RX_CONTEXT_SWITCH)

extern "C"
void  USART1_RX_vect(void) __attribute__((signal, naked));

void USART1_RX_vect(void)
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterUsart1::ms_pThis->interruptRxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART1_RX_vect(void) __attribute__((signal));

void USART1_RX_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterUsart1::ms_pThis->interruptRxServiceRoutine();
  }

#endif /* OS_CFGBOOL_DEVICECHARACTERUSART1_RX_CONTEXT_SWITCH */

#endif /* OS_INCLUDE_DEVICECHARACTERUSART1 */
