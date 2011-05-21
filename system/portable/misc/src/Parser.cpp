/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_PARSER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/Parser.h"

Parser::Parser()
{
  OSDeviceDebug::putConstructor_P(PSTR("Parser"), this);

  m_pSeparators = (unsigned char*) "\0";
  m_pSpaces = 0;

  m_pToken = 0;
  m_tokenSize = 0;
}

void
Parser::setInput(unsigned char* pLine)
{
  m_pLine = pLine;
  m_pCrt = pLine;

  m_len = 0;
  m_sep = '\0';
}

void
Parser::setCurrent(unsigned short index)
{
  m_pCrt = &m_pLine[index];
}

OSReturn_t
Parser::skipTokens(unsigned short nTokens)
{
  return skipTokens(nTokens, m_pSeparators);
}

OSReturn_t
Parser::skipTokens(unsigned short nTokens, unsigned char* pSeparators)
{
  OSReturn_t ret;

  for (int i = 0; i < nTokens; ++i)
    {
      ret = parseToken(pSeparators, m_pSpaces);
      if (ret == '\0')
        break;
    }
  return ret;
}

OSReturn_t
Parser::parseToken(void)
{
  return parseToken(m_pSeparators, m_pSpaces);
}

// Parse to the next separator, ignoring spaces
OSReturn_t
Parser::parseToken(unsigned char* pSeparators, unsigned char* pSpaces)
{
  unsigned char* p;
  p = m_pToken;
  if (p == 0)
    return OSReturn::OS_NOT_INITIALIZED;

  *p = '\0';

  unsigned short tlen;
  tlen = 0;

  bool bFound;
  bFound = false;
  while (!bFound)
    {
      unsigned char c;
      c = *m_pCrt++;

      bFound = false;

      // First check if end of string reached
      if (c == '\0')
        {
          bFound = true;
          m_sep = '\0';
          --m_pCrt; // Remain on terminator
        }
      else
        {
          // Then check if a separator, like COMMA is encountered
          for (unsigned char* q = pSeparators; q && *q && !bFound; ++q)
            {
              if (*q == c)
                {
                  bFound = true;
                  m_sep = *q;
                  break;
                }
            }

          if (!bFound)
            {
              if (tlen < m_tokenSize - 1)
                {
                  // If there is enough free space in the token array
                  bool bSpaceFound;
                  bSpaceFound = false;

                  // Check if a special space character is encountered
                  for (unsigned char* q = pSpaces; q && *q && !bSpaceFound; ++q)
                    {
                      if (*q == c)
                        {
                          bSpaceFound = true;
                          break;
                        }
                    }
                  if (!bSpaceFound)
                    {
                      // Store the current character in the token array
                      *p++ = c;
                      *p = '\0';

                      tlen++;
                    }
                }
            }
        }
    }

  m_len = tlen;

#if defined(DEBUG) && defined(OS_DEBUG_PARSER_PARSETOKEN)
  OSDeviceDebug::putString("Token='");
  OSDeviceDebug::putString((const char*) m_pToken);
  OSDeviceDebug::putString("' 0x");
  OSDeviceDebug::putHex(m_sep);
  OSDeviceDebug::putNewLine();
#endif

  return m_sep;
}

OSReturn_t
Parser::parseSubstring(unsigned short len)
{
  if (len == 0)
    return OSReturn::OS_BAD_PARAMETER;

  unsigned char* p;
  p = m_pToken;
  *p = '\0';

  unsigned char c;

  unsigned short i;
  for (i = 0; i < len; ++i)
    {
      c = *m_pCrt++;
      if (i < m_tokenSize - 1)
        {
          *p++ = c;
        }
    }
  *p = '\0';

  return OSReturn::OS_OK;
}

OSReturn_t
Parser::parseHex(unsigned char* pChar)
{
  return parseHex(m_pToken, pChar);
}

OSReturn_t
Parser::parseUnsigned(unsigned short* pShort)
{
  return parseUnsigned(m_pToken, pShort);
}
OSReturn_t
Parser::parseSigned(signed long* pLong)
{
  return parseSigned(m_pToken, pLong);
}

