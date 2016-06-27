/*
 *      Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "TaskDbgIn.h"

#if defined(OS_CONFIG_BOARD_A0739)
#include "SignalIMUX.h"
#endif

TaskDbgIn::TaskDbgIn(const char *pName, OSDeviceCharacter& outDev,
    OSDeviceCharacter& inDev2, std::ostream& cout) :
    OSThread(pName, m_stack, sizeof(m_stack)), m_outDev(outDev), m_inDev2(
        inDev2), m_cout(cout)
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

    }
  os.sched.lock.exit();

  // A delay, to allow the CLI task to initialise the port

  os.sched.timerTicks.sleep(1000);

  // thread endless loop
  for (;;)
    {
      inDev->open();

      bool isNewLineFound;
      isNewLineFound = false;

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
#if true
              debug.putChar('|');
              debug.putDec((unsigned short)cnt);
#endif

#if true
              int first;
              first = 0;

              int last;
              for (; first < cnt; first = last + 1)
                {
                  if (isNewLineFound)
                    {
                      unsigned long seconds;
                      OSTimerTicks_t ticks;
                      os.sched.timerSeconds.getUptime(&seconds, &ticks);

                      unsigned long minutes;
                      minutes = seconds / 60;

                      seconds %= 60;

#if true
                      os.sched.lock.enter();
                        {
                          m_cout.width(2);
                          m_cout << std::dec << minutes << ':';
                          m_cout.width(2);
                          m_cout << std::dec << seconds << '.';
                          m_cout.width(3);
                          m_cout << std::dec << ticks << ": ";
                         }
                      os.sched.lock.exit();
#endif
                      isNewLineFound = false;
                    }

                  last = cnt - 1;
                  int i;
                  for (i = first; i < cnt; ++i)
                    {
                      if (m_buff[i] == '\n')
                        {
                          isNewLineFound = true;
                          last = i;
                          break;
                        }
                    }
                  if (outDev.isConnected() && outDev.isOpened())
                    {
                      outDev.writeBytes(&m_buff[first], last - first + 1);
                      outDev.flush();
                    }
                  else
                    {
                      if (os.isDebug())
                        {
                          for (int j = first; j <= last; ++j)
                            clog.put(m_buff[j]);
                        }
                    }
                }
              //outDev.writeByte('|');

#else
              outDev.writeBytes(&m_buff[0], cnt);
              outDev.flush();
#endif
            }
        }
      inDev->close();
    }
}
