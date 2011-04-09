/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMERTICKS_H_
#define OSTIMERTICKS_H_

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_OSTIMERTICKS_SIZE)
#define OS_CFGINT_OSTIMERTICKS_SIZE 	(OS_CFGINT_TASKS_TABLE_SIZE)
#endif

// the main timer functionality
// all timing intervals of this timer are expressed in scheduler ticks.
class OSTimerTicks : public OSTimer
  {
public:
  // constructor
  OSTimerTicks();

  // transform  the microseconds number into ticks number
  // possible issues: the result is approximative, the higher the tick rate the better the approximation
  // and the result should fit the range of OSTimerTicks_t, otherwise results are erroneous
  inline static OSTimerTicks_t microsToTicks(unsigned short micros) __attribute__((always_inline));

  // called each time a tick expire
  // increment the current ticks number, and call the OSTimer::interruptTick.
  void interruptServiceRoutine(void);

  inline static void interruptContextHandler(void) __attribute__((always_inline));

  // TODO: delete if not needed
  // static OSTimerTicks_t ms_secondTicks;

protected:
  // OSScheduler calls OSTimerTicks::init() from OSScheduler::start()
  friend class OSScheduler;

  // initialise and it is called from OSScheduler::start()
  static void init(void);

  // initialise the low level timer (implementation related)
  static void implInit(void);

  // clear the interrupt flag; used in interruptServiceRoutine (implementation related)
  static void implAcknowledgeInterrupt(void);

private:
  // contain the timeouts( expressed in ticks) for every alarm
  static OSTimerStruct_t m_array[OS_CFGINT_OSTIMERTICKS_SIZE];
  };

// transform  the microseconds number into ticks number
inline OSTimerTicks_t OSTimerTicks::microsToTicks(unsigned short micros)
  {
    return (((micros)+(OS_CFGINT_TICK_RATE_HZ-1))/OS_CFGINT_TICK_RATE_HZ);
  }

inline  void OSTimerTicks::interruptContextHandler(void)
{
  // the current interrupt level disabled in here
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
  // the current interrupt level enabled after this point
}

#endif /*OSTIMERTICKS_H_*/
