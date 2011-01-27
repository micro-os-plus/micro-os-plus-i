/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkBusyWait.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlinkBusyWait::TaskBlinkBusyWait(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlinkBusyWait()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rate = rate;
}

/*
 * Task main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskBlinkBusyWait::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec(m_oLed.bitNumber());
          debug.putString(", millis=");
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
      os.busyWaitMillis(m_rate);
      debug.putChar('.');

      // finally toggle led
      m_oLed.toggle();
    }
}
