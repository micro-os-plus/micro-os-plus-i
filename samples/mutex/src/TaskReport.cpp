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
  OSThread(pName, m_stack, sizeof(m_stack))
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
 * The reporting rate is done with sleep(seconds).
 */

void
TaskReport::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', seconds=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // Task endless loop
  for (;;)
    {
      // This test requires the debug device to be present,
      // it makes no sense to run the Release build.
      if (os.isDebug())
        {
          // Block this task until the mutex is acquired
          mutex.acquire();
            {
              // Disable all other tasks to write during the display.
              // Only the interrupts may write something
              os.sched.lock.enter();
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
              os.sched.lock.exit();
            }
          mutex.release();
        }

      // Finally wait several seconds
      os.sched.timerSeconds.sleep(m_rate);
    }
}

