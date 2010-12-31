/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "portable/kernel/include/OS.h"

//#include "portable/kernel/include/OSTimer.h"

class OSTimer;

class Timer
  {
public:
  Timer(OSTimer *);
  virtual ~Timer();

  void start(OSTimerTicks_t ticks, bool periodic=false);
  void stop();
  void cancel(OSEventWaitReturn_t ret=OSEventWaitReturn::OS_CANCELED);

  void eventSet(OSEvent_t event,
      OSEventWaitReturn_t ret=OSEventWaitReturn::OS_VOID);
  OSEventWaitReturn_t eventWait(void);

  bool isPeriodic();
  OSTimerTicks_t getTicksStart();

  OSTimerTicks_t getTicks();

  // callback
  virtual void timerEvent(void);

protected:
  OSEvent_t m_event;
  OSEventWaitReturn_t m_ret;

  OSTimerTicks_t m_ticks;
  bool m_periodic;

  OSTimer *m_pTimer;
  };

inline bool Timer::isPeriodic()
  {
    return m_periodic;
  }

inline OSTimerTicks_t Timer::getTicksStart()
  {
    return m_ticks;
  }

inline OSTimerTicks_t Timer::getTicks()
  {
    return m_pTimer->getTicks();
  }

#endif /*TIMER_H_*/
