/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink4.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlink4::TaskBlink4(const char *pName, schedTicks_t rate, TaskBlink1& task) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_task(task)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskBlink4()=");
    debug.putPtr(this);
    debug.putNewLine();
#endif

    m_rate = rate;
    m_bToggleBusyWait = true;
    m_bTogglePreemptive = false;
  }

/*
 * Task main code.
 * Step between 4 states and toggle both preeption and another
 * flag to set the type of busy wait performed in associated task.
 */

void TaskBlink4::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            debug.putString("Task '");
            debug.putString(getName());
            debug.putString("', ticks=");
            debug.putDec(m_rate);
            debug.putNewLine();
          }
        os.sched.unlock();
      }

    // task endless loop
    for (;;)
      {
        // go to sleep for some ticks
        os.sched.timerTicks.sleep(m_rate);

        if (os.isDebug())
          debug.putChar(m_bToggleBusyWait ? 'B' : 'b');

        // set flag in associated task
        m_task.setBusyWait(m_bToggleBusyWait);

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

