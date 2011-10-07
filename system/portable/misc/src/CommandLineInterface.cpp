/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_COMMANDLINEINTERFACE)

#include "portable/kernel/include/OS.h"

#include <string.h>
#include <ctype.h>

#include "portable/misc/include/CommandLineInterface.h"
#include "portable/misc/include/ASCII.h"

CommandLineInterface::CommandLineInterface(std::istream& cin,
    std::ostream& cout, unsigned char* pHistory,
    unsigned short iHistorySizeBytes) :
  m_cin(cin), m_cout(cout)
{
  OSDeviceDebug::putConstructor_P(PSTR("CommandLineInterface"), this);

  m_pHistoryCurrentPosition = m_pHistory = pHistory;
  m_iHistorySizeBytes = iHistorySizeBytes;

  memset(pHistory, '\0', iHistorySizeBytes);

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

  for (; dev.isConnected() && dev.isOpened();)
    {
      int c;

      // One new line is printed before the prompt
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

      // one more new line before processing the line
      cout << std::endl;
      processLine();
    }

  return OSReturn::OS_OK;
}

int
CommandLineInterface::readLine()
{
  std::istream& cin = m_cin;
  std::ostream& cout = m_cout;

  cout << std::endl << m_pPrompt;

  int nFree;
  nFree = m_pHistory + m_iHistorySizeBytes - m_pHistoryCurrentPosition;
  if (nFree < 40)
    {
      // No enough space left, start line from the beginning
      memset(m_pHistoryCurrentPosition + 1, 0, nFree - 1);

#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
      OSDeviceDebug::putString(" bufbeg ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

      // Start from the beginning
      m_pHistoryCurrentPosition = m_pHistory;
    }
  else
    {
      // Skip over last line terminator
      m_pHistoryCurrentPosition++;
    }

  // Remember the line beginning
  m_pHistoryLineBeginning = m_pHistoryCurrentPosition;

  uchar_t* pNavigate;
  pNavigate = m_pHistoryLineBeginning;

  // Mark start of text
  *m_pHistoryCurrentPosition++ = ASCII::STX;

  // Empty line
  *m_pHistoryCurrentPosition = '\0';

  int lineLength;
  lineLength = 1; // account for STX

  bool isInVerticalNavigation;
  isInVerticalNavigation = false;

  int c;

  for (;;)
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

          if (c == ASCII::ESC)
            {
              c = cin.get();
              if (c == '[')
                {
                  c = cin.get();
                  if (c == 'A')
                    {
#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                      //OSDeviceDebug::putString(" up ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                      if (pNavigate <= m_pHistory)
                        {
                          // Move to the end
                          pNavigate = m_pHistory + m_iHistorySizeBytes;
                        }

                      for (; *--pNavigate != ASCII::STX;)
                        ;

                      goto vNav;
                    }
                  else if (c == 'B')
                    {
#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                      //OSDeviceDebug::putString(" down ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                      for (; *++pNavigate != ASCII::STX;)
                        if (pNavigate >= m_pHistory + m_iHistorySizeBytes)
                          pNavigate = m_pHistory;

                      vNav: if (pNavigate == m_pHistoryLineBeginning)
                        {
                          // We reached the same line, i.e. end of buffer
                          c = ASCII::BELL;
                          cout.put(c);

                          continue;
                        }

#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                      // In pNavigate we have the previous line
                      OSDeviceDebug::putPtr(pNavigate);
                      OSDeviceDebug::putChar(' ');
                      OSDeviceDebug::putString((const char*) (pNavigate + 1));
                      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                      cout.put('\r');
                      cout.put(ASCII::ESC);
                      cout << "[2K" << m_pPrompt << pNavigate;

                      isInVerticalNavigation = true;
                    }
                  else if (c == 'C')
                    {
#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                      OSDeviceDebug::putString(" right ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                      isInVerticalNavigation = false;
                    }
                  else if (c == 'D')
                    {
#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                      OSDeviceDebug::putString(" left ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                      isInVerticalNavigation = false;
                    }
                  else
                    {
                      c = ASCII::BELL;
                      cout.put(c);
                    }
                }
              else
                {
                  c = ASCII::BELL;
                  cout.put(c);
                }
              c = '\0';

              if (!isInVerticalNavigation)
                lineLength = updateCurrentFromHistory(pNavigate);
            }
          else
            {
              if (isInVerticalNavigation)
                {
                  lineLength = updateCurrentFromHistory(pNavigate);
                  isInVerticalNavigation = false;
                }

              if ((c == '\r') || (c == '\n'))
                {
                  break;
                }

              // Check Backspace or DEL
              if (c == '\b' || c == ASCII::DEL)
                {
                  if (lineLength > 1)
                    {
                      // Delete previous character
                      cout.put('\b');
                      cout.put(' ');
                      cout.put('\b');

                      *--m_pHistoryCurrentPosition = '\0';
                      --lineLength;
                    }
                  else
                    {
                      c = ASCII::BELL;
                      cout.put(c);
                    }
                }
              else if (c == '\t')
                {
                  OSDeviceDebug::putString(" tab ");

                  c = ASCII::BELL;
                  cout.put(c);

                  c = '\0';
                }
              else
                {
                  // If buffer not full, store the character
                  if (m_pHistoryCurrentPosition < (m_pHistory
                      + m_iHistorySizeBytes - 2))
                    {
                      *m_pHistoryCurrentPosition++ = c;
                      // -2 is to leave space for the terminator

                      // Update current line length
                      lineLength++;
                    }
                  else
                    {
                      // Is the line starting at the beginning of the buffer?
                      if (m_pHistoryLineBeginning == m_pHistory)
                        {
                          // Buffer really full
                          c = ASCII::BELL;
                          cout.put(c);
                        }
                      else
                        {
                          // Move line to the beginning, including the terminator
                          memmove(m_pHistory, m_pHistoryLineBeginning,
                              lineLength + 1);

#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                          OSDeviceDebug::putString(" bufbeg2 ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

                          // Overlapping?
                          if (m_pHistory + lineLength + 1
                              >= m_pHistoryLineBeginning)
                            {
                              // yes, clear to the end of the buffer
                              memset(m_pHistory + lineLength + 1, '\0',
                                  m_iHistorySizeBytes - (lineLength + 1));

#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
                              OSDeviceDebug::putString(" clr ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */
                            }
                          else
                            {
                              // non overlapping, clear current marker
                              // to avoid matching this in history
                              *m_pHistoryLineBeginning = '\0';
                            }

                          m_pHistoryLineBeginning = m_pHistory;
                          m_pHistoryCurrentPosition = m_pHistoryLineBeginning
                              + lineLength;

                          // Finally store the character
                          *m_pHistoryCurrentPosition++ = c;

                          // Update current line length
                          lineLength++;
                        }
                    }
                  // Add the terminator
                  *m_pHistoryCurrentPosition = '\0';
                }
            }
          if (isprint(c))
            {
              cout.put(c);
            }
        }

    }

  return lineLength;
}

