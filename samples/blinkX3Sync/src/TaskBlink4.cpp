/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink4.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink4::TaskBlink4(const char *pName, schedTicks_t rate, TaskBlink1& thread) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_thread(thread)
  {
    debug.putConstructor_P(PSTR("TaskBlink4"), this);

    m_rate = rate;
    m_bToggleBusyWait = true;
    m_bTogglePreemptive = false;
  }

/*
 * Thread main code.
 * Step between 4 states and toggle both preeption and another
 * flag to set the type of busy wait performed in associated thread.
 */

void TaskBlink4::threadMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock.enter();
          {
            debug.putString("Thread '");
            debug.putString(getName());
            debug.putString("', ticks=");
            debug.putDec(m_rate);
            debug.putNewLine();
          }
        os.sched.lock.exit();
      }

    // thread endless loop
    for (;;)
      {
        // go to sleep for some ticks
        os.sched.timerTicks.sleep(m_rate);

        if (os.isDebug())
          debug.putChar(m_bToggleBusyWait ? 'B' : 'b');

        // set flag in associated thread
        m_thread.setBusyWait(m_bToggleBusyWait);

        if (m_bToggleBusyWait)
          {
            if (os.isDebug())
              debug.putChar(m_bTogglePreemptive ? 'P' : 'p');

            // toggle preemption mode
            os.sched.setPreemptive(m_bTogglePreemptive);

            m_bTogglePreemptive = !m_bTogglePreemptive;
          }
        m_bToggleBusyWait = !m_bToggleBusyWait;
      }
  }

