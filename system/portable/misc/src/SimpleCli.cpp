/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_SIMPLECLI)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/SimpleCli.h"

SimpleCli::SimpleCli(unsigned char *pLine, unsigned short iSize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("SimpleCli()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_pLine = pLine;
  m_iSize = iSize;
}

SimpleCli::~SimpleCli()
{
}

void
SimpleCli::parseReset(void)
{
  m_pNext = m_pLine;
}

unsigned char *
SimpleCli::parseNext(void)
{
  unsigned char * pc;
  unsigned char c;

  for (; c = *m_pNext, c == ' ' || c == '\t'; ++m_pNext)
    {
      ;
    }
  pc = m_pNext;
  if (c == '\0')
    return 0;

  for (; c = *m_pNext, c != '\0' && c != ' ' && c != '\t'; ++m_pNext)
    ;

  if (c != '\0')
    {
      *m_pNext++ = '\0';
    }

  return pc;
}

#ifdef OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE

char
SimpleCli::parseHexNibble(unsigned char *p, unsigned char *pc)
{
  unsigned char ch, tmp;

  ch = 0;
  while ((tmp = *p) != '\0')
    {
      ch <<= 4;
      if ('0' <= tmp && tmp <= '9')
        ch |= (tmp - '0');
      else if ('a' <= tmp && tmp <= 'f')
        ch |= (tmp - (unsigned char) 'a' + 10);
      else if ('A' <= tmp && tmp <= 'F')
        ch |= (tmp - (unsigned char) 'A' + 10);
      else
        return -1;
      ++p;
    }
  *pc = ch;
  return 0;
}

#endif /* OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE */

#ifdef OS_INCLUDE_SIMPLECLI_PARSE_HEX_CHAR

// Convert two ascii characters to hex
char
SimpleCli::parseHex( unsigned char *p, unsigned char *pc )
  {
    unsigned char ch, i, tmp;

    ch = 0;
    for ( i = 2; i != 0; --i )
      {
        ch <<= 4;
        tmp = *p;
        if ( '0' <= tmp && tmp <= '9' )
        ch |= ( tmp - '0' );
        else if ( 'a' <= tmp && tmp <= 'f' )
        ch |= ( tmp - ( unsigned char ) 'a' + 10 );
        else if ( 'A' <= tmp && tmp <= 'F' )
        ch |= ( tmp - ( unsigned char ) 'A' + 10 );
        else
        return -1;

        ++p;
      }
    *pc = ch;
    return 0;
  }

#endif /* OS_INCLUDE_SIMPLECLI_PARSE_HEX_CHAR */

#ifdef OS_INCLUDE_SIMPLECLI_PARSE_HEX_SHORT

char SimpleCli::parseHex(unsigned char *p, unsigned short *pw)
  {
    unsigned char tmp;
    unsigned short sh;

    sh = 0;
    while ( (tmp = *p ) != '\0')
      {
        sh <<= 4;
        if ( '0' <= tmp && tmp <= '9')
        sh |= (tmp - '0' );
        else if ( 'a' <= tmp && tmp <= 'f')
        sh |= (tmp - ( unsigned char ) 'a' + 10 );
        else if ( 'A' <= tmp && tmp <= 'F')
        sh |= (tmp - ( unsigned char ) 'A' + 10 );
        else
        return -1;
        ++p;
      }
    *pw = sh;
    return 0;
  }

#endif /* OS_INCLUDE_SIMP<ECLI_PARSE_HEX_SHORT */

#ifdef OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG

char
SimpleCli::parseHex(unsigned char *p, unsigned long *pl)
{
  unsigned char tmp;
  unsigned long lh;

  lh = 0;
  while ((tmp = *p) != '\0')
    {
      lh <<= 4;
      if ('0' <= tmp && tmp <= '9')
        lh |= (tmp - '0');
      else if ('a' <= tmp && tmp <= 'f')
        lh |= (tmp - (unsigned char) 'a' + 10);
      else if ('A' <= tmp && tmp <= 'F')
        lh |= (tmp - (unsigned char) 'A' + 10);
      else
        return -1;
      ++p;
    }
  *pl = lh;
  return 0;
}

#endif /* OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG */

#ifdef OS_INCLUDE_SIMPLECLI_PARSE_UNSIGNED_SHORT

char
SimpleCli::parseUnsigned( unsigned char *p, unsigned short *pw )
  {
    unsigned short sh;
    unsigned char tmp;

    sh = 0;
    while ( ( tmp = *p ) != '\0' )
      {
        sh *= 10;
        if ( '0' <= tmp && tmp <= '9' )
        sh += ( tmp - '0' );
        else
        return -1;
        ++p;
      }
    *pw = sh;
    return 0;
  }

#endif /* OS_INCLUDE_SIMPLECLI_PARSE_UNSIGNED_SHORT */

#define CHR_ASCII_BELL 0x07

int
SimpleCli::readLine(istream& cin, ostream& cout)
{
  unsigned char * pc;
  int c;

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
                  c = CHR_ASCII_BELL;
                  cout.put(c);
                }
            }
          else
            {
              if (((unsigned short) (pc - m_pLine)) < (m_iSize - 2))
                *pc++ = c;
              else
                {
                  c = CHR_ASCII_BELL;
                  cout.put(c);
                }
            }
          if ((' ' <= c) && (c < 0x7F))
            {
              cout.put(c);
            }
        }
    }
  //*pc++ = ' ';
  *pc = '\0';

  return pc - m_pLine;
}

#endif