size_t
CommandLineInterface::updateCurrentFromHistory(uchar_t* pCurrent)
{
  size_t len;

#if defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY)
  OSDeviceDebug::putString(" use hist ");
#endif /* defined(OS_DEBUG_COMMANDLINEINTERFACE_READLINE_HISTORY) */

  // Compute length of the desired command, excluding terminator
  len = strlen((const char*) pCurrent);

  // Do we have enough space at the end?
  if (m_pHistoryLineBeginning + len + 1 < m_pHistory + m_iHistorySizeBytes)
    {
      // Yes, copy the desired line to the end
      memcpy(m_pHistoryLineBeginning, pCurrent, len + 1);
      m_pHistoryCurrentPosition = m_pHistoryLineBeginning + len;
    }
  else
    {
      // Copy at the beginning, taking care of overlapping
      memmove(m_pHistory, pCurrent, len + 1);
      m_pHistoryLineBeginning = m_pHistory;
      m_pHistoryCurrentPosition = m_pHistoryLineBeginning + len;
    }
  return len;
}

static const char str_unknown[] = "Cmd?";

void
CommandLineInterface::processLine()
{
  std::ostream& cout = m_cout;
  Parser& parser = m_parser;

  if (*m_pHistoryLineBeginning != ASCII::STX)
    {
      OSDeviceDebug::putString(" no STX ");

      return;
    }

  OSDeviceDebug::putString("Line at ");
  OSDeviceDebug::putPtr( m_pHistoryLineBeginning);
  OSDeviceDebug::putNewLine();

  parser.setInput(m_pHistoryLineBeginning + 1);
  parser.setSeparators(" ");
  parser.setSpaces(" "); // used to ignore multiple spaces
  parser.setTokenBuffer(m_token, sizeof(m_token));

  OSReturn_t ret;
  ret = recurse((Token_t*) m_pToken);

  if ((ret == OSReturn::OS_OK) || (ret == OSReturn::OS_ERROR))
    return;

  if (ret == OSReturn::OS_BAD_COMMAND)
    {
      cout << str_unknown;
      return;
    }

  cout << "Error " << std::dec << ret;
}

