/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_CONFIG_ARCH_AVR32)

#include "portable/kernel/include/OS.h"
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

static int i;


__attribute__((__interrupt__)) void TikTestIsr(void);

void OSTimerTicks::init(void)
  {
  volatile avr32_pm_t* pm = &AVR32_PM;
  volatile avr32_tc_t* tc_reg = &AVR32_TC0;
#if true
  //initialize  exception vector base address
  INTC_init_interrupts();
  // enable clock for TC0
  pm->pbamask |= 1 << 16;
  // initialize registers for every Channel
  // disable channel
  tc_reg->channel[0].CCR.clkdis = 1;
  // clear interrupt flags, by reading SR
  tc_reg->channel[0].sr;
  // reset register
  tc_reg->channel[0].cmr = 0;
  // set waveform mode as default
  tc_reg->channel[0].CMR.waveform.wave = 1;
  // for waveform mode the TIOB must not be input
  tc_reg->channel[0].CMR.waveform.eevt = 2;
  //disable all interrupt sources
  tc_reg->channel[0].idr = 0xFFFFFFFF;
  //register the interrupt
  INTC_register_interrupt(TikTestIsr,AVR32_TC0_IRQ0,0);
  //clock source PBA/2;
  tc_reg->channel[0].CMR.waveform.tcclks = 1;
  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[0].CMR.waveform.wavsel = 2;
  // no event on TIOA when RC compare
  tc_reg->channel[0].CMR.waveform.acpc = 0;
  // no event on TIOB when RC compare
  tc_reg->channel[0].CMR.waveform.bcpc = 0;
  // enable channel, as it might be disabled
  tc_reg->channel[0].CCR.clken = 1;
  // start channel using software trigger
  tc_reg->channel[0].CCR.swtrg = 1;
  // set RC value
  tc_reg->channel[0].RC.rc = 0xFFFF;
   // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[0].CMR.waveform.wavsel = 2;
   // set interrupt source RC Compare
  tc_reg->channel[0].IER.cpcs = 1;
#endif
#if defined(DEBUG)

    OSDeviceDebug::putString("OSTimerTicks::init()");
    OSDeviceDebug::putNewLine();

#endif

    i = 0;
  }

void SysTick_Handler(void)
  {
    /* This is an interrupt function. */
    OSScheduler::ISRledActiveOn();

    OSScheduler::timerTicks.interruptServiceRoutine();

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT)
    if (++OSTimerSeconds::ms_schedulerTicks == OS_CFGINT_TICK_RATE_HZ)
      {
        OSTimerSeconds::ms_schedulerTicks = 0;

        OSScheduler::timerSeconds.interruptServiceRoutine();
      }
#endif

#if defined(DEBUG)

    if ((i++ % OS_CFGINT_TICK_RATE_HZ) == 0)
      OSDeviceDebug::putChar('!');

#endif

    if (OSScheduler::requireContextSwitch())
      OSScheduler::ISRcontextSwitchRequest();
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

__attribute__((__interrupt__)) void TikTestIsr(void)
{
  volatile avr32_tc_t* tc_reg = &AVR32_TC0;

 int val =  tc_reg->channel[0].cv;
#if defined(DEBUG)

    if ((i++ % OS_CFGINT_TICK_RATE_HZ) == 0)
      OSDeviceDebug::putChar('!');

#endif
}

#endif /* defined(OS_CONFIG_ARCH_AVR32) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
