/*
 * TaskStress.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: alex
 */

#include "TaskStress.h"

// global variables defined in main.cpp
extern OSMutex mutex;
extern int resourceValue;
extern int resourceAccessNum[];

// the random global variable
unsigned int TaskStress::ms_rand;


/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskStress::TaskStress(const char *pName, int taskId) :
  OSTask(pName, m_stack, sizeof(m_stack))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskStress()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
  m_taskId = taskId;
}

/*
 * Task main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskStress::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  int sleepTicks; // sleep interval in OS ticks

  // task endless loop
  for (;;)
    {
      // sleep for a random number of ticks
      sleepTicks = (rand() % (m_maxSleepTicks - m_minSleepTicks)) + m_minSleepTicks;
      os.sched.timerTicks.sleep(sleepTicks);

      // obtain mutex while did not reach the maximum number of access
      //if (resourceAccessNum[m_taskId] > m_maxAccessNum)
      //  continue;

      // block task until the mutex is acquired
      mutex.acquire();
      {
        resourceValue++; //access the resource
        resourceAccessNum[m_taskId]++; // increment the number of access

        os.sched.timerTicks.sleep(rand()%200);

        // check if the task is the current owner of the mutex
        if (this != mutex.getOwnerTask())
          resourceAccessNum[m_taskId] = APP_CONFIG_ERROR_CODE;
      }
      mutex.release();
    }
}

unsigned int
TaskStress::rand(void)
{
  ms_rand = ms_rand * 214013L + 2531011L;
#if (__SIZEOF_INT__ == 2)
  return (ms_rand & 0x7fff);
#else
  return ((ms_rand >> 16) & 0x7fff);
#endif
}
