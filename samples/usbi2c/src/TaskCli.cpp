/*
 *	Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskCli.h"

#if defined(OS_CONFIG_BOARD_A0739)
#include "SignalIMUXEN.h"
#include "SignalIMUX.h"
#endif

/*
 * Active object constructor. 
 * Initialise parent system thread, initialise member objects
 * and store parameters in private members.
 *
 */

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev,
    OSDeviceCharacter& devIn, std::istream& cin,
    std::ostream& cout) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_dev(dev), m_cin(cin), m_cout(
      cout), m_cli(m_line, sizeof(m_line)), m_devIn(devIn)
{
  debug.putConstructor_P(PSTR("TaskCli"), this);

  //suspend(); // start in suspended state
}

// ---------------------------------------------------------------------------

static const char prompt[] = "> ";

/*
 * Thread main code. 
 * Open device, output greeting and in a loop read and process lines.
 * I/O is done via standard streams routed to the attached device.
 *
 */

void
TaskCli::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          clog << "TaskCli::threadMain(" << std::showbase << std::hex << this << ") SP="
              << std::hex << (unsigned short) SP << std::endl;
        }
      os.sched.lock.exit();
    }

  OSDeviceCharacter& dev = m_dev;

  std::istream& cin = m_cin;
  std::ostream& cout = m_cout;

  SimpleCli & cli = m_cli;

#if defined(OS_CONFIG_BOARD_A0739)
  SignalIMUXEN::init();
#endif

  // thread endless loop
  for (;;)
    {
      dev.open();

#if defined(OS_CONFIG_BOARD_A0739)
      SignalIMUXEN::enable();
#endif

      os.sched.lock.enter();
        {
          cout << std::endl << std::endl << greeting << std::endl;

#if defined(OS_CONFIG_BOARD_A0739)
          if (SignalIMUX::isI2C())
          cout << endl << "I2C input" << endl;
          else
          cout << endl << "USART input" << endl;
#endif

        }
      os.sched.lock.exit();

      for (; dev.isConnected();)
        {
          cout << std::endl << prompt;

          int c;

          c = cli.readLine(cin, cout);
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
 * 	st = show threads
 * 
 */

void
TaskCli::lineProcess()
{
  unsigned char *pc;
  pc = m_line;

  SimpleCli &cli = m_cli;
  std::ostream& cout = m_cout;

  unsigned char *p;
  unsigned char c;
  //unsigned long l;
  //unsigned char v;
  //unsigned char i;

  //int r;

  // ?? os.WDTreset();

  cli.parseReset(); // reset pointer to start of line

  p = cli.parseNext();
  if (p == 0)
    return;

  c = *p | 0x20;
  if (*p == '?')
    {
      cout << std::endl << str_help;
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
          cnt = os.sched.getThreadsCount();

          for (int i = 0; i < cnt; ++i)
            {
              OSThread *pt;
              pt = os.sched.getThread(i);

              cout << std::endl;
              if (pt == this)
                cout << '*';
              else
                cout << ' ';

              cout << *pt; // print thread info
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

  err: cout << std::endl << str_unknown;
  cout << std::endl << str_help;
}

