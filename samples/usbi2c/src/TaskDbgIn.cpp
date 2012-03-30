/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "TaskDbgIn.h"

#if defined(OS_CONFIG_BOARD_A0739)
#include "SignalIMUX.h"
#endif

TaskDbgIn::TaskDbgIn(const char *pName, OSDeviceCharacter& outDev,
    OSDeviceCharacter& inDev2) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_outDev(outDev), m_inDev2(inDev2)
{
  debug.putConstructor_P(PSTR("TaskDbgIn"), this);
}

void
TaskDbgIn::threadMain(void)
{
  OSDeviceCharacter& outDev = m_outDev;

  OSDeviceCharacter *inDev;

  os.sched.lock.enter();
    {
      if (os.isDebug())
        {
          clog << "TaskDbgIn::threadMain(" << std::showbase << std::hex << this
              << std::endl;
        }

      outDev = m_outDev;

#if defined(OS_CONFIG_BOARD_A0739)
      SignalIMUX::init();

      // TODO: select according to EEPROM value
      if (false)
        {
          inDev = &m_inDev1;
          SignalIMUX::usart();
        }
      else
        {
          inDev = &m_inDev2;
          SignalIMUX::i2c();
        }
#else
      inDev = &m_inDev2;
#endif

      //m_taskCli.resume();
    }
  os.sched.lock.exit();

  // thread endless loop
  for (;;)
    {
      inDev->open();

      for (; inDev->isOpened() && inDev->isConnected();)
        {
          int c;
          int cnt;

#if true
          c = inDev->readBytes(m_buff, sizeof(m_buff), &cnt);
#else
          c = inDev->readByte();
          m_buff[0] = (unsigned char) c;
          cnt = 1;
#endif

          if (c == std::traits::eof())
            {
              if (os.isDebug())
                clog << "disconnected" << std::endl;

              break;
            }
          else if (c == OSReturn::OS_TIMEOUT)
            {
              if (os.isDebug())
                clog << "timeout" << std::endl;

              break;
            }
          else if (c < 0)
            {
              if (os.isDebug())
                clog << "error -" << std::dec << (int) (-c) << std::endl;

              break;
            }

          if (cnt > 0)
            {
              if (outDev.isConnected())
                {
                  outDev.writeBytes(m_buff, cnt);
                  outDev.flush();
                }
              else
                {
                  if (os.isDebug())
                    {
                      for (int i = 0; i < cnt; ++i)
                        clog.put(m_buff[i]);

                    }
                }
            }
        }
      inDev->close();
    }
}
