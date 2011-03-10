/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

// global variables defined in main.cpp
extern OSMutex mutex;
extern int resourceValue;
extern int resourceAccessNum[];

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
          debug.putDec((unsigned short)m_oLed.bitNumber());
          debug.putString(", ticks=");
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

      if (os.isDebug())
        {
          // block this task until the mutex is acquired
          mutex.acquire();
          // Disable all other tasks to write during the display.
          // Only the interrupts may write something
          os.sched.lock();
            {
              //int nTasks;
              //nTasks = os.sched.getTasksCount();
              int sum;
              sum = 0;
              clog << endl;
              clog << resourceValue;
              clog << " : [ ";
              for (int j = 0; j < APP_CONFIG_TASKS_NUM_MAX; ++j)
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
          mutex.release();
        }

      os.sched.timerTicks.sleep(m_rate);

      //debug.putChar(',');

#if OS_TEST_PHASE == 1
      ;
#else
      // finally toggle led
      m_oLed.toggle();
#endif

    }
}

