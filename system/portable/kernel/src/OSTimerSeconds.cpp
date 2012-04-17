/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#if defined(OS_INCLUDE_OSTIMERSECONDS)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSTimerSeconds.h"

// static members
OSTimerStruct_t volatile OSTimerSeconds::m_array[OS_CFGINT_OSTIMERSECONDS_SIZE];
OSTimerTicks_t volatile OSTimerSeconds::ms_schedulerTicks;
#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
OSTime_t volatile OSTimerSeconds::ms_uptime;
#endif

OSTimerSeconds::OSTimerSeconds() :
    OSTimer((OSTimerStruct_t*) &m_array[0],
        sizeof(m_array) / sizeof(m_array[0]))
{
  OSDeviceDebug::putConstructor_P(PSTR("OSTimerSeconds"), this);

  ms_schedulerTicks = 0;
#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  ms_uptime = 0;
#endif
  ;
}

#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)

void
OSTimerSeconds::getUptime(unsigned long* pSeconds, OSTimerTicks_t* pTicks)
{
  OSCriticalSection::enter();
    {
      *pSeconds = ms_uptime;
      *pTicks = ms_schedulerTicks;
    }
  OSCriticalSection::exit();
}

#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
void OSTimerSeconds::checkVirtualWatchdogs(void)
  {
    for (int i=0; i < OSScheduler::getThreadsCount(); ++i)
      {
        OSThread* pThread;
        pThread = OSScheduler::getThread(i);
        pThread->virtualWatchdogCheck();
      }
  }
#endif

#endif /* OS_INCLUDE_OSSCHEDULER_TIMERSECONDS */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
