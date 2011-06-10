/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskTest.h"

#if !defined OS_INCLUDE_SIMPLECLI

#include "portable/kernel/include/ostream_OSThread.h"

/*
 * Active object constructor. 
 * Initialise parent system thread, initialise member objects
 * and store parameters in private members.
 *
 */

TaskTest::TaskTest(const char *pName, OSDeviceCharacter& dev) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_dev(dev)
{
  debug.putConstructor_P(PSTR("TaskTest"), this);
}

// ----------------------------------------------------------------------------

/*
 * Thread main code. 
 *
 */

void
TaskTest::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
#if false
          clog << "TaskTest::threadMain(" << showbase << hex << this <<
              ") SP="<< hex
              << ( unsigned short ) SP <<
              endl;
#else
          debug.putString("TaskTest::threadMain()");
          debug.putNewLine();
#endif
        }
      os.sched.lock.exit();
    }

  OSDeviceCharacter& dev = m_dev;

  // thread endless loop
  for (;;)
    {
      dev.open(); // wait for dtr

      for (; dev.isConnected();)
        {
          dev.readBytes(m_loopBuff, 512, &m_countTest);

          if(m_countTest != 0)
            {
              dev.writeBytes(m_loopBuff, m_countTest);
              dev.flush();
            }
        }
      dev.close();
    }
}

#endif  // OS_INCLUDE_SIMPLECLI
