/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMERTICKS_H_
#define OSTIMERTICKS_H_

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_OSTIMERTICKS_SIZE)
#define OS_CFGINT_OSTIMERTICKS_SIZE 	(OS_CFGINT_TASKS_TABLE_SIZE)
#endif

class OSTimerTicks : public OSTimer
  {
public:
  OSTimerTicks();

  inline static OSTimerTicks_t microsToTicks(unsigned short micros) __attribute__( ( always_inline ) );

  void interruptServiceRoutine(void);
  static OSTimerTicks_t ms_secondTicks;

protected:
  friend class OSScheduler;
  static void init(void);

  static void implInit(void);
  static void implAcknowledgeInterrupt(void);

private:
  static OSTimerStruct_t m_array[OS_CFGINT_OSTIMERTICKS_SIZE];
  };


inline OSTimerTicks_t OSTimerTicks::microsToTicks(unsigned short micros)
  {
    return (((micros)+(OS_CFGINT_TICK_RATE_HZ-1))/OS_CFGINT_TICK_RATE_HZ);
  }

#endif /*OSTIMERTICKS_H_*/
