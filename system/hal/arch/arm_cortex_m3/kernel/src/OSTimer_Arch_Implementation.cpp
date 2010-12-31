/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_SDI12SENSOR)
#include "portable/devices/sdi12/include/SDI12Sensor.h"
#endif

#ifndef OS_CFGINT_INTERRUPT_PRIORITY_SYSTICK
#define OS_CFGINT_INTERRUPT_PRIORITY_SYSTICK 253
#endif

static int i;

void OSTimerTicks::init(void)
  {
#if defined(DEBUG)

    OSDeviceDebug::putString("OSTimerTicks::init()");
    OSDeviceDebug::putNewLine();

#endif
    SCB->SHP[15 - 4] = OS_CFGINT_INTERRUPT_PRIORITY_SYSTICK;

    /* Configure SysTick to interrupt at the requested rate. */
    SysTick->LOAD = (OS_CFGLONG_SYSCLOCK_HZ / OS_CFGINT_TICK_RATE_HZ) - 1UL;
    // core clock, count down to 0, enable multi-shot
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT
        | SysTick_CTRL_ENABLE;

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

#endif /* defined(OS_CONFIG_ARCH_ARM_CORTEX_M3) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
