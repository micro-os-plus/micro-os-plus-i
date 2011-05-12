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
  OSThread(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
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
          debug.putDec((unsigned short) m_oLed.bitNumber());
          debug.putString(", ticks=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // initialise led port as output
  m_oLed.init();

#if false
  int n1, n2, n3;
  n2 = m_rate;
  os.sched.critical.enter();
    {
      os.sched.timerTicks.sleep(n2);
      os.sched.eventWait((OSEvent_t)&n2);
    }
  os.sched.critical.exit();

#if false
  OSScheduler::criticalEnter();
    {
      os.sched.timerTicks.sleep(n2);
      os.sched.eventWait((OSEvent_t)&n2);
    }
  OSScheduler::criticalExit();
#endif

  OSSchedulerLock::enter();
  {
    os.sched.timerTicks.sleep(n2);
    os.sched.eventWait((OSEvent_t)&n2);
  }
  OSSchedulerLock::exit();

  os.sched.timerTicks.sleep(n2);
  os.sched.eventWait((OSEvent_t)&n2);
#endif

  // task endless loop
  for (;;)
    {
      os.sched.timerTicks.sleep(m_rate);

      debug.putChar(',');

#if OS_TEST_PHASE == 1
      ;
#else
      // finally toggle led
      m_oLed.toggle();
#endif
    }
}

