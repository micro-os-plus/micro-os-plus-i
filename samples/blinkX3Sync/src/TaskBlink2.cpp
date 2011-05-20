/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink2.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink2::TaskBlink2(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSThread(pName, m_stack, sizeof(m_stack )), m_oLed(iLed)
  {
    debug.putConstructor_P(PSTR("TaskBlink2"), this);

    m_rate = rate;
    m_bToggle = true;
  }

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 */

void TaskBlink2::threadMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock.enter();
          {
            debug.putString("Thread '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_rate);
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
        os.sched.timerTicks.sleep(m_rate);

        // toggle led
        m_oLed.toggle();

        m_bToggle = !m_bToggle;

        // every second time notify the custom event;
        // threads sleeping on it will wakeup
        if (m_bToggle)
          os.sched.eventNotify(EVENT_CUSTOM);
      }
  }

