/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_ACTIVE_STACKSREPORTER)

#include "portable/kernel/include/OS.h"

#include "portable/active/include/StacksReporter.h"

/*
 * Thread constructor.
 * Initialise system thread object and store parameters in private members.
 */

StacksReporter::StacksReporter(const char* pName, schedTicks_t rateSeconds,
    schedTicks_t maxSeconds, uint8_t increaseRate, OSThreadPriority_t priority) :
  OSThread(pName, m_stack, sizeof(m_stack), priority)
{
  debug.putConstructor_P(PSTR("StacksReporter"), this);

  m_rateSeconds = rateSeconds;
  m_maxSeconds = maxSeconds;
  m_increaseRate = increaseRate;
}

/*
 * Thread main code.
 *
 * The toggle rate is done with sleep().
 */

void
StacksReporter::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', seconds=");
          debug.putDec(m_rateSeconds);
          debug.putString(", max=");
          debug.putDec(m_maxSeconds);
          debug.putString(", increase%=");
          debug.putDec((unsigned short) m_increaseRate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  unsigned int i;
  i = 0;

  unsigned int n;
  n = m_rateSeconds;

  // thread endless loop
  for (;;)
    {
      if (os.isDebug())
        {
          // Disable all other threads to write during the display.
          // Only the interrupts may write something
          os.sched.lock.enter();
            {
              if (i > n)
                {
                  if (n < m_maxSeconds)
                    {
                      // if max limit not reached,
                      // increase the limit with given rate
                      n = n * (100 + m_increaseRate) / 100;
                    }
                  i = 0;
                }
              if (i == 0)
                {
                  int nThreads;
                  nThreads = os.sched.getThreadsCount();
                  for (int j = 0; j < nThreads; ++j)
                    {
                      OSThread* pt;
                      pt = os.sched.getThread(j);
                      clog << std::endl;
                      clog << ((pt == this) ? '*' : ' ');
#if false
                      clog << *pt; // print thread info
#else
                      clog << pt->getName() << ' ' << pt->getStackUsed() << '/'
                          << pt->getStackSize();
#endif
                    }
                  clog << std::endl;
                }
              ++i;
            }
          os.sched.lock.exit();
        }

#if defined(OS_INCLUDE_OSTIMERSECONDS)
      os.sched.timerSeconds.sleep(1);
#else
      os.sched.timerTicks.sleep(OS_CFGINT_TICK_RATE_HZ);
#endif
    }
}

#endif /* defined(OS_INCLUDE_ACTIVE_STACKSREPORTER) */
