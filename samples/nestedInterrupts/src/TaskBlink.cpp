/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink.h"

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
          debug.putDec(m_oLed.bitNumber());
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
      os.sched.timerTicks.sleep(m_rate);

      // finally toggle led
      m_oLed.toggle();
    }
}

void
busyWaitMicros(unsigned int n)
{
  int i;
  // calibrate from OSC
  for (i = (OS_CFGLONG_SYSCLOCK_HZ / 1000 * n / OS_CFGINT_BUSYWAIT_CALIBRATION); i--;)
    {
      OSImpl::NOP();
    }
}

// artificially make the system timer interrupt longer, so that we
// increase the chance of having nested interrupts.

#define APP_INCLUDE_BUSY_WAIT           1
#define APP_BUSY_PROCENTAGE             30
#define APP_EVENT_DUMMY                 0x1111
#define APP_CFGINT_NOTIFIES             100
void
TaskBlink::schedulerTick(void)
{
#if defined(APP_INCLUDE_BUSY_WAIT)
  busyWaitMicros(1000000/OS_CFGINT_TICK_RATE_HZ*APP_BUSY_PROCENTAGE/100);
#else
  for (int i = APP_CFGINT_NOTIFIES; --i;)
    os.sched.eventNotify(APP_EVENT_DUMMY);
#endif
}
