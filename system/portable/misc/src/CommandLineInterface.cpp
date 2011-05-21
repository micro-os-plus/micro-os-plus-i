/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_COMMANDLINEINTERFACE)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/CommandLineInterface.h"
#include "portable/misc/include/ASCII.h"

CommandLineInterface::CommandLineInterface(std::istream& cin, std::ostream& cout,
    unsigned char* pLine, unsigned short iSize) :
  m_cin(cin), m_cout(cout)
{
  OSDeviceDebug::putConstructor_P(PSTR("CommandLineInterface"), this);

  m_pLine = pLine;
  m_iSize = iSize;

  m_pPrompt = (unsigned char*) "> ";
}

CommandLineInterface::~CommandLineInterface()
{
  OSDeviceDebug::putDestructor_P(PSTR("CommandLineInterface"), this);
}

OSReturn_t
CommandLineInterface::loop(OSDeviceCharacter& dev, unsigned char* greeting)
{
  std::ostream& cout = m_cout;

  if (greeting != 0)
    cout << std::endl << std::endl << greeting << std::endl;

  for (; dev.isConnected();)
    {
      cout << std::endl << m_pPrompt;
      int c;

      c = readLine();
      if (c == std::traits::eof())
        {
#if defined(DEBUG)
          OSDeviceDebug::putString("disconnected");
          OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

          return OSReturn::OS_DISCONNECTED;
        }
      else if (c == OSReturn::OS_TIMEOUT)
        {
#if defined(DEBUG)
          OSDeviceDebug::putString("timeout");
          OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

          return OSReturn::OS_DISCONNECTED;
        }
      else if (c < 0)
        {
#if defined(DEBUG)
          OSDeviceDebug::putString("error -");
          OSDeviceDebug::putDec((unsigned short) (-c));
          OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

          return c;
        }

      processLine();
    }

  return OSReturn::OS_OK;
}

int
CommandLineInterface::readLine()
{
  unsigned char*  pc;
  int c;

  std::istream& cin = m_cin;
  std::ostream& cout = m_cout;

  for (pc = m_pLine;;)
    {
      cout.flush();
      c = cin.get();
      if (c < 0)
        {
          return c;
        }
      else
        {
          // OSDeviceDebug::putHex((unsigned char)c);

          if ((c == '\r') || (c == '\n'))
            break;

          if (c == '\b')
            {
              if (pc > m_pLine)
                {
                  cout.put(c);
                  cout.put(' ');
                  cout.put(c);
                  --pc;
                }
              else
                {
                  c = ASCII::BELL;
                  cout.put(c);
                }
            }
          else
            {
              if (((unsigned short) (pc - m_pLine)) < (m_iSize - 2))
                *pc++ = c;
              else
                {
                  c = ASCII::BELL;
                  cout.put(c);
                }
            }
          if ((' ' <= c) && (c < 0x7F))
            {
              cout.put(c);
            }
        }
    }
  // *pc++ = ' ';
  *pc = '\0';

  return pc - m_pLine;
}

static const char str_unknown[] = "Cmd?";

void
CommandLineInterface::processLine()
{
  std::ostream& cout = m_cout;
  Parser& parser = m_parser;

  parser.setInput(m_pLine);
  parser.setSeparators((unsigned char*) " ");
  parser.setToken(m_token, sizeof(m_token));

  if (recurse((cliToken_t*) m_pToken) == OSReturn::OS_OK)
    return;

  cout << std::endl << str_unknown;
  //cout << endl << str_help;
}

OSReturn_t
CommandLineInterface::recurse(cliToken_t* p)
{
  Parser& parser = m_parser;

  parser.parseToken();

  if (parser.getTokenLength() == 0)
    return OSReturn::OS_BAD_COMMAND; // no token

  while (p->pToken != 0)
    {
      if (parser.tokenCompare((const unsigned char*) p->pToken) == 0)
        {
          if (p->pDown != 0)
            return recurse(p->pDown);
          else if (p->pMethod != 0)
            return ((pCliClass_t) m_pClass->*(p->pMethod))();
        }
      ++p;
    }
  return OSReturn::OS_BAD_COMMAND;
}

#endif /* defined(OS_INCLUDE_COMMANDLINEINTERFACE) */
