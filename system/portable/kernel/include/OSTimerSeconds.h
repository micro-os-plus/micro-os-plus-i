/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTIMERSECONDS_H_
#define OSTIMERSECONDS_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTIMERSECONDS)

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_OSTIMERSECONDS_SIZE)
#define OS_CFGINT_OSTIMERSECONDS_SIZE 	(OS_CFGINT_TASKS_TABLE_SIZE)
#endif

class OSTimerSeconds : public OSTimer
  {
public:
  OSTimerSeconds();

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  static void incrementUptime(void);
  static unsigned long getUptime(void);
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  static void checkVirtualWatchdogs(void);
#endif

  //void interruptSchedulerTick(void);
  static OSTimerTicks_t getSchedulerTicks(void);

  void interruptServiceRoutine(void);

  static OSTimerTicks_t ms_schedulerTicks;

protected:
  friend class OSScheduler;
  static void init(void);

private:
  static OSTimerStruct_t m_array[OS_CFGINT_OSTIMERSECONDS_SIZE];

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  static OSTime_t ms_uptime;
#endif
  };

inline OSTimerTicks_t OSTimerSeconds::getSchedulerTicks(void)
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

inline void OSTimerSeconds::interruptServiceRoutine(void)
  {
    interruptTick();
    incrementTicks();

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
    incrementUptime();
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
    checkVirtualWatchdogs();
#endif
  }

#endif

#endif /*OSTIMERSECONDS_H_*/
