/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#if defined(OS_CONFIG_ARCH_AVR8)

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_SDI12SENSOR)
#include "portable/devices/sdi12/include/SDI12Sensor.h"
#endif

void OSTimerTicks::implInit(void)
  {
    // Using 8bit timer0/16bit timer 1 to generate the tick.
    // The correct fuses must be selected for the main oscillator frequency.

    // We only have 8/16 bits so have to scale to get our required tick rate.
    // and adjust for correct value.

#define TIMER_CLOCK_HZ	(OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / OS_CFGINT_TIMER_PRESCALLER)
#define TIMER_CONSTANT	((TIMER_CLOCK_HZ / OS_CFGINT_TICK_RATE_HZ)- 1UL)
#define TIMER_FREQ_HZ		(TIMER_CLOCK_HZ/(TIMER_CONSTANT+1UL))

#if defined(DEBUG)

    unsigned short timer_constant;
    unsigned short timer_freq;
    unsigned short timer_precision;
    timer_constant = TIMER_CONSTANT;

#if !defined(OS_INCLUDE_OSSCHEDULER_16BIT_TIMER)

    timer_constant &= 0xFF;

#endif

    timer_freq = TIMER_CLOCK_HZ / ( timer_constant + 1 );
    if (timer_freq > OS_CFGINT_TICK_RATE_HZ)
      timer_precision = (timer_freq - OS_CFGINT_TICK_RATE_HZ ) * 100
          / OS_CFGINT_TICK_RATE_HZ;
    else
      timer_precision = ( OS_CFGINT_TICK_RATE_HZ - timer_freq ) * 100
          / OS_CFGINT_TICK_RATE_HZ;
    if (timer_precision > 3)
      {
        OSDeviceDebug::putDec(timer_freq);
        OSDeviceDebug::putChar( ' ');
        OSDeviceDebug::putDec(timer_precision);
        OSDeviceDebug::putNewLine();
      }

#endif

    /* Setup compare match value for compare match A.  Interrupts are disabled
     before this is called so we need not worry here. */
#if defined(OS_INCLUDE_OSSCHEDULER_16BIT_TIMER)

    OCR1A = TIMER_CONSTANT;

    /* Setup clock source and compare match behaviour. */
    /* normal A, B, C, */
    TCCR1A = 0;

    /* CTC (wgm=4), clk */
    TCCR1B = _BV( WGM12 ) | ( OS_CFGINT_TIMER_CLOCK_SELECT << CS10 );

    /* force output compare for channel A */
    TCCR1C = _BV( FOC1A );

    /* Enable the interrupt - this is okay as interrupt are currently globally
     disabled. */
    TIMSK1 |= _BV( OCIE1A );

#else

    OCR0A = TIMER_CONSTANT;

    /* Setup clock source and compare match behaviour. */
#if defined (__AVR_AT90CAN128__)

    /* CTC (WGM=2), force output compare for channel A, clk */
    TCCR0A = _BV(FOC0A) | _BV(WGM01) | (OS_CFGINT_TIMER_CLOCK_SELECT << CS00);

#else
    /* CTC (WGM=2), normal A, B */
    TCCR0A = (2 << WGM00);

    /* force output compare for channel A, clk */
    TCCR0B = _BV(FOC0A) | (OS_CFGINT_TIMER_CLOCK_SELECT << CS00);
#endif

    /* Enable the compare match A interrupt;
     * this is okay as interrupts are currently globally disabled.
     */
    TIMSK0 |= _BV(OCIE0A );

#endif

  }

void OSTimerTicks::implAcknowledgeInterrupt()
{
  // nothing to do
}

#if defined(OS_INCLUDE_OSSCHEDULER_16BIT_TIMER)

#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
extern "C" __attribute__((naked))
#else
extern "C" __attribute__((signal))
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
void TIMER1_COMPA_vect(void);

void
TIMER1_COMPA_vect(void)
  {
#if true

  OSTimerTicks::interruptContextHandler();

#else

    // interrupts disabled in here
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptEnter();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
      {
        OSScheduler::timerTicks.interruptServiceRoutine();
      }
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
    // interrupts enabled after this point

#endif
  }

#else

#if defined (__AVR_AT90CAN128__)

#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
extern "C" __attribute__((naked))
#else
extern "C" __attribute__((signal))
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
void TIMER0_COMP_vect(void);

void TIMER0_COMP_vect(void)
  {
#if true

  OSTimerTicks::interruptContextHandler();

#else

    // interrupts disabled in here
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptEnter();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
      {
        OSScheduler::timerTicks.interruptServiceRoutine();
      }
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
    // interrupts enabled after this point

#endif
  }

#else

#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
extern "C" __attribute__((naked))
#else
extern "C" __attribute__((signal))
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
void  TIMER0_COMPA_vect(void);

void TIMER0_COMPA_vect(void)
  {
#if true

  OSTimerTicks::interruptContextHandler();

#else
    // interrupts disabled in here
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptEnter();
#else
    OSScheduler::ISR_ledActiveOn();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
      {
        OSScheduler::timerTicks.interruptServiceRoutine();
      }
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
    OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */
    // interrupts enabled after this point
#endif
  }

#endif /* __AVR_AT90CAN128__ */

#endif

#if defined(OS_INCLUDE_OSTIMERSECONDS)

void OSTimerSeconds::implAcknowledgeInterrupt()
{
  // nothing to do
}

void OSTimerSeconds::init(void)
  {
#if defined(OS_INCLUDE_32KHZ_TIMER)
    TCCR2A = 0; // Normal (counter) mode
    TCCR2B = 5; // clk/128
    ASSR = _BV( AS2 ); // Timer2 clocked from crystal
    TIMSK2 = _BV( TOIE2 );
#endif
  }

#if defined(OS_INCLUDE_32KHZ_TIMER)

extern "C" void TIMER2_OVF_vect(void) __attribute__((signal, naked));

void
TIMER2_OVF_vect(void)
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        OSScheduler::timerSeconds.interruptServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#endif

#endif

#endif /* defined(OS_CONFIG_ARCH_AVR8) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
