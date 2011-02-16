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
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskBlink::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec(m_oLed.bitNumber());
          debug.putString(", ticks=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  // initialise led port as output
  m_oLed.init();

#if defined(APP_INCLUDE_DUMP_TASKS)
  int i;
  i = 0;

  int n;
  n = APP_CFGINT_DUMP_INTERVAL_SECONDS;
#endif

  // task endless loop
  for (;;)
    {

#if defined(APP_INCLUDE_DUMP_TASKS)

      if (os.isDebug())
        {
          // Disable all other tasks to write during the display.
          // Only the interrupts may write something
          os.sched.lock();
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
                  int nTasks;
                  nTasks = os.sched.getTasksCount();
                  for (int j = 0; j < nTasks; ++j)
                    {
                      OSTask *pt;
                      pt = os.sched.getTask(j);
                      clog << endl;
                      clog << ((pt == this) ? '*' : ' ');

                      clog << *pt; // print task info
                    }
                  clog << endl;
                }
              ++i;
            }
          os.sched.unlock();
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

