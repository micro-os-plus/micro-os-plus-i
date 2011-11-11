/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#if defined(OS_INCLUDE_OSTIMERRTC)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSTimerRtc.h"


OSTimerRtc::OSTimerRtc() :
  OSTimer((OSTimerStruct_t*) &m_array[0], sizeof(m_array) / sizeof(m_array[0]))
{
  OSDeviceDebug::putConstructor_P(PSTR("OSTimerRtc"), this);

#if defined(OS_INCLUDE_OSTIMERRTC_UPTIME)
  ms_uptime = 0;
#endif
  ;
}

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
void OSTimerRtc::checkVirtualWatchdogs(void)
  {
    for (int i=0; i < OSScheduler::getThreadsCount(); ++i)
      {
        OSThread* pThread;
        pThread = OSScheduler::getThread(i);
        pThread->virtualWatchdogCheck();
      }
  }
#endif

#endif /* OS_INCLUDE_OSTIMERRTC */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
