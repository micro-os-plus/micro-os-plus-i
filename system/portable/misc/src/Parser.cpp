/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_PARSER)

#include "portable/kernel/include/OS.h"

#include "portable/misc/include/Parser.h"
#include <string.h>
#include <ctype.h>

Parser::Parser()
{
  OSDeviceDebug::putConstructor_P(PSTR("Parser"), this);

  m_pSeparators = (uchar_t*) "\0";
  m_pSpaces = 0;

  m_pToken = 0;
  m_tokenSize = 0;
}

void
Parser::setInput(uchar_t* pLine)
{
  m_pLine = pLine;
  m_pCrt = pLine;

  m_tokenLength = 0;
  m_sep = '\0';
}

void
Parser::clearToken(void)
{
  if (m_pToken != NULL)
    *m_pToken = '\0';

  m_tokenLength = 0;
}

void
Parser::setCurrentPosition(uint_t index)
{
  m_pCrt = &m_pLine[index];
}

OSReturn_t
Parser::skipTokens(uint_t nTokens)
{
  return skipTokens(nTokens, (const char*) m_pSeparators);
}

OSReturn_t
Parser::skipTokens(uint_t nTokens, const char* pSeparators)
{
  clearToken();

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  for (uint_t i = 0; i < nTokens; ++i)
    {
      ret = parseNextToken(pSeparators, (const char*) m_pSpaces);
      if (ret == '\0')
        break;
    }
  return ret;
}

OSReturn_t
Parser::parseNextToken(void)
{
  return parseNextToken((const char*) m_pSeparators, (const char*) m_pSpaces);
}

