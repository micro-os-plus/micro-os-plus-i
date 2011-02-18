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

// the parent class of all custom timer classes.
class Timer
  {
public:
  // constructor
  Timer(OSTimer *);

  // destructor
  virtual ~Timer();

  // start the timer to trigger an event after the given number of ticks
  // if periodic is true, the event will periodically be triggered
  void start(OSTimerTicks_t ticks, bool periodic=false);

  // stop a periodic timer after the pending event is completed,
  // i.e. do not reschedule another event
  // the number of ticks will be set to zero and periodic to false
  void stop();

  // cancel the pending event, by notifying it with the given return value
  void cancel(OSEventWaitReturn_t ret=OSEventWaitReturn::OS_CANCELED);

  // set the event to be triggered by the timer and the return value
  void eventSet(OSEvent_t event,
      OSEventWaitReturn_t ret=OSEventWaitReturn::OS_VOID);

  // wait for the trigger event to occur.
  OSEventWaitReturn_t eventWait(void);

  // return true if the timer is set to periodic mode
  bool isPeriodic();

  // return the number of ticks set by start()
  // if the timer is stopped, the returned value is zero
  OSTimerTicks_t getTicksStart();

  // return the current ticks counter of the associated timer
  OSTimerTicks_t getTicks();

  // this is the callback invoked by the timer interrupt
  // the default action is notify the event programmed by eventSet()
  virtual void timerEvent(void);

protected:
  // record the event set by eventSet
  OSEvent_t m_event;
  // record the return value of the event set by eventSet
  OSEventWaitReturn_t m_ret;

  // record the number of ticks for the current alarm
  OSTimerTicks_t m_ticks;

  // record if the alarm is periodic or not
  bool m_periodic;

  // record the pointer to the OSTimer used
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
