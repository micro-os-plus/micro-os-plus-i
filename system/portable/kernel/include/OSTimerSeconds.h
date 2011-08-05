/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMERSECONDS_H_
#define OSTIMERSECONDS_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTIMERSECONDS)

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_OSTIMERSECONDS_SIZE)
#define OS_CFGINT_OSTIMERSECONDS_SIZE 	(OS_CFGINT_THREADS_ARRAY_SIZE)
#endif

// the second system timer functionality, intended to measure longer intervals
class OSTimerSeconds : public OSTimer
{
public:
  // initialise internal structures
  OSTimerSeconds();

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  // increment the current seconds number
  static void incrementUptime(void);
  // return the current seconds number
  static unsigned long getUptime(void);
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  // check for every thread if the software watchdog (counter) expired
  static void checkVirtualWatchdogs(void);
#endif

  // return the current number of ticks within the current second
  static OSTimerTicks_t
  getSchedulerTicks(void);

  // interrupt service routine called each second
  void
  interruptServiceRoutine(void);

  // the number of OS ticks within current second
  static OSTimerTicks_t ms_schedulerTicks;

protected:
  // OSScheduler calls OSTimerSeconds::initialise() from OSScheduler::start()
  friend class OSScheduler;

  // initialise and it is called from OSScheduler::start()
  static void
  init(void);
  static void
  implAcknowledgeInterrupt(void);

private:
  // contain the timeouts(expressed in ticks) for every alarm
  static OSTimerStruct_t m_array[OS_CFGINT_OSTIMERSECONDS_SIZE];

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  // current seconds number
  static OSTime_t ms_uptime;
#endif
};

inline OSTimerTicks_t
OSTimerSeconds::getSchedulerTicks(void)
{
  return ms_schedulerTicks;
}

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)

inline void OSTimerSeconds::incrementUptime(void)
  {
    ms_uptime++;
  }

inline OSTime_t OSTimerSeconds::getUptime(void)
  {
    return ms_uptime;
  }

#endif

inline void
OSTimerSeconds::interruptServiceRoutine(void)
{
  implAcknowledgeInterrupt();
  interruptTick();
  incrementTicks();

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  incrementUptime();
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  checkVirtualWatchdogs();
#endif
}

#endif

#endif /* OSTIMERSECONDS_H_ */