// Parse to the next separator, ignoring spaces
OSReturn_t
Parser::parseNextToken(const char* pSeparators, const char* pSpaces)
{
  clearToken();

  uchar_t* p;
  p = m_pToken;
  if (p == 0)
    {
      OSDeviceDebug::putString("Not initialised");
      return OSReturn::OS_NOT_INITIALISED;
    }
  *p = '\0';

  size_t tlen;
  tlen = 0;

  uchar_t c;
  bool bSpaceFound;

  for (;;)
    {
      c = *m_pCrt;
      bSpaceFound = false;
      for (uchar_t* q = (uchar_t*) pSpaces; q && *q && !bSpaceFound; ++q)
        {
          if (*q == c)
            {
              bSpaceFound = true;
              break;
            }
        }

      // If not space found, that's great, we can start parsing
      if (!bSpaceFound)
        break;

      // If space found, skip it and continue to loop
      m_pCrt++;
    }

  // We are now on the first non space character

  bool bFound;
  bFound = false;
  while (!bFound)
    {
      c = *m_pCrt++;

      bFound = false;

      // First check if end of string reached
      if (c == '\0')
        {
          bFound = true;
          m_sep = '\0';
          --m_pCrt; // Point to the terminator
          // This make future parseNextToken do not exceed the input
        }
      else
        {
          // If not end of line, check if a separator, like COMMA is encountered
          for (uchar_t* q = (uchar_t*) pSeparators; q && *q && !bFound; ++q)
            {
              if (*q == c)
                {
                  bFound = true;
                  m_sep = *q; // Remeber the separator that terminated the call
                  break;
                }
            }

          if (!bFound)
            {
              // Here we have a regular character, to be returned.

              // First check if there is enough free space in the token array
              if (tlen < m_tokenSize - 1)
                {
                  bSpaceFound = false;

                  // Check if a special space character is encountered
                  for (uchar_t* q = (uchar_t*) pSpaces; q && *q && !bSpaceFound; ++q)
                    {
                      if (*q == c)
                        {
                          bSpaceFound = true;
                          break;
                        }
                    }
                  // If space, ignore it, do not store in the output
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

  m_tokenLength = tlen;

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
Parser::parseNextSubstring(size_t len)
{
  clearToken();

  if (len == 0)
    return OSReturn::OS_BAD_PARAMETER;

  uchar_t* p;
  p = m_pToken;
  *p = '\0';

  uchar_t c;

  size_t i;
  for (i = 0; i < len; ++i)
    {
      c = *m_pCrt++;
      if (i < m_tokenSize - 1)
        {
          *p++ = c;
          m_tokenLength++;
        }
    }
  *p = '\0';

  return OSReturn::OS_OK;
}

OSReturn_t
Parser::convertHex(uchar_t* pChar)
{
  return convertHex(m_pToken, pChar, 1);
}

OSReturn_t
Parser::convertHex(uint16_t* pShort)
{
  return convertHex(m_pToken, pShort, 2);
}

OSReturn_t
Parser::convertHex(uint32_t* pLong)
{
  return convertHex(m_pToken, pLong, 4);
}

OSReturn_t
Parser::convertHex(uint_t* pInt)
{
  return convertHex(m_pToken, pInt, sizeof(*pInt));
}

OSReturn_t
Parser::convertUnsigned(unsigned short* pShort)
{
  return convertUnsigned(m_pToken, pShort);
}

OSReturn_t
Parser::convertUnsigned(uint32_t* pLong)
{
  return convertUnsigned(m_pToken, pLong);
}

OSReturn_t
Parser::convertSigned(signed long* pLong)
{
  return convertSigned(m_pToken, pLong);
}

OSReturn_t
Parser::convertFixedPrecision(int32_t* pLong, uint_t prec)
{
  return convertFixedPrecision(m_pToken, pLong, prec, true);
}

OSReturn_t
Parser::convertNibble(unsigned char nibble)
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
Parser::convertHex(unsigned char* pStr, void* p, size_t size)
{
  if (p == NULL)
    {
      OSDeviceDebug::putString("Null Pointer");
      return OSReturn::OS_NULL_POINTER;
    }

  uint32_t v;
  v = 0;

  for (uchar_t nibble; ((nibble = *pStr) != '\0'); pStr++)
    {
      v <<= 4;

      if ('0' <= nibble && nibble <= '9')
        v |= (nibble - '0');
      else if ('a' <= nibble && nibble <= 'f')
        v |= (nibble - (unsigned char) 'a' + 10);
      else if ('A' <= nibble && nibble <= 'F')
        v |= (nibble - (unsigned char) 'A' + 10);
      else
        return OSReturn::OS_BAD_PARAMETER;
    }

  if (size == 1)
    *((uint8_t*) p) = (uint8_t) v;
  else if (size == 2)
    *((uint16_t*) p) = (uint16_t) v;
  else
    *((uint32_t*) p) = (uint32_t) v;

  return OSReturn::OS_OK;
}

OSReturn_t
Parser::convertUnsigned(uchar_t* pStr, uint16_t* pShort)
{
  if (pShort == NULL)
    {
      OSDeviceDebug::putString("Null Pointer");
      return OSReturn::OS_NULL_POINTER;
    }

  uint16_t w;
  w = 0;

  uchar_t ch;

  while ((ch = *pStr) != '\0')
    {
      if ('0' <= ch && ch <= '9')
        {
          w *= 10;
          w += (ch - '0');
        }
      else
        return ch;

      ++pStr;
    }
  *pShort = w;
  return OSReturn::OS_OK;
}

OSReturn_t
Parser::convertSigned(uchar_t* pStr, int32_t* pLong)
{
  return convertFixedPrecision(pStr, pLong, 0, true);
}

OSReturn_t
Parser::convertUnsigned(uchar_t* pStr, uint32_t* pLong)
{
  return convertFixedPrecision(pStr, (signed long*) pLong, 0, false);
}

OSReturn_t
Parser::convertFixedPrecision(uchar_t* pStr, int32_t* pLong, uint_t prec,
    bool hasSign)
{
  if (pLong == NULL)
    {
      OSDeviceDebug::putString("Null Pointer");
      return OSReturn::OS_NULL_POINTER;
    }

  int32_t l;
  l = 0;

  bool bMinus;
  bMinus = false;

  uchar_t ch;

  for (; isspace((ch = *pStr));)
    {
      ++pStr; // ignore leading spaces
    }

  if (*pStr == '\0')
    {
      return OSReturn::OS_EMPTY_STRING;
    }

  if (hasSign && (ch = *pStr) != '\0')
    {
      if (ch == '-')
        {
          pStr++;
          bMinus = true;
        }
      else if (ch == '+')
        {
          pStr++;
        }
    }

  while ((ch = *pStr) != '\0')
    {
      if ('0' <= ch && ch <= '9')
        {
          l *= 10;
          l += (ch - '0');
        }
      else if (ch == '.')
        {
          ++pStr; // skip '.'
          break;
        }
      else
        return ch;

      ++pStr;
    }

  for (uint_t i = 0; i < prec; ++i)
    {
      l *= 10;
      ch = *pStr;
      if (ch != '\0')
        {
          if ('0' <= ch && ch <= '9')
            {
              l += (ch - '0');
            }
          else
            return ch;

          ++pStr;
        }
    }

  if (bMinus)
    l = -l;

  *pLong = l;
  return OSReturn::OS_OK;
}

#if false
bool
Parser::doesTokenStartWith(const unsigned char* pStr)
  {
    return (strncmp((const char*) m_pToken, (const char*) pStr,
            strlen((const char*) pStr)) == 0);
  }
#endif

bool
Parser::doesStringStartWithToken(const char* pStr)
{
  if (m_tokenLength == 0)
    return false;

  return (strncmp(pStr, (const char*) m_pToken, strlen((const char*) m_pToken))
      == 0);
}

int
Parser::compareStringWithToken(const char* pStr)
{
  return strcmp(pStr, (const char*) m_pToken);
}

bool
Parser::doesStringMatchToken(const char* pStr)
{
  if (m_tokenLength == 0)
    return false;

  return (strcmp(pStr, (const char*) m_pToken) == 0);
}

// Warning: does not work on AVR32
int
Parser::compareStringWithTokenIgnoreCase(const char* pStr)
{
  return strcasecmp(pStr, (const char*) m_pToken);
}

#endif /* defined(OS_INCLUDE_PARSER) */
