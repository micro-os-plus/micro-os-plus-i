/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskReport.h"

// global variables defined in main.cpp
extern OSMutex mutex;
extern int resourceValue;
extern int resourceAccessNum[];

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskReport::TaskReport(const char *pName, schedTicks_t seconds) :
  OSTask(pName, m_stack, sizeof(m_stack))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskReport()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rate = seconds;
}

/*
 * Task main code.
 * Report mutex usage counters.
 *
 * The reporting rate is done with sleep().
 */

void
TaskReport::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', seconds=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  // task endless loop
  for (;;)
    {
      if (os.isDebug())
        {
          // block this task until the mutex is acquired
          mutex.acquire();
            {
              // Disable all other tasks to write during the display.
              // Only the interrupts may write something
              os.sched.lock();
                {
                  int sum;
                  sum = 0;
                  clog << endl;
                  clog << resourceValue;
                  clog << " : [ ";
                  for (int j = 0; j < APP_CONFIG_RESOURCES_ARRAY_SIZE; ++j)
                    {
                      sum += resourceAccessNum[j];
                      clog << resourceAccessNum[j];
                      clog << ' ';
                    }
                  clog << "] sum=";
                  clog << sum;
                  clog << endl;
                }
              os.sched.unlock();
            }
          mutex.release();
        }

      // finally wait several seconds
      os.sched.timerSeconds.sleep(m_rate);
    }
}
