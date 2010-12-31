/*
 *	Copyright (C) 2007 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

/*
 * Task constructor.
 * Initialize system task object and store parameters in private members.
 */

TaskBlink::TaskBlink(const char *pName, unsigned char iLed, schedTicks_t iRate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskBlink()=");
    debug.putPtr(this);
    debug.putNewLine();
#endif

    m_iRate = iRate;
  }

/*
 * Task main code.
 * Initialize led and toggle it using the rate.
 */

void TaskBlink::taskMain(void)
  {
    if (os.isDebug())
      {
        // lock scheduler to ensure line is atomic.
        os.sched.lock();
          {
            // display task parameters.
            debug.putString("Task '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_iRate);
            debug.putNewLine();
          }
        os.sched.unlock();
      }

    // initialize led port as output
    m_oLed.init();

    // task endless loop
    for (;;)
      {
        // go to sleep for some ticks
        os.sched.timerTicks.sleep(m_iRate);

        m_oLed.toggle();
      }
  }

