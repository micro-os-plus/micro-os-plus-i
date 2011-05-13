/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink3.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink3::TaskBlink3(const char *pName, unsigned char iLed) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
    debug.putString_P(PSTR("TaskBlink3"), this);
  }

/*
 * Thread main code.
 * Initialise led and toggle it based on events notified by thread 2.
 */

void TaskBlink3::threadMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock.enter();
          {
            debug.putString("Thread '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putNewLine();
          }
        os.sched.lock.exit();
      }

    // initialise led port as output
    m_oLed.init();

    // thread endless loop
    for (;;)
      {
        // wait for the custom event triggered by another thread.
        // thread is sleeping...
        os.sched.eventWait(EVENT_CUSTOM);

        // toggle led
        m_oLed.toggle();
      }
  }

