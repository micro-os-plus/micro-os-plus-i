/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_TASKBLINK)

#include "portable/tasks/include/TaskBlink.h"

/*
 * Thread constructor.
 * Initialise system thread object and store parameters in private members.
 *
 */

TaskBlink::TaskBlink(const char* pName, schedTicks_t rate) :

  OSThread(pName, m_stack, sizeof(m_stack)), m_timerSeconds((OSTimer*)
      &OSScheduler::timerSeconds)
  {
    OSDeviceDebug::putConstructor_P(PSTR("TaskBlink"), this);

    m_rate = rate;
  }

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 *
 * Used mainly to show the device is stil alive.
 *
 */

void TaskBlink::threadMain(void)
  {
#if defined(DEBUG)
    OSSchedulerLock::enter();
      {
        OSDeviceDebug::putString("TaskBlink::ThreadMain()");
        OSDeviceDebug::putNewLine();
      }
    OSSchedulerLock::exit();
#endif
    // initialise led port as output, turn led off
    ledInit();
    ledOff();

    // for didactic purposes,
    // use a periodic timer instead of sleep()
    m_timerSeconds.eventSet((OSEvent_t) this); // set event on this thread
    m_timerSeconds.start(m_rate, true); // start periodic timer

    // thread endless loop
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
