/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSThreadIdle.h"

OSThreadIdle::OSThreadIdle() :
  OSThread("IDLE", m_stack, sizeof(m_stack), OSThread::IDLE_PRIORITY)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSThreadIdle"), this);
}

void
OSThreadIdle::threadMain(void)
{
#if defined(DEBUG)
  // TODO : eventually define and use lockEnter()/lockExit()
  OSCriticalSection::enter();
    {
      OSDeviceDebug::putString_P(PSTR("OSThreadIdle::threadMain()"));
      OSDeviceDebug::putNewLine();
    }
  OSCriticalSection::exit();
#endif

  for (;;)
    {

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
      if (!enterSleep())
        {
          OSScheduler::ledActiveOff(); // turn activity led off
          OSCPU::idle();
        }
#else
      OSScheduler::ledActiveOff(); // turn activity led off
      OSCPU::idle();
#endif
      // required for cooperative and after non-scheduled interrupts
      OSScheduler::yield();
    }
}

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
bool
OSThreadIdle::enterSleep(void)
{
  bool bIsDeepSleepAllowed;

#if defined(OS_INCLUDE_OSCPUDEEPSLEEPCRITICALSECTION)

  bIsDeepSleepAllowed = true;

#else /* !defined(OS_INCLUDE_OSCPUDEEPSLEEPCRITICALSECTION) */

  bIsDeepSleepAllowed = OSScheduler::isDeepSleepAllowed();

#endif /* defined(OS_INCLUDE_OSCPUDEEPSLEEPCRITICALSECTION) */

  for (int i = 0; i < OSScheduler::getThreadsCount(); ++i)
    {
      OSThread* pThread;
      pThread = OSScheduler::getThread(i);
      if (pThread == NULL)
        continue;

      // If any thread requests to prevent deep sleep, return
#if defined(OS_INCLUDE_OSCPUSLEEPCRITICALSECTION)

      if (!pThread->getCpuSleepCriticalSection().isSleepAllowed())
        return false;

#if defined(OS_INCLUDE_OSCPUDEEPSLEEPCRITICALSECTION)

      if (bIsDeepSleepAllowed && !pThread->getCpuDeepSleepCriticalSection().isSleepAllowed())
        bIsDeepSleepAllowed = false;

#endif /* defined(OS_INCLUDE_OSCPUDEEPSLEEPCRITICALSECTION) */

#else

      if (!pThread->isSleepAllowed())
        return false;

#endif
    }

  // If there are threads scheduled on the ticks timer, return
  if (OSScheduler::timerTicks.getCount() > 0)
    return false;

  // Finally enter (deep) sleep
  if (bIsDeepSleepAllowed)
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSTHREADIDLE_ENTERSLEEP)
      OSDeviceDebug::putChar('S');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTHREADIDLE_ENTERSLEEP) */

      OSScheduler::ledActiveOff(); // turn activity led off
      OSCPU::deepSleep();
    }
  else
    {
#if defined(DEBUG) && defined(OS_DEBUG_OSTHREADIDLE_ENTERSLEEP)
      OSDeviceDebug::putChar('s');
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTHREADIDLE_ENTERSLEEP) */

      OSScheduler::ledActiveOff(); // turn activity led off
      OSCPU::sleep();
    }

  return true;
}
#endif

#ifndef OS_INCLUDE_CUSTOM_IDLE_THREAD
OSThreadIdle idleThread;
#endif

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
