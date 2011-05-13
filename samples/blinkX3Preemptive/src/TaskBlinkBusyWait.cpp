/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkBusyWait.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlinkBusyWait::TaskBlinkBusyWait(const char *pName, unsigned int iLed, unsigned int rate) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
  debug.putString_P(PSTR("TaskBlinkBusyWait"), this);

  m_rate = rate;
}

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskBlinkBusyWait::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec((unsigned short)m_oLed.bitNumber());
          debug.putString(", millis=");
          debug.putDec((unsigned short)m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // initialise led port as output
  m_oLed.init();

  // thread endless loop
  for (;;)
    {
      os.busyWaitMillis(m_rate);
      debug.putChar('.');

      // finally toggle led
      m_oLed.toggle();
    }
}

