/*
 * TaskStress.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: alex
 */

#include "TaskStress.h"

unsigned int TaskStress::ms_rand;

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskStress::TaskStress(const char *pName, unsigned int minMicros,
    unsigned int maxMicros, unsigned int minTicks, unsigned int maxTicks) :
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
 * Simulate a typical activity, a busy wait followed by a sleep.
 */

void
TaskStress::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', micros=");
          debug.putDec((unsigned short) m_minMicros);
          debug.putString("..");
          debug.putDec((unsigned short) m_maxMicros);
          debug.putString(", ticks=");
          debug.putDec((unsigned short) m_minTicks);
          debug.putString("..");
          debug.putDec((unsigned short) m_maxTicks);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  int nBusy, nSleep;
  int nCnt;
  nCnt = 0;

  // task endless loop
  for (;;)
    {
      nBusy = (rand() % (m_maxMicros - m_minMicros)) + m_minMicros;
      nSleep = (rand() % (m_maxTicks - m_minTicks)) + m_minTicks;

      os.busyWaitMicros(nBusy);
      os.sched.timerTicks.sleep(nSleep);
      nCnt += nSleep;
      if (nCnt >= 1000)
        {
          nCnt -= 1000;
          debug.putString(getName());
        }

    }
}

unsigned int
TaskStress::rand(void)
{
  unsigned int ret;
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
  return ret;
}
