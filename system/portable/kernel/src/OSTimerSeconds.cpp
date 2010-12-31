/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTIMERSECONDS)

#include "portable/kernel/include/OSTimerSeconds.h"

// static members
OSTimerStruct_t OSTimerSeconds::m_array[OS_CFGINT_OSTIMERSECONDS_SIZE];
OSTimerTicks_t OSTimerSeconds::ms_schedulerTicks;
#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
OSTime_t OSTimerSeconds::ms_uptime;
#endif

OSTimerSeconds::OSTimerSeconds() :
OSTimer(&m_array[0], sizeof(m_array)/sizeof(m_array[0]))
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OSTimerSeconds()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    ms_schedulerTicks = 0;
#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
    ms_uptime = 0;
#endif
    ;
  }

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
void OSTimerSeconds::checkVirtualWatchdogs(void)
  {
    for (int i=0; i < OSScheduler::getTasksCount(); ++i)
      {
        OSTask *pTask;
        pTask = OSScheduler::getTask(i);
        pTask->virtualWatchdogCheck();
      }
  }
#endif

#endif /* OS_INCLUDE_OSSCHEDULER_TIMERSECONDS */

