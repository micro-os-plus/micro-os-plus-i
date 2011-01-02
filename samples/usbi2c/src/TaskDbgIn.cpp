/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "TaskDbgIn.h"

#include "SignalIMUX.h"

TaskDbgIn::TaskDbgIn(const char *pName, OSDeviceCharacter& outDev,
    OSDeviceCharacter& inDev1, OSDeviceCharacter& inDev2) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_outDev(outDev), m_inDev1(inDev1),
      m_inDev2(inDev2)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskDbgIn()=");
    debug.putHex((unsigned short)this);
    debug.putNewLine();
#endif
  }

void TaskDbgIn::taskMain(void)
  {
    OSDeviceCharacter& outDev = m_outDev;

    OSDeviceCharacter *inDev;

    os.sched.lock();
      {
        if (os.isDebug() )
          {
            clog << "TaskDbgIn::taskMain("<< showbase << hex
                << ( unsigned short ) this << endl;
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
    os.sched.unlock();

    // task endless loop
    for (;;)
      {
        inDev->open();

        for (; inDev->isOpened() && inDev->isConnected();)
          {
            int c;

            c = inDev->readByte();
            if (c == traits::eof())
              {
                if (os.isDebug)
                  clog << "disconnected"<< endl;

                break;
              }
            else if (c == OSReturn::OS_TIMEOUT)
              {
                if (os.isDebug)
                  clog << "timeout"<< endl;

                break;
              }
            else if (c < 0)
              {
                if (os.isDebug)
                  clog << "error -"<< dec << (int)(-c) << endl;

                break;
              }

            if (outDev.isConnected())
              {
                outDev.writeByte((unsigned char)c);
                outDev.flush();
              }
            else
              {
                if (os.isDebug())
                  clog.put((unsigned char)c);
              }
          }
        inDev->close();
      }

  }
