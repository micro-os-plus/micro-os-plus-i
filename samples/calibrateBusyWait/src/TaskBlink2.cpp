/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink2.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlink2::TaskBlink2(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskBlink2()=");
    debug.putPtr(this);
    debug.putNewLine();
#endif

    m_rate = rate;
    m_bToggle = true;
  }

/*
 * Task main code.
 * Initialise led and toggle it using the rate.
 */

void TaskBlink2::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            debug.putString("Task '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_rate);
            debug.putNewLine();
          }
        os.sched.unlock();
      }

    // initialise led port as output
    m_oLed.init();

    // task endless loop
    for (;;)
      {
        delayWithYield(m_rate); // sequence of short waits

        // toggle led
        m_oLed.toggle();
      }
  }

