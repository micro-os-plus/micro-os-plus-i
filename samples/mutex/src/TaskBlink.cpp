/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink::TaskBlink(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
  debug.putConstructor_P(PSTR("TaskBlink"), this);

  m_rate = rate;
}

/*
 * Thread main code.
 * Initialise led and toggle it using the given rate.
 *
 * The toggle rate is done with sleep(ticks).
 */

void
TaskBlink::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec((unsigned short) m_oLed.bitNumber());
          debug.putString(", ticks=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // Initialise led port as output
  m_oLed.init();

  // Thread endless loop
  for (;;)
    {
      // Sleep for a programmable interval, given in ticks
      os.sched.timerTicks.sleep(m_rate);

      // Finally toggle led
      m_oLed.toggle();
    }
}

