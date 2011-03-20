/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskCli.h"

#include "portable/kernel/include/ostream_OSTask.h"

/*
 * Task constructor. 
 * Initialise system task object, initialise member objects
 * and store parameters in private members.
 *
 */

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_dev(dev),
#if true
  m_cin(&m_dev), m_cout(&m_dev),
#endif
  m_cli(m_line, sizeof(m_line))
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
#if false
          clog << "TaskCli::taskMain(" << showbase << hex << this <<
              ") SP="<< hex
              << ( unsigned short ) SP <<
              endl;
#else
          debug.putString("TaskCli::taskMain()");
          debug.putNewLine();
#endif
        }
      os.sched.lock.exit();
    }

  OSDeviceCharacter& dev = m_dev;

#if true
  istream& cin = m_cin;
  ostream& cout = m_cout;

  SimpleCli & cli = m_cli;
#endif

  // task endless loop
  for (;;)
    {
      dev.open(); // wait for dtr
#if true
      cout << endl << endl << greeting << endl;
#else
      dev.writeByte('\n');
      dev.writeByte('\r');
      for (const char* p = greeting; *p; ++p)
        dev.writeByte(*p);

      dev.writeByte('\n');
      dev.writeByte('\r');

      dev.flush();
#endif
      for (; dev.isConnected();)
        {
#if true
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
          lineProcess();
#else
          int c;
          c = dev.readByte();
          dev.writeByte(c);
          dev.flush();
          if (c == 0x03)
            break; //CtrlC should quit
#endif
        }
      dev.close();
    }
}

#if true
static const char str_checksum[] = "Checksum?";

/*
 * For cases when commands are sent by a device it is preferable to use
 * a method to validate the correctness of the command.
 * 
 * For this we addopted the following syntax:
 * 
 * 	$abcde*XX 
 * 
 * where XX is the hex value of the xor-ed 'abcde' string, starting with 00
 * 
 * To help human operators compute the checksum of some commands we also 
 * accept the form:
 * 
 * 	$abcde
 * 
 * situation when the command is not executed but only the checksum is 
 * computed and displayed.
 *
 */

int
TaskCli::xorCheck(unsigned char *pc)
{
  ostream & cout = m_cout;

  unsigned char c;
  unsigned char xor1, xor2;
  unsigned char *q;
  unsigned char *qq;

  *pc = ' ';

  for (xor1 = 0, q = pc + 1; (c = *q) != '\0'; ++q)
    {
      if (c == '*')
        break;

      xor1 ^= c;
    }

  if (c == '\0')
    {
      // no xor received, return computed xor
      cout << endl << ("*");
      cout.width(2);
      cout << hex << (unsigned short) xor1;
      cout.flush();
      return -2;
    }

  qq = q;

  xor2 = 0;
  q++;
  c = *q;
  if (c == '\0')
    {
      // syntax error
      goto err;
    }

  if ('0' <= c && c <= '9')
    xor2 = c - '0';
  else if ('A' <= c && c <= 'F')
    xor2 = c - 'A' + 10;
  else
    {
      // syntax error
      goto err;
    }
  q++;
  c = *q;
  if (c == '\0')
    {
      // syntax error
      goto err;
    }

  xor2 <<= 4;

  if ('0' <= c && c <= '9')
    xor2 |= c - '0';
  else if ('A' <= c && c <= 'F')
    xor2 |= c - 'A' + 10;
  else
    {
      // syntax error
      goto err;
    }

  q++;
  c = *q;
  if (c != '\0')
    {
      // syntax error
      goto err;
    }

  if (xor1 != xor2)
    {
      // crc error
      cout << endl << str_checksum;
      return -2; // return 'crc error'
    }
  //*qq++ = ' ';
  *qq = '\0';

  return 0; // return OK

  err: return -1; // return 'syntax error'
}

static const char str_help[] = "ri|re|rd|rc | st";
static const char str_unknown[] = "Cmd?";

/*
 * Parse input line, identify commands and output results.
 * 
 * Accepted commands:
 * 
 * 	ss = show stack
 * 	st = show tasks
 * 
 */

void
TaskCli::lineProcess()
{
  unsigned char * pc;
  pc = m_line;

  SimpleCli & cli = m_cli;
  ostream& cout = m_cout;

  unsigned char *p;
  unsigned char c;
  unsigned long l;
  unsigned char v;
  unsigned char i;

  int r;

//  os.WDTreset();

  if (*pc == '$') // is command using checksum?
    {
      r = xorCheck(pc);
      if (r == -2)
        return;
      else if (r == -1)
        goto err;
    }

  cli .parseReset(); // reset pointer to start of line

  p = cli.parseNext();
  if (p == 0)
    return;

  c = *p | 0x20;
  if (*p == '?')
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
  else if (c == 'r')
    {
      // flight recorder test commands
      ++p;
      c = *p;
      if (c == '\0')
        goto err;

      c |= 0x20;
      ++p;
      if (*p != '\0')
        goto err;

      if (c == 'i')
        {
          if ((p = cli.parseNext()) != 0)
            goto err;

          cout << endl << "uid=00C80A0C";
          cout << endl << "nmax=0001E9FF";
          cout << endl << "nbeg=00005F7D";
        }
      else if (c == 'e')
        {
          if ((p = cli.parseNext()) == 0)
            goto err;

          cout << endl;
        }
      else if (c == 'd' || c == 'c' || c == 'o')
        {
          if ((p = cli.parseNext()) == 0)
            goto err;

          if (cli.parseHex(p, &l) < 0)
            goto err;

          if ((p = cli.parseNext()) != 0 && cli.parseHexNibble(p, &i) == 0)
            {
              v = i;
            }
          else
            {
              v = 0;
            }

          cout << endl
              << "$B06F590E0001CF68638921FA30000028438A21F33000827111FA3801AEFF3200827122FA387FFF883100827112FA387FFFFF3200827121FA3801BA863100827101FB3801E0863120537211FA3C1EB6537311FA3C1CF2537411F93C1DB0537511FA3C0054FF00FF00*CD";
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
#endif

