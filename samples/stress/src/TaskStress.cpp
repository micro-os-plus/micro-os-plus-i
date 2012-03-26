/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "TaskStress.h"

uint32_t TaskStress::ms_rand;

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskStress::TaskStress(const char *pName, uint16_t minMicros,
    uint16_t maxMicros, uint16_t minTicks, uint16_t maxTicks) :
  OSThread(pName, m_stack, sizeof(m_stack))
{
  debug.putConstructor_P(PSTR("TaskStress"), this);

  m_minMicros = minMicros;
  m_maxMicros = maxMicros;
  m_minTicks = minTicks;
  m_maxTicks = maxTicks;
}

/*
 * Thread main code.
 * Simulate a typical activity, a busy wait followed by a sleep.
 */

void
TaskStress::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
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
      os.sched.lock.exit();
    }

  uint16_t nBusy, nSleep;
  uint16_t nCnt;
  nCnt = 0;

  // thread endless loop
  for (;;)
    {
      nBusy = (rand() % (m_maxMicros - m_minMicros)) + m_minMicros;
      nSleep = (rand() % (m_maxTicks - m_minTicks)) + m_minTicks;

      os.busyWaitMicros(nBusy);
      os.sched.timerTicks.sleep(nSleep);

      // The critical sections are not needed, but are here just to test
      // if embedded critical sections work properly.
      os.sched.critical.enter();
        {
          nCnt += nSleep;
          if (nCnt >= 1000)
            {
              os.sched.critical.enter();
                {
                  nCnt -= 1000;
                }
              os.sched.critical.exit();

              debug.putString(getName());
            }
        }
      os.sched.critical.exit();
    }
}

uint16_t
TaskStress::rand(void)
{
#if false
  return 9;
#else
  uint16_t ret;
  os.sched.lock.enter();
  //os.sched.critical.enter();
    {
      ms_rand = ms_rand * 214013L + 2531011L;
#if (__SIZEOF_INT__ == 2) && false
      ret = (ms_rand & 0x7fff);
#else
      ret = ((ms_rand >> 16) & 0x7fff);
#endif
    }
  os.sched.lock.exit();
  //os.sched.critical.exit();

  return ret;
#endif
}