OSReturn_t
Parser::parseFixedPrec(signed long* pLong, unsigned short prec)
{
  return parseFixedPrec(m_pToken, pLong, prec, true);
}

OSReturn_t
Parser::parseNibble(unsigned char nibble)
{
  unsigned char ch;

  ch = 0;
  if ('0' <= nibble && nibble <= '9')
    ch = (nibble - '0');
  else if ('a' <= nibble && nibble <= 'f')
    ch = (nibble - (unsigned char) 'a' + 10);
  else if ('A' <= nibble && nibble <= 'F')
    ch = (nibble - (unsigned char) 'A' + 10);
  else
    return OSReturn::OS_ILLEGAL_CHARACTER;

  return ch;
}

OSReturn_t
Parser::parseHex(unsigned char* pStr, unsigned char* pChar)
{
  unsigned char ch;

  ch = 0;
  for (int i = 0; i < 2; ++i)
    {
      ch <<= 4;

      unsigned char nibble;
      nibble = *pStr++;

      if ('0' <= nibble && nibble <= '9')
        ch |= (nibble - '0');
      else if ('a' <= nibble && nibble <= 'f')
        ch |= (nibble - (unsigned char) 'a' + 10);
      else if ('A' <= nibble && nibble <= 'F')
        ch |= (nibble - (unsigned char) 'A' + 10);
      else
        return OSReturn::OS_BAD_PARAMETER;
    }

  *pChar = ch;
  return OSReturn::OS_OK;
}

OSReturn_t
Parser::parseUnsigned(unsigned char* pStr, unsigned short* pShort)
{
  unsigned short w;
  unsigned char c;

  w = 0;
  while ((c = *pStr) != '\0')
    {
      if ('0' <= c && c <= '9')
        {
          w *= 10;
          w += (c - '0');
        }
      else
        return c;

      ++pStr;
    }
  *pShort = w;
  return OSReturn::OS_OK;
}

OSReturn_t
Parser::parseSigned(unsigned char* pStr, signed long* pLong)
{
  return parseFixedPrec(pStr, pLong, 0, true);
}

OSReturn_t
Parser::parseUnsigned(unsigned char* pStr, unsigned long* pLong)
{
  return parseFixedPrec(pStr, (signed long*) pLong, 0, false);
}

OSReturn_t
Parser::parseFixedPrec(unsigned char* pStr, signed long* pLong,
    unsigned short prec, bool hasSign)
{
  signed long l;
  unsigned char c;

  l = 0;
  bool bMinus;
  bMinus = false;

  for (; (c = *pStr) == ' ';)
    {
      ++pStr; // ignore leading spaces
    }

  if (hasSign && (c = *pStr) != '\0')
    {
      if (c == '-')
        {
          pStr++;
          bMinus = true;
        }
      else if (c == '+')
        {
          pStr++;
        }
    }

  while ((c = *pStr) != '\0')
    {
      if ('0' <= c && c <= '9')
        {
          l *= 10;
          l += (c - '0');
        }
      else if (c == '.')
        {
          ++pStr; // skip '.'
          break;
        }
      else
        return c;

      ++pStr;
    }

  for (uint_t i = 0; i < prec; ++i)
    {
      l *= 10;
      c = *pStr;
      if (c != '\0')
        {
          if ('0' <= c && c <= '9')
            {
              l += (c - '0');
            }
          else
            return c;

          ++pStr;
        }
    }

  if (bMinus)
    l = -l;

  *pLong = l;
  return OSReturn::OS_OK;
}

int
Parser::tokenCompare(const unsigned char* pStr)
{
  return strcmp((const char*) m_pToken, (const char*) pStr);
}

// Warning: does not work on AVR32
int
Parser::tokenCompareIgnoreCase(const unsigned char* pStr)
{
  return strcasecmp((const char*) m_pToken, (const char*) pStr);
}

#endif /* defined(OS_INCLUDE_PARSER) */
