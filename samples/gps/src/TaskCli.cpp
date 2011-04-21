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
      m_cout(&m_dev), m_cli(m_cin, m_cout, m_line, sizeof(m_line))
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskCli()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

// ----------------------------------------------------------------------------
extern const cliToken_t l2[];

const cliToken_t l1[] =
  {
    { "show", (cliToken_t*) l2, 0 },
    { 0, 0, 0 } };

const cliToken_t l2[] =
  {
    { "tasks", 0, (pCliMethod_t) & TaskCli::commandShowTasks },
    { "stacks", 0, (pCliMethod_t) & TaskCli::commandShowStacks },
    { "position", 0, (pCliMethod_t) & TaskCli::commandShowPosition },
    { 0, 0, 0 } };

static const char prompt[] = "> ";

// ----------------------------------------------------------------------------

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

  //istream& cin = m_cin;
  ostream& cout = m_cout;

  CommandLineInterface & cli = m_cli;

  cli.setCommands((cliToken_t*) l1, (pCliClass_t*) this);

  // task endless loop
  for (;;)
    {
      dev.open(); // wait for dtr

      cout << endl << endl << greeting << endl;

      for (; dev.isConnected();)
        {
          cout << endl << prompt;
          int c;

          c = cli.readLine();
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

          cli.processLine();
        }
      dev.close();
    }
}

static const char str_help[] = "show tasks | show stacks | show position";
static const char str_unknown[] = "Cmd?";

OSReturn_t
TaskCli::commandShowTasks(void)
{
  ostream& cout = m_cout;

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

  return OSReturn::OS_OK;
}

OSReturn_t
TaskCli::commandShowStacks(void)
{
  ostream& cout = m_cout;

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

  return OSReturn::OS_OK;
}

OSReturn_t
TaskCli::commandShowPosition(void)
{
  ostream& cout = m_cout;

  GpsPosition pos;
  app.gps.getPosition(&pos);

  cout << endl << pos;

  return OSReturn::OS_OK;
}
