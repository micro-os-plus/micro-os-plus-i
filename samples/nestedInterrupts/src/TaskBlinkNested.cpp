/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkNested.h"

#define EVENT_NESTEED 0x1234
void
nestedInterruptInit(void);

void
nestedInterruptAck(void);

void
NestedInterrupt_contextHandler(void);

void
NestedInterrupt_interruptServiceRoutine();

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlinkNested::TaskBlinkNested(const char *pName, unsigned char iLed,
    schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlinkNested()=");
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
TaskBlinkNested::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec(m_oLed.bitNumber());
          debug.putString(", divider=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  // initialise led port as output
  m_oLed.init();

  nestedInterruptInit();

  int i;
  i = m_rate;
  // task endless loop
  for (;;)
    {
      os.sched.eventWait(EVENT_NESTEED);
      if (--i == 0)
        {
          debug.putChar('.');

          // finally toggle led
          m_oLed.toggle();

          i = m_rate;
        }
    }
}

void
nestedInterruptInit(void)
{
  ; // TODO: initialise the second timer on a nested interrupt
}

void
nestedInterruptAck(void)
{
  ; // TODO: add code
}

#if defined(OS_EXCLUDE_PREEMPTION)
__attribute__((interrupt))
#else
__attribute__((naked))
#endif
void
NestedInterrupt_contextHandler(void)
{
  OSScheduler::interruptEnter();
    {
      NestedInterrupt_interruptServiceRoutine();
    }
  OSScheduler::interruptExit();
}

__attribute__ ((noinline)) void
NestedInterrupt_interruptServiceRoutine(void)
{
  OSScheduler::eventNotify(EVENT_NESTEED);

  nestedInterruptAck();
}
