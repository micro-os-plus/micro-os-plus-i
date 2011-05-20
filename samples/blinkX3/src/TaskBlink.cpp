/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink::TaskBlink(const char *pName, unsigned char iLed, schedTicks_t iRate) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
    debug.putConstructor_P(PSTR("TaskBlink"), this);

    m_iRate = iRate;
  }

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 */

void TaskBlink::threadMain(void)
  {
    if (os.isDebug())
      {
        // lock scheduler to ensure line is atomic.
        os.sched.lock.enter();
          {
            // display thread parameters.
            debug.putString("Thread '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_iRate);
            debug.putNewLine();
          }
        os.sched.lock.exit();
      }

    // initialise led port as output
    m_oLed.init();

    // thread endless loop
    for (;;)
      {
        // go to sleep for some ticks
        os.sched.timerTicks.sleep(m_iRate);

        m_oLed.toggle();
      }
  }

