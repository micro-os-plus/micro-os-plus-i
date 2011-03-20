/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
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
 * Acquire the mutex, increment counter and release the mutex.
 *
 * The task activity is simulated with some random sleep().
 */

void
TaskStress::taskMain(void)
{
  if (true && os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', id=");
          debug.putDec((unsigned short) m_taskId);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  os.sched.lock.isSet();

  // The sleep interval, in OS ticks, randomly computed
  schedTicks_t sleepTicks;

  // task endless loop
  for (;;)
    {
      // Compute a random value in the given range
      sleepTicks = (rand() % (m_maxSleepTicks - m_minSleepTicks))
          + m_minSleepTicks;

      // Sleep for a random number of ticks
      os.sched.timerTicks.sleep(sleepTicks);

      // Block task until the mutex is acquired
      mutex.acquire();
        {
          // We have exclusive access to the shared resource

          // Increment the global counter
          resourceValue++;
          // Increment the task specific counter
          resourceAccessNum[m_taskId]++;

          // Simulate some activity done by the task
          os.sched.timerTicks.sleep(rand() % m_maxActivityTicks);

          // Check if the task is the current owner of the mutex
          if (this != mutex.getOwnerTask())
            resourceAccessNum[m_taskId] = APP_CONFIG_ERROR_CODE;
        }
      mutex.release();
    }
}

unsigned int
TaskStress::rand(void)
{
  unsigned int ret;

#if defined(APP_CFGINT_TASKSTRESS_RAND_CONST)
  ret = APP_CFGINT_TASKSTRESS_RAND_CONST;
#else
  os.sched.lock.enter();
    {
      ms_rand = ms_rand * 214013L + 2531011L;
#if (__SIZEOF_INT__ == 2)
      ret = (ms_rand & 0x7fff);
#else
      ret = ((ms_rand >> 16) & 0x7fff);
#endif
    }
  os.sched.lock.exit();

  if (os.isDebug() && false)
    {
      os.sched.lock.enter();
        {
          debug.putString("rand()=");
          debug.putDec((unsigned short) ret);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }
#endif /* defined(APP_CFGINT_TASKSTRESS_RAND_CONST) */

  return ret;
}
