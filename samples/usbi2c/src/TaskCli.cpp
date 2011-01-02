/*
 *	Copyright (C) 2007 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskCli.h"

#include "SignalIMUXEN.h"
#include "SignalIMUX.h"

/*
 * Task constructor. 
 * Initialize system task object, initialize member objects
 * and store parameters in private members.
 *
 */

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev,
    OSDeviceCharacter& devIn) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_dev(dev), m_cin(&m_dev),
      m_cout(&m_dev), m_cli(m_line, sizeof(m_line)), m_devIn(devIn)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    debug.putString("TaskCli()=");
    debug.putHex((unsigned short)this);
    debug.putNewLine();
#endif

    //suspend(); // start in suspended state
  }

// ---------------------------------------------------------------------------

static const char prompt[] = "> ";

/*
 * Task main code. 
 * Open device, output greeting and in a loop read and process lines.
 * I/O is done via standard streams routed to the attached device.
 *
 */

void TaskCli::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            clog << "TaskCli::taskMain("<< showbase << hex
                << ( unsigned short ) this << ") SP="<< hex
                << ( unsigned short ) SP << endl;
          }
        os.sched.unlock();
      }

    OSDeviceCharacter& dev = m_dev;

    istream& cin = m_cin;
    ostream& cout = m_cout;

    SimpleCli & cli = m_cli;

#if defined(OS_CONFIG_BOARD_A0739)
    SignalIMUXEN::init();
#endif

    // task endless loop
    for (;;)
      {
        dev.open();

#if defined(OS_CONFIG_BOARD_A0739)
        SignalIMUXEN::enable();
#endif

        os.sched.lock();
          {
            cout << endl << endl << greeting << endl;

#if defined(OS_CONFIG_BOARD_A0739)
            if (SignalIMUX::isI2C())
            cout << endl << "I2C input" << endl;
            else
            cout << endl << "USART input" << endl;
#endif

          }
        os.sched.unlock();

        for (; dev.isConnected();)
          {
            cout << endl << prompt;

            int c;

c            = cli.readLine(cin, cout);
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
            lineProcess();

          }

#if defined(OS_CONFIG_BOARD_A0739)
        SignalIMUXEN::disable();
#endif
        dev.close();
        m_devIn.close();
        m_devIn.open();
      }
  }

static const char str_help[] = "st";
static const char str_unknown[] = "Cmd?";

/*
 * Parse input line, identify commands and output results.
 * 
 * Accepted commands:
 * 
 * 	st = show tasks
 * 
 */

void TaskCli::lineProcess()
  {
    unsigned char *pc;
    pc = m_line;

    SimpleCli &cli = m_cli;
    ostream& cout = m_cout;

    unsigned char *p;
    unsigned char c;
    //unsigned long l;
    //unsigned char v;
    //unsigned char i;

    //int r;

    // ?? os.WDTreset();

cli    .parseReset(); // reset pointer to start of line

    p = cli.parseNext();
    if (p == 0)
    return;

    c = *p | 0x20;
    if ( *p == '?')
      {
        cout << endl << str_help;
      }
    else if (c == 's')
      {
        ++p;
        c = *p;
        if (c == '\0')
        goto err;

        c |= 0x20;
        if (c == 't')
          {
            int cnt;
            cnt = os.sched.getTasksCount();

            for (int i = 0; i < cnt; ++i)
              {
                OSTask *pt;
                pt = os.sched.getTask(i);

                cout << endl;
                if (pt == this)
                cout << '*';
                else
                cout << ' ';

                cout << *pt; // print task info
              }
          }
        else
        goto err;
      }
    else if (c == 't')
      {
        // test commands
        ++p;
        c = *p;
        if (c == '\0')
        goto err;

        c |= 0x20;
        if (c == '1')
          {
            if (os.isDebug())
            debug.putChar('U');
          }
        else if (c == '2')
          {
            if (os.isDebug())
            clog << "u";
          }
        else
        goto err;
      }
    else
    goto err;

    return;

    err: cout << endl << str_unknown;
    cout << endl << str_help;
  }

