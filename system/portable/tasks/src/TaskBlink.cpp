/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_TASKBLINK)

#include "portable/tasks/include/TaskBlink.h"

/*
 * Task constructor.
 * Initialize system task object and store parameters in private members.
 *
 */

TaskBlink::TaskBlink(const char *pName, schedTicks_t rate) :

  OSTask(pName, m_stack, sizeof(m_stack)), m_timerSeconds((OSTimer*)
      &OSScheduler::timerSeconds)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSScheduler::lock();
      {
        OSDeviceDebug::putString("TaskBlink()=");
        OSDeviceDebug::putPtr(this);
        OSDeviceDebug::putNewLine();
      }
    OSScheduler::unlock();
#endif

    m_rate = rate;

  }

/*
 * Task main code.
 * Initialize led and toggle it using the rate.
 *
 * Used mainly to show the device is stil alive.
 *
 */

void TaskBlink::taskMain(void)
  {
#if defined(DEBUG)
    OSScheduler::lock();
      {
        OSDeviceDebug::putString("TaskBlink::TaskMain()");
        OSDeviceDebug::putNewLine();
      }
    OSScheduler::unlock();
#endif
    // initialize led port as output, turn led off
    ledInit();
    ledOff();

    // for didactic purposes,
    // use a periodic timer instead of sleep()
    m_timerSeconds.eventSet((OSEvent_t) this); // set event on this task
    m_timerSeconds.start(m_rate, true); // start periodic timer

    // task endless loop
    for (;;)
      {
        m_timerSeconds.eventWait(); // sleep(m_rate);

        ledToggle();

#if defined(DEBUG) && defined(OS_DEBUG_TASKBLINK_DOT)
        OSDeviceDebug::putChar('.');
#endif
      }
  }

#endif
