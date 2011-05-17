/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if (defined(OS_CONFIG_BOARD_METRILOG_M512) || defined(OS_CONFIG_BOARD_METRILOG_M514)) && (defined(OS_INCLUDE_SDI12SENSOR) || defined(OS_INCLUDE_SDI12SENSOR_TINY))

#include "portable/devices/sdi12/include/SDI12Sensor.h"

#define OS_SDI12SENSOR_CFGINT_USART_BAUD_RATE	(1200)

#if (OS_SDI12SENSOR_CFGINT_USART_DOUBLE_SPEED)
#define SDI_USART_PRESCALLER	(8)
#else
#define SDI_USART_PRESCALLER	(16)
#endif

#define SDI_USART_CLOCK_HZ	(OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / SDI_USART_PRESCALLER)
#define SDI_USART_BAUD_CONSTANT	((SDI_USART_CLOCK_HZ / OS_SDI12SENSOR_CFGINT_USART_BAUD_RATE)- 1UL)

void SDI12Sensor::init(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString("SDI12Sensor::init()");
    OSDeviceDebug::putNewLine();
#endif

    ms_ownAddress = DEFAULT_ADDRESS;

#if defined(OS_INCLUDE_SDI12SENSOR)
    ms_useCRC = false;
    ms_cmdCRC = false;

    ms_dCount = 0;
    ms_rCount = 0;
#endif

    // Enable RS485 receiver
    rs485Init();
    rs485Receive();

    // RXD0 as input
    pinInit();

    UBRR0 = SDI_USART_BAUD_CONSTANT;

    //debug.putDec(SDI_USART_BAUD_CONSTANT);
    //debug.putNewLine();

#if defined(OS_SDI12SENSOR_CFGINT_USART_DOUBLE_SPEED)

    //debug.putChar('d');
    UCSR0A = _BV(U2X0);
#else

    UCSR0A = 0;
#endif

#if defined(OS_INCLUDE_SDI12SENSOR)
    /* Enable the Rx interrupt.  The Tx interrupt will be enabled
     later. Also enable the Tx. Rx will be enabled after break */
    UCSR0B = _BV(RXCIE0) | _BV(TXEN0);
#endif

    /* Set even parity, 1-bit stop and the data bits to 7. */
    UCSR0C = ((2 << UPM00) | (2 << UCSZ00));

#if defined(OS_INCLUDE_SDI12SENSOR)
#if !defined(OS_EXCLUDE_PCINT3_VECT)
    interruptPinChangeDisable();

    PCIFR &= ~_BV(PCIF3);
    // Enable Pin Change Interrupt group 3
    PCICR |= _BV(PCIE3);

    ms_prevValue3 = 0;

#endif

    ms_ticksBRK = 0;
    //interruptPinChangeEnable();
    ms_breakDetectEnable = true;

#endif
  }

#if !defined(OS_INCLUDE_SDI12SENSOR_TINY)

#if !defined(OS_EXCLUDE_PCINT3_VECT)

extern "C"
void  PCINT3_vect(void) __attribute__((signal, naked));

void PCINT3_vect(void)
  {
    OSScheduler::contextSave(); // interrupts disabled in here
      {
        OSScheduler::ISRledActiveOn();

        unsigned char crt;
        crt = PIND;

        // beware of race conditions when bits change during the ISR
        PulseCounter::interruptPinChange3ServiceRoutine(crt,
            PinChangeDispatchers::ms_prevValue3);

        SDI12Sensor::interruptPinChangeServiceRoutine(crt,
            PinChangeDispatchers::ms_prevValue3);

        PinChangeDispatchers::ms_prevValue3 = crt;

        if (OSScheduler::requireContextSwitch())
          {
            OSScheduler::contextSwitch(true);
          }
      }
    OSScheduler::contextRestore();
    OS::returnFromInterrupt();
  }

#endif

extern "C"
void USART0_RX_vect(void) __attribute__((signal, naked));

void USART0_RX_vect(void)
  {
    OSScheduler::contextSave(); // interrupts disabled in here

      {
        //OSDeviceDebug::putChar('@');

        OSScheduler::ISRledActiveOn();

        SDI12Sensor::interruptRxServiceRoutine();

        if (OSScheduler::requireContextSwitch())
          {
            OSScheduler::contextSwitch();
          }
      }
    OSScheduler::contextRestore();
    OS::returnFromInterrupt();
    // interrupts enabled after this point
  }

extern "C"
void USART0_UDRE_vect(void) __attribute__((signal));

void USART0_UDRE_vect(void)
  {
    SDI12Sensor::interruptTxServiceRoutine();
  }

extern "C"
void USART0_TX_vect(void) __attribute__((signal));

void USART0_TX_vect(void)
  {
    SDI12Sensor::interruptTxCompleteServiceRoutine();
  }

#endif
#endif