OSReturn_t
CommandLineInterface::recurse(Token_t* pIn)
{
  Parser& parser = m_parser;

  parser.parseNextToken();

  if (parser.getTokenLength() == 0)
    return OSReturn::OS_BAD_COMMAND; // no token

  Token_t* p;

  p = pIn;
  while (p->pString != 0)
    {
      if (parser.compareStringWithToken((const char*) p->pString) == 0)
        {
          if (p->pDown != 0)
            return recurse(p->pDown);
          else if (p->pMethod != 0)
            return (*p->pMethod)(m_cin, m_cout, m_parser, *this,
                (void*) m_pObject, p->parameter);
        }
      ++p;
    }

  // If an exact match is not possible, retry as substring
  if (parser.getTokenLength() >= 2)
    {
      Token_t* pMatched;
      pMatched = NULL;

      uint_t cnt;

      p = pIn;
      for (cnt = 0; p->pString != 0; ++p)
        {
          // Check if the parameter starts with the parsed token
          if (parser.doesStringStartWithToken((const char*) p->pString))
            {
              pMatched = p;
              ++cnt;
            }
        }

      if (cnt == 1)
        {
          if (pMatched->pDown != 0)
            return recurse(pMatched->pDown);
          else if (pMatched->pMethod != 0)
            return (*pMatched->pMethod)(m_cin, m_cout, m_parser, *this,
                (void*) m_pObject, pMatched->parameter);
        }
    }

  return OSReturn::OS_BAD_COMMAND;
}

void
CommandLineInterface::recurseHelp(Token_t* pToken, uchar_t* pBuf,
    size_t bufSize, uint_t index)
{
  std::ostream& cout = m_cout;
  Token_t* p;

  int i;
  for (i = 0, p = pToken; p->pString != 0; ++p, ++i)
    {
      size_t len;
      len = strlen(p->pString);
      if (index + len + 1 < bufSize)
        {
          strcpy((char*) (pBuf + index), (const char*) (p->pString));
          pBuf[index + len] = ' ';
          pBuf[index + len + 1] = '\0';

          if (p->pDown != 0)
            recurseHelp(p->pDown, pBuf, bufSize, index + len + 1);
          else
            {
              if (!(i == 0 && index == 0))
                {
                  cout << std::endl;
                }
              cout << pBuf;
              if (p->pHelp != 0)
                cout << p->pHelp;
            }
        }
    }
}

#endif /* defined(OS_INCLUDE_COMMANDLINEINTERFACE) */
