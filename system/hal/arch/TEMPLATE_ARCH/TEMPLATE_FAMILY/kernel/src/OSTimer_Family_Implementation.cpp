/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_TEMPLATE)

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#include "portable/kernel/include/OS.h"

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
;
}


void
SysTick_contextHandler(void)
{
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
  OSScheduler::interruptEnter();
#else
  OSScheduler::ISR_ledActiveOn();
#endif /*!defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)*/
    {
      OSScheduler::timerTicks.interruptServiceRoutine();
    }
#if !defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)*/
}

void
OSTimerTicks::implAcknowledgeInterrupt(void)
{
  ;
}

#if defined(OS_INCLUDE_OSTIMERSECONDS)

void
OSTimerSeconds::init(void)
  {
#if defined(OS_INCLUDE_32KHZ_TIMER)

;

#endif /*defined(OS_INCLUDE_32KHZ_TIMER)*/
  }

void
OSTimerSeconds::implAcknowledgeInterrupt(void)
  {
#if defined(OS_INCLUDE_32KHZ_TIMER)
;
#endif /*defined(OS_INCLUDE_32KHZ_TIMER)*/
  }

#if defined(OS_INCLUDE_32KHZ_TIMER)
void
SysSeconds_contextHandler(void)
  {
#if !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)
    OSScheduler::interruptEnter();
#else
    OSScheduler::ISR_ledActiveOn();
#endif /*!defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION) */
      {
        OSScheduler::timerSeconds.interruptServiceRoutine();
      }
#if !defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION)
    OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_OSTIMERSECONDS_ISR_PREEMPTION) */
  }
#endif /*defined(OS_INCLUDE_32KHZ_TIMER)*/

#endif /*defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)*/

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */

#endif /* defined(OS_CONFIG_FAMILY_TEMPLATE) */

