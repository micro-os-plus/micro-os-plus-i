/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "portable/kernel/include/OS.h"

typedef unsigned long OSTime_t;

class Timer;

// structure of an alarm slot
typedef struct timerStruct
  {
  OSTimerTicks_t ticks;
  OSEvent_t event;
  union
    {
    OSEventWaitReturn_t ret;
    Timer *pTimer;
    } u;
  } OSTimerStruct_t;

/*
 * Support class, used to store timer specific data.
 */
class OSTimer
  {
public:
  // constructor
  OSTimer(OSTimerStruct_t* pArray, int size);

  // sleep for the number of timer ticks
  OSEventWaitReturn_t sleep(OSTimerTicks_t ticks, OSEvent_t event = 0);

  // schedule the timer to notify the event after the number of ticks
  // if event is OSEvent::OS_CUSTOM_TIMER, the ret value is the address
  // of the custom timer 
  void eventNotify(OSTimerTicks_t ticks, OSEvent_t event,
      OSEventWaitReturn_t ret);

  // remove the scheduled timer notification
  int eventRemove(OSEvent_t event);

  // return the current number of tick
  OSTimerTicks_t getTicks(void) const;

  // return the number of jobs scheduled
  int getCount(void) const;

  // used in interrupt routines
  void interruptTick(void);
  void incrementTicks(void);

protected:
  // insert a new timeout into the array
  bool insert(OSTimerTicks_t ticks, OSEvent_t event, OSEventWaitReturn_t ret);

  // remove the timeout from the given slot
  void remove(int i);

  // the array of timeouts
  OSTimerStruct_t* m_pArray;

  // the size of m_pArray
  int m_size;
  // the number of slots used from m_pArray
  int m_count;

private:
  // the current number of ticks
  OSTimerTicks_t ms_ticks;
  };

inline OSTimerTicks_t OSTimer::getTicks(void) const
  {
    return ms_ticks;
  }

inline void OSTimer::incrementTicks(void)
  {
    ms_ticks++;
  }

inline int OSTimer::getCount(void) const
  {
    return m_count;
  }

#endif /* OSTIMER_H_ */
