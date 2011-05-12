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
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlink()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

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
          debug.putString(", ticks=");
          debug.putDec((unsigned short)m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // initialise led port as output
  m_oLed.init();

#if defined(APP_INCLUDE_DUMP_TASKS)
  int i;
  i = 0;

  int n;
  n = APP_CFGINT_DUMP_INTERVAL_SECONDS;
#endif

  // thread endless loop
  for (;;)
    {

#if defined(APP_INCLUDE_DUMP_TASKS)

      if (os.isDebug())
        {
          // Disable all other threads to write during the display.
          // Only the interrupts may write something
          os.sched.lock.enter();
            {
              if (i > n)
                {
                  if (n < APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS)
                    {
                      // if max limit not reached,
                      // increase the limit with given rate
                      n = n * (100 + APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS)
                          / 100;
                    }
                  i = 0;
                }
              if (i == 0)
                {
                  int nThreads;
                  nThreads = os.sched.getThreadsCount();
                  for (int j = 0; j < nThreads; ++j)
                    {
                      OSThread *pt;
                      pt = os.sched.getThread(j);
                      clog << endl;
                      clog << ((pt == this) ? '*' : ' ');

                      clog << *pt; // print thread info
                    }
                  clog << endl;
                }
              ++i;
            }
          os.sched.lock.exit();
        }
#endif

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

