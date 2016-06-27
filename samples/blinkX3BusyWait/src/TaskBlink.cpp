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
  debug.putConstructor("TaskBlink", this);

  m_rate = rate;
}

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
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
          debug.putDec((unsigned short)m_oLed.bitNumber());
          debug.putString(", millis=");
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
      delayWithYield(m_rate); // sequence of short waits

      // finally toggle led
      m_oLed.toggle();

      if (os.isDebug())
        {
          debug.putString(getName());
        }
    }
}

/*
 * Perform a series of short (1 millisecond) busy waits  separated
 * by yields.
 */

void
TaskBlink::delayWithYield(schedTicks_t n)
{
  for (; n--;)
    {
      os.busyWaitMillis(1); // busy wait one millisecond
      os.sched.yield(); // release control to next thread
    }
}

// ---------------------------------------------------------------------------

