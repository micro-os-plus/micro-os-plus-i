/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#include "portable/kernel/include/OS.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"
#include "hal/arch/avr32/uc3/lib/include/tc.h"
#include "hal/arch/avr32/uc3/lib/include/rtc.h"

#define COUNT_IRQ_NUM               0

#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)*/
void
SysTick_contextHandler(void);

#if false
__attribute__((noinline)) void
SysTick_interruptServiceRoutine();
#endif

#if !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)*/
void
SysSeconds_contextHandler(void);

void
OSTimerTicks::implInit(void)
{

  //initialise  exception vector base address
  INTC_init_interrupts();

#if defined(OS_INCLUDE_OSTIMERTICKS_IMPLINIT_TIMERCOUNTER)

  volatile avr32_pm_t* pm = &AVR32_PM;
  volatile avr32_tc_t* tc_reg = &OS_CFGVAR_TIMER;

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
#define COUNTER (OS_CFGLONG_PBA_FREQUENCY_HZ/OS_CFGINT_TIMER_PRESCALLER/DIVIDER/OS_CFGINT_TICK_RATE_HZ)

#if defined(DEBUG)

  OSDeviceDebug::putDec(OS_CFGINT_TIMER_PRESCALLER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(DIVIDER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(COUNTER);
  OSDeviceDebug::putNewLine();

#endif /* defined(DEBUG) */

  // set RC value
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].RC.rc = COUNTER;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].CMR.waveform.wavsel = 2;

  // set interrupt source RC Compare
  tc_reg->channel[OS_CFGINT_TIMER_CHANNEL].IER.cpcs = 1;

#else
  Set_system_register(AVR32_COUNT, 0);
  Set_system_register(AVR32_COMPARE, OS_CFGLONG_CPU_FREQUENCY_HZ
      /OS_CFGINT_TICK_RATE_HZ);
  //register the interrupt
  INTC_register_interrupt(SysTick_contextHandler, COUNT_IRQ_NUM,
      OS_CFGINT_TIMER_IRQ_LEVEL);
#endif /* defined(OS_INCLUDE_OSTIMERTICKS_IMPLINIT_TIMERCOUNTER) */
}


void
SysTick_contextHandler(void)
{
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
}

void
OSTimerTicks::implAcknowledgeInterrupt(void)
{
#if defined(OS_INCLUDE_OSTIMERTICKS_IMPLINIT_TIMERCOUNTER)
  OS_CFGVAR_TIMER.channel[OS_CFGINT_TIMER_CHANNEL].sr;
#else
  Set_system_register(AVR32_COMPARE, OS_CFGLONG_OSCILLATOR_HZ
      /OS_CFGINT_TICK_RATE_HZ);
#endif /* defined(OS_INCLUDE_OSTIMERTICKS_IMPLINIT_TIMERCOUNTER) */
}

#if defined(OS_INCLUDE_OSTIMERSECONDS)

void
OSTimerSeconds::init(void)
  {
#if defined(DEBUG)

  OSDeviceDebug::putString_P(PSTR("OSTimerSeconds::init()"));
  OSDeviceDebug::putNewLine();

#endif

#if defined(OS_INCLUDE_32KHZ_TIMER)

    volatile avr32_rtc_t* rtc_reg = &AVR32_RTC;

    if(rtc_init(rtc_reg, RTC_OSC_32KHZ, 0))
      {
        rtc_set_top_value(rtc_reg, (OS_CFG_RTC_TOP / 2) - 1);
#if defined(DEBUG)
        OSDeviceDebug::putString_P(PSTR("RTC 32KHz"));
        OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
      }
    else
      {
        rtc_init(rtc_reg, RTC_OSC_RC, 0);
        rtc_set_top_value(rtc_reg, ((115000) / 2) - 1);
#if defined(DEBUG)
        OSDeviceDebug::putString_P(PSTR("RTC RC"));
        OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
      }

    rtc_enable_wake_up(rtc_reg);
    rtc_enable(rtc_reg);
    //register the interrupt
    INTC_register_interrupt(SysSeconds_contextHandler, AVR32_RTC_IRQ,
        OS_CFGINT_TIMER_IRQ_LEVEL);
    rtc_enable_interrupt(rtc_reg);
#endif /* defined(OS_INCLUDE_32KHZ_TIMER) */
  }

void
OSTimerSeconds::implAcknowledgeInterrupt(void)
  {
#if defined(OS_INCLUDE_32KHZ_TIMER)
    rtc_clear_interrupt(&AVR32_RTC);
#endif /* defined(OS_INCLUDE_32KHZ_TIMER) */
  }

#if defined(OS_INCLUDE_32KHZ_TIMER)
void
SysSeconds_contextHandler(void)
  {
#if !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)
    OSScheduler::interruptEnter();
#else
    OSScheduler::ISR_ledActiveOn();
#endif /* !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION) */
      {
        // OSDeviceDebug::putChar('~');
        OSScheduler::timerSeconds.interruptServiceRoutine();
      }
#if !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)
    OSScheduler::interruptExit();
#endif /* !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION) */
  }
#endif /* defined(OS_INCLUDE_32KHZ_TIMER) */

#endif /* defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
