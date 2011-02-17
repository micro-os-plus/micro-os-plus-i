/*
 * TaskStress.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: alex
 */

#include "TaskStress.h"

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskStress::TaskStress(const char *pName, unsigned int minMicros, unsigned int maxMicros, unsigned int minTicks, unsigned int maxTicks) :
  OSTask(pName, m_stack, sizeof(m_stack))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskStress()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_minMicros = minMicros;
  m_maxMicros = maxMicros;
  m_minTicks = minTicks;
  m_maxTicks = maxTicks;
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
          debug.putString("', micros=");
          debug.putDec(m_minMicros);
          debug.putString("..");
          debug.putDec(m_maxMicros);
          debug.putString(", ticks=");
          debug.putDec(m_minTicks);
          debug.putString("..");
          debug.putDec(m_maxTicks);
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  int nBusy,nSleep;
  int nCnt;
  nCnt = 0;

  // task endless loop
  for (;;)
    {
      nBusy = (rand() % (m_maxMicros-m_minMicros)) + m_minMicros;
      nSleep = (rand() % (m_maxTicks-m_minTicks)) + m_minTicks;

      os.busyWaitMicros(nBusy);
      os.sched.timerTicks.sleep(nSleep);
      nCnt+=nSleep;
      if(nCnt>= 1000)
        {
          nCnt-=1000;
          debug.putString(getName());
        }

    }
}
