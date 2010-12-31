/*
 *	Copyright (C) 2007 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlink3.h"

/*
 * Task constructor.
 * Initialize system task object and store parameters in private members.
 */

TaskBlink3::TaskBlink3(const char *pName, unsigned char iLed) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskBlink3()=");
    debug.putHex((unsigned short)this);
    debug.putNewLine();
#endif
  }

/*
 * Task main code.
 * Initialize led and toggle it based on events notified by task 2.
 */

void TaskBlink3::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            debug.putString("Task '");
            debug.putString(getName());
            debug.putString("', led=");
            debug.putDec(m_oLed.bitNumber());
            debug.putNewLine();
          }
        os.sched.unlock();
      }

    // initialize led port as output
    m_oLed.init();

    // task endless loop
    for (;;)
      {
        // wait for the custom event triggered by another task.
        // task is sleeping...
        os.sched.eventWait(EVENT_CUSTOM);

        // toggle led
        m_oLed.toggle();
      }
  }

