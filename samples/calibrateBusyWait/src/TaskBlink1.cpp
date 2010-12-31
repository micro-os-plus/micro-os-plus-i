/*
 *	Copyright (C) 2007 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink1.h"

/*
 * Task constructor.
 * Initialize system task object and store parameters in private members.
 */

TaskBlink1::TaskBlink1(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskBlink1()=");
    debug.putHex((unsigned short)this);
    debug.putNewLine();
#endif

    m_rate = rate;
    m_bBusyWait = false;
  }

/*
 * Task main code.
 * Initialize led and toggle it using the rate.
 *
 * The toggle rate is done either with busy wait, or with real sleep.
 * Busy wait is of two kinds, if preemption is enabled a long loop is
 * entered, if preemption is disabled the loop is interrupted by yields.
 */

void TaskBlink1::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            debug.putString("Task '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_rate);
            debug.putNewLine();
          }
        os.sched.unlock();
      }

    // initialize led port as output
    m_oLed.init();

    // task endless loop
    for (;;)
      {
        // task sleep using operating system support
        os.sched.timerTicks.sleep(m_rate);

        // finally toggle led
        m_oLed.toggle();
      }
  }

// method to set the flag used to decide if we do busy wait

void TaskBlink1::setBusyWait(bool bFlag)
  {
    m_bBusyWait = bFlag;
  }

