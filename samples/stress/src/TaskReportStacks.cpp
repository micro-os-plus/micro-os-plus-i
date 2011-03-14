/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskReportStacks.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskReportStacks::TaskReportStacks(const char* pName, schedTicks_t rateSeconds,
    schedTicks_t maxSeconds, unsigned char increaseRate) :
  OSTask(pName, m_stack, sizeof(m_stack))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskReportStacks()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rateSeconds = rateSeconds;
  m_maxSeconds = maxSeconds;
  m_increaseRate = increaseRate;
}

/*
 * Task main code.
 *
 * The toggle rate is done with sleep().
 */

void
TaskReportStacks::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("'");
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }


  int i;
  i = 0;

  int n;
  n = m_rateSeconds; //APP_CFGINT_DUMP_INTERVAL_SECONDS;

  // task endless loop
  for (;;)
    {
      if (os.isDebug())
        {
          // Disable all other tasks to write during the display.
          // Only the interrupts may write something
          os.sched.lock.enter();
            {
              if (i > n)
                {
                  if (n < m_maxSeconds) // APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS
                    {
                      // if max limit not reached,
                      // increase the limit with given rate
                      n = n * (100 + m_increaseRate) //APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS)
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
                      OSTask* pt;
                      pt = os.sched.getTask(j);
                      clog << endl;
                      clog << ((pt == this) ? '*' : ' ');

                      clog << *pt; // print task info
                    }
                  clog << endl;
                }
              ++i;
            }
          os.sched.lock.exit();
        }

      os.sched.timerSeconds.sleep(1);
    }
}
