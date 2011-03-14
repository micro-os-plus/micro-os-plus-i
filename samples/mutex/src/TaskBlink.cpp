/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlink::TaskBlink(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlink()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rate = rate;
}

/*
 * Task main code.
 * Initialise led and toggle it using the given rate.
 *
 * The toggle rate is done with sleep(ticks).
 */

void
TaskBlink::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Task '");
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

  // Task endless loop
  for (;;)
    {
      // Sleep for a programmable interval, given in ticks
      os.sched.timerTicks.sleep(m_rate);

      // Finally toggle led
      m_oLed.toggle();
    }
}

