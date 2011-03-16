/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSTaskIdle.h"

OSTaskIdle::OSTaskIdle() :
  OSTask("IDLE", m_stack, sizeof(m_stack), OSTask::IDLE_PRIORITY)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OSTaskIdle()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

void OSTaskIdle::taskMain(void)
  {
#if defined(DEBUG)
    // TODO : eventually define and use lockEnter()/lockExit()
    OSCriticalSection::enter();
      {
        OSDeviceDebug::putString_P(PSTR("OSTaskIdle::taskMain()"));
        OSDeviceDebug::putNewLine();
      }
    OSCriticalSection::exit();
#endif

    for (;;)
      {

#if defined(OS_INCLUDE_OSTASK_SLEEP)
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

#if defined(OS_INCLUDE_OSTASK_SLEEP)
bool OSTaskIdle::enterSleep(void)
  {
    for (int i = 0; i < OSScheduler::getTasksCount(); ++i)
      {
        // if any task requests to prevent deep sleep, return
        if (!OSScheduler::getTask(i)->isAllowDeepSleep())
        return false;
      }

    // if there are tasks scheduled on the ticks timer, return
    if (OSScheduler::timerTicks.getCount()> 0)
    return false;

    // finally enter (deep) sleep
    if (OSScheduler::isAllowDeepSleep())
      {
        OSScheduler::ledActiveOff(); // turn activity led off
        OSCPU::deepSleep();
      }
    else
      {
        OSScheduler::ledActiveOff(); // turn activity led off
        OSCPU::sleep();
      }

    return true;
  }
#endif

#ifndef OS_INCLUDE_CUSTOM_IDLE_TASK
OSTaskIdle idleTask;
#endif

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
