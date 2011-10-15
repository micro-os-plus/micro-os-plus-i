/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMERRTC_H_
#define OSTIMERRTC_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTIMERRTC)

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_OSTIMERTC_SIZE)
#define OS_CFGINT_OSTIMERTC_SIZE 	(OS_CFGINT_THREADS_ARRAY_SIZE)
#endif

// the second system timer functionality, intended to measure longer intervals
class OSTimerRtc : public OSTimer
{
public:
  // initialise internal structures
  OSTimerRtc();

#if defined(OS_INCLUDE_OSTIMERRTC_UPTIME)
  // increment the current seconds number
  static void incrementUptime(void);
  // return the current seconds number
  static unsigned long getUptime(void);
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  // check for every thread if the software watchdog (counter) expired
  static void checkVirtualWatchdogs(void);
#endif

  // interrupt service routine called each second
  void
  interruptServiceRoutine(void);

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
  static OSTimerStruct_t volatile m_array[OS_CFGINT_OSTIMERTC_SIZE];

#if defined(OS_INCLUDE_OSTIMERRTC_UPTIME)
  // current seconds number
  static OSTime_t volatile ms_uptime;
#endif
};

#if defined(OS_INCLUDE_OSTIMERRTC_UPTIME)

inline void OSTimerRtc::incrementUptime(void)
  {
    ms_uptime++;
  }

inline OSTime_t OSTimerRtc::getUptime(void)
  {
    return ms_uptime;
  }

#endif

inline void
OSTimerRtc::interruptServiceRoutine(void)
{
  implAcknowledgeInterrupt();
  interruptTick();
  incrementTicks();

#if defined(OS_INCLUDE_OSTIMERRTC_UPTIME)
  incrementUptime();
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  checkVirtualWatchdogs();
#endif
}

#endif

#endif /* OSTIMERRTC_H_ */
