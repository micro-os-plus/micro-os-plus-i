/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "portable/kernel/include/OS.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

#if !defined(OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif
void
SysTick_contextHandler(void);

__attribute__((noinline)) void
SysTick_interruptServiceRoutine();

void
OSTimerTicks::implInit(void)
{

  volatile avr32_pm_t* pm = &AVR32_PM;
  volatile avr32_tc_t* tc_reg = &OS_CFGVAR_TIMER;

  //initialise  exception vector base address
  INTC_init_interrupts();

  // enable clock for TC0
#if OS_CFGINT_TIMER_ID == 0
  pm->pbamask |= 1 << 16;
#elif OS_CFGINT_TIMER_ID == 1
  pm->pbamask |= 1 << 17;
#else
#error "Illegal OS_CFGINT_TIMER_ID"
#endif

  // initialise registers for every Channel

  // disable channel
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CCR.clkdis = 1;

  // clear interrupt flags, by reading SR
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].sr;

  // reset register
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].cmr = 0;

  // set waveform mode as default
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.wave = 1;

  // for waveform mode the TIOB must not be input
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.eevt = 2;
  //disable all interrupt sources
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].idr = 0xFFFFFFFF;

  //register the interrupt
  INTC_register_interrupt(SysTick_contextHandler, OS_CFGINT_TIMER_IRQ_ID,
      OS_CFGINT_TIMER_IRQ_LEVEL);

  //clock source
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.tcclks
      = OS_CFGINT_TIMER_CLOCK_SELECT;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.wavsel = 2;
  // no event on TIOA when RC compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.acpc = 0;
  // no event on TIOB when RC compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.bcpc = 0;
  // enable channel, as it might be disabled
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CCR.clken = 1;

  // start channel using software trigger
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CCR.swtrg = 1;

#define DIVIDER (1 << ((OS_CFGINT_TIMER_CLOCK_SELECT-2)*2+1))
#define COUNTER (OS_CFGLONG_OSCILLATOR_HZ/OS_CFGINT_TIMER_PRESCALLER/DIVIDER/OS_CFGINT_TICK_RATE_HZ)

#if defined(DEBUG)

  OSDeviceDebug::putDec(OS_CFGINT_TIMER_PRESCALLER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(DIVIDER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(COUNTER);
  OSDeviceDebug::putNewLine();

#endif

  // set RC value
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].RC.rc = COUNTER;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.wavsel = 2;

  // set interrupt source RC Compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].IER.cpcs = 1;

}

#if defined(OS_INCLUDE_SYSTICK_CONTEXT_HANDLER_UNDER_CONSTRUCTION)

void
SysTick_contextHandler(void)
  {
#if true
    OSSchedulerImpl::registersSave();
    if (OSSchedulerImpl::isContextSwitchAllowed())
      {
        OSSchedulerImpl::stackPointerSave();
      }
    else
    OSImpl::NOP();
#endif
    OS_CFGVAR_TIMER.channel[OS_CFGINT_TIMER_CHANNEL].sr;

    OSScheduler::timerTicks.interruptServiceRoutine();
#if true
    if (OSSchedulerImpl::isContextSwitchAllowed())
      {
#if true
        if (OSScheduler::isContextSwitchRequired())
          {
            OSScheduler::performContextSwitch();
          }
#endif
        OSSchedulerImpl::stackPointerRestore();
      }
    else
    OSImpl::NOP();
    OSSchedulerImpl::registersRestore();

    OSImpl::returnFromInterrupt();
#endif

  }

#else

void
SysTick_contextHandler(void)
{
#if !defined(OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR)
  OSScheduler::interruptEnter();
#endif
    {
      OSScheduler::timerTicks.interruptServiceRoutine();
    }
#if !defined(OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR)
  OSScheduler::interruptExit();
#endif
}

#endif

void
OSTimerTicks::implAcknowledgeInterrupt(void)
{
  OS_CFGVAR_TIMER.channel[OS_CFGINT_TIMER_CHANNEL].sr;
}

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)

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

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
