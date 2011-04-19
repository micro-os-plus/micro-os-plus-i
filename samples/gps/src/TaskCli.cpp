/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskCli.h"

#include "portable/kernel/include/ostream_OSTask.h"

#include "GpsPosition.h"
#include "Application.h"

// Task constructor.

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_dev(dev), m_cin(&m_dev),
      m_cout(&m_dev), m_cli(m_line, sizeof(m_line))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskCli()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

// ----------------------------------------------------------------------------

static const char prompt[] = "> ";

/*
 * Task main code. 
 * Open device, output greeting and in a loop read and process lines.
 * I/O is done via standard streams routed to the attached device.
 *
 */

void
TaskCli::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          clog << "TaskCli::taskMain()" << endl;
        }
      os.sched.lock.exit();
    }

  OSDeviceCharacter& dev = m_dev;

  istream& cin = m_cin;
  ostream& cout = m_cout;

  SimpleCli & cli = m_cli;

  // task endless loop
  for (;;)
    {
      dev.open(); // wait for dtr

      cout << endl << endl << greeting << endl;

      for (; dev.isConnected();)
        {
          cout << endl << prompt;
          int c;

          c = cli.readLine(cin, cout);
          if (c == traits::eof())
            {
              if (os.isDebug)
                clog << "disconnected" << endl;

              break;
            }
          else if (c == OSReturn::OS_TIMEOUT)
            {
              if (os.isDebug)
                clog << "timeout" << endl;

              break;
            }
          else if (c < 0)
            {
              if (os.isDebug)
                clog << "error -" << dec << (int) (-c) << endl;

              break;
            }

          processLine();
        }
      dev.close();
    }
}

static const char str_help[] = "show tasks | show stacks | show position";
static const char str_unknown[] = "Cmd?";

/*
 * Parse input line, identify commands and output results.
 * 
 * Accepted commands:
 * 
 *      show tasks
 *      show stacks
 *      show position
 */

void
TaskCli::processLine()
{
  ostream& cout = m_cout;
  Parser& parser = m_parser;

  parser.setInput(m_line);
  parser.setSeparators((unsigned char*) " ");
  parser.setToken(m_token, sizeof(m_token));

  parser.parseToken();

  if (parser.getTokenLength() == 0)
    goto err; // no token, empty line

  if (parser.tokenCompare((const unsigned char*) "show") == 0)
    {
      parser.parseToken();

      if (parser.getTokenLength() == 0)
        goto err;
      // no subcommand

      if (parser.tokenCompare((const unsigned char*) "tasks") == 0)
        {
          int nTasks;
          nTasks = os.sched.getTasksCount();

          for (int i = 0; i < nTasks; ++i)
            {
              OSTask *pt;
              pt = os.sched.getTask(i);

              cout << endl;
              cout << ((pt == this) ? '*' : ' ');
              cout << *pt; // print task info
            }
        }
      else if (parser.tokenCompare((const unsigned char*) "stacks") == 0)
        {
          int nTasks;
          nTasks = os.sched.getTasksCount();
          for (int i = 0; i < nTasks; ++i)
            {
              OSTask* pt;
              pt = os.sched.getTask(i);

              cout << endl;
              cout << ((pt == this) ? '*' : ' ');
              cout << pt->getName() << ' ' << pt->getStackUsed() << '/'
                  << pt->getStackSize();
            }
        }
      else if (parser.tokenCompare((const unsigned char*) "position") == 0)
        {
          GpsPosition pos;
          app.gps.getPosition(&pos);

          cout << endl << pos;
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

