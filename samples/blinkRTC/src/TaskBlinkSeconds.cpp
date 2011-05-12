/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkSeconds.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlinkSeconds::TaskBlinkSeconds(const char *pName, unsigned char iLed, unsigned int rate) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlinkSeconds()=");
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
TaskBlinkSeconds::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec((unsigned short)m_oLed.bitNumber());
          debug.putString(", seconds=");
          debug.putDec((unsigned short)m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // initialise led port as output
  m_oLed.init();

  // task endless loop
  for (;;)
    {
      os.sched.timerSeconds.sleep(m_rate);

      debug.putChar('.');
      // finally toggle led
      m_oLed.toggle();
    }
}

