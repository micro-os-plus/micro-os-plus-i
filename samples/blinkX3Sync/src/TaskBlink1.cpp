/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink1.h"

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

TaskBlink1::TaskBlink1(const char *pName, unsigned char iLed, schedTicks_t rate) :
  OSThread(pName, m_stack, sizeof(m_stack )), m_oLed(iLed)
  {
    debug.putString_P(PSTR("TaskBlink1"), this);

    m_rate = rate;
    m_bBusyWait = false;
  }

/*
 * Thread main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done either with busy wait, or with real sleep.
 * Busy wait is of two kinds, if preemption is enabled a long loop is
 * entered, if preemption is disabled the loop is interrupted by yields.
 */

void TaskBlink1::threadMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock.enter();
          {
            debug.putString("Thread '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putString(", ticks=");
            debug.putDec(m_rate);
            debug.putNewLine();
          }
        os.sched.lock.exit();
      }

    // initialise led port as output
    m_oLed.init();

    // thread endless loop
    for (;;)
      {
        if (m_bBusyWait)
          {
            // busy wait case, to simulate a computing intensive thread
            if (os.sched.isPreemptive())
              os.busyWaitMillis(m_rate); // long wait
            else
              delayWithYield(m_rate); // sequence of short waits
          }
        else
          {
            // thread sleep using operating system support
            os.sched.timerTicks.sleep(m_rate);
          }

        // finally toggle led
        m_oLed.toggle();
      }
  }

// method to set the flag used to decide if we do busy wait

void TaskBlink1::setBusyWait(bool bFlag)
  {
    m_bBusyWait = bFlag;
  }

