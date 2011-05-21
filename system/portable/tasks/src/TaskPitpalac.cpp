/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_TASKPITPALAC)

#include "portable/kernel/include/OS.h"

#include "portable/tasks/include/TaskPitpalac.h"

/*
 * Thread constructor.
 * Initialise system thread object and store parameters in private members.
 *
 */

TaskPitpalac::TaskPitpalac(const char* pName, schedTicks_t rate) :
  OSThread(pName, m_stack, sizeof(m_stack))
  {
    OSDeviceDebug::putConstructor_P(PSTR("TaskPitpalac"), this);

    m_rate = OS_CFGINT_TICK_RATE_HZ * 2* rate;
  }

/*
 * Thread main code.
 * Initialise led and progressively toggle it using the rate,
 * giving the feeling of a live heart beat.
 *
 * Used as a luxury way to show the device is still alive.
 *
 */

void TaskPitpalac::threadMain(void)
  {

#if defined(DEBUG)
    OSSchedulerLock::enter();
      {
        OSDeviceDebug::putString_P(PSTR("TaskPitpalac::ThreadMain()"));
        OSDeviceDebug::putNewLine();
      }
    OSSchedulerLock::exit();
#endif
    // initialise led port as output
    ledInit();

    uint_t n;
    for (n = 1; n * n < (m_rate / 4); ++n)
      ;
    --n; // compute n = sqrt(m_rate / 4);

    // thread endless loop
    for (;;)
      {
        uint_t i;

        OSScheduler::timerSeconds.sleep(1);

        for (i = 1; i < n; ++i)
          {
            // turn On
            waitAndToggle(n - i, i); // total (n-1) * n ticks
          }

        ledOn(); // stay On
        OSScheduler::timerTicks.sleep(m_rate / 4);
        ledOff();

        for (i = 1; i < n; ++i)
          {
            // turn Off
            waitAndToggle(i, n - i); // total (n-1) * n ticks
          }

        // stay Off

#if defined(DEBUG) && defined(OS_DEBUG_TASKPITPALAC_DOT)
        OSDeviceDebug::putChar('.');
#endif
      }
  }

void TaskPitpalac::waitAndToggle(schedTicks_t tOff, schedTicks_t tOn)
  {
    // go to sleep for some ticks
    OSScheduler::timerTicks.sleep(tOff);
    ledOn();
    OSScheduler::timerTicks.sleep(tOn);
    ledOff();
  }

#endif
