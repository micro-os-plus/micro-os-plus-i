/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskCli.h"

#include "portable/kernel/include/ostream_OSThread.h"

#include "GpsPosition.h"
#include "Application.h"

// ----- Active object constructor --------------------------------------------

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_dev(dev), m_cin(&m_dev),
      m_cout(&m_dev), m_cli(m_cin, m_cout, m_line, sizeof(m_line))
{
  debug.putConstructor("TaskCli", this);
}

// ----- CLI commands ---------------------------------------------------------

extern const cliToken_t cliCommandsShow[];

const cliToken_t cliCommands[] =
  {
    { "show", (cliToken_t*) cliCommandsShow, 0 },
    { "test", 0, (pCliMethod_t) & TaskCli::commandTest },
    { 0, 0, 0 } };

const cliToken_t cliCommandsShow[] =
  {
    { "threads", 0, (pCliMethod_t) & TaskCli::commandShowThreads },
    { "stacks", 0, (pCliMethod_t) & TaskCli::commandShowStacks },
    { "position", 0, (pCliMethod_t) & TaskCli::commandShowPosition },
    { 0, 0, 0 } };

// ----------------------------------------------------------------------------

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
          clog << "TaskCli::threadMain()" << endl;
        }
      os.sched.lock.exit();
    }

  OSDeviceCharacter& dev = m_dev;
  CommandLineInterface& cli = m_cli;

  // Register the commands to the CLI
  cli.setCommands((cliToken_t*) cliCommands, (pCliClass_t*) this);

  // Thread endless loop
  for (;;)
    {
      dev.open(); // wait for dtr

      // Read and process lines
      cli.loop(dev, (unsigned char*) greeting);

      dev.close();
    }
}

// ----- Commands implementation ----------------------------------------------

// Show threads

OSReturn_t
TaskCli::commandShowThreads(void)
{
  ostream& cout = m_cout;

  int nThreads;
  nThreads = os.sched.getThreadsCount();

  for (int i = 0; i < nThreads; ++i)
    {
      OSThread *pt;
      pt = os.sched.getThread(i);

      cout << endl;
      cout << ((pt == this) ? '*' : ' ');

      // Print the thread info
      cout << *pt;
    }

  return OSReturn::OS_OK;
}

// Show stacks

OSReturn_t
TaskCli::commandShowStacks(void)
{
  ostream& cout = m_cout;

  int nThreads;
  nThreads = os.sched.getThreadsCount();

  for (int i = 0; i < nThreads; ++i)
    {
      OSThread* pt;
      pt = os.sched.getThread(i);

      cout << endl;
      cout << ((pt == this) ? '*' : ' ');

      // Print the stack used versus size ratio
      cout << pt->getName() << ' ' << pt->getStackUsed() << '/'
          << pt->getStackSize();
    }

  return OSReturn::OS_OK;
}

// Show GPS position

OSReturn_t
TaskCli::commandShowPosition(void)
{
  ostream& cout = m_cout;

  GpsPosition pos;

  // Fill in with GPS coordinates
  app.gps.getPosition(&pos);

  cout << endl << pos; // Print the GPS position

  return OSReturn::OS_OK;
}

// Sample command with numerical parameters

OSReturn_t
TaskCli::commandTest(void)
{
  ostream& cout = m_cout;
  CommandLineInterface& cli = m_cli;
  Parser& parser = cli.getParser();

  parser.parseToken();

  if (parser.getTokenLength() == 0)
    {
      cout << endl << "test 1 ¦ test 2";

      return OSReturn::OS_BAD_COMMAND; // no token
    }
  else
    {
      unsigned short n;
      parser.parseUnsigned(&n);

      if (n == 1)
        {
          cout << endl << "one";
        }
      else if (n == 2)
        {
          cout << endl << "two";
        }
      else
        {
          return OSReturn::OS_BAD_COMMAND; // no token
        }
    }

  return OSReturn::OS_OK;
}
