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
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("Parser()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    m_pSeparators = (unsigned char *)"\0";
    m_pSpaces = 0;

    m_pToken = 0;
    m_tokenSize = 0;
  }

void Parser::setInput(unsigned char *pLine)
  {
    m_pLine = pLine;
    m_pCrt = pLine;

    m_len = 0;
    m_sep = '\0';
  }

void Parser::setCurrent(unsigned short index)
  {
    m_pCrt = &m_pLine[index];
  }

OSReturn_t Parser::parseToken(void)
  {
    return parseToken(m_pSeparators, m_pSpaces);
  }

OSReturn_t Parser::parseToken(unsigned char *pSeparators,
    unsigned char *pSpaces)
  {
    unsigned char *p;
    p = m_pToken;
    if (p == 0)
      return OSReturn::NOT_INITIALIZED;

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

        if (c == '\0')
          {
            bFound = true;
            m_sep = '\0';
          }
        else
          {
            for (unsigned char *q = pSeparators; q && *q && !bFound; ++q)
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
                if (tlen < m_tokenSize-1)
                  {
                    bool bSpaceFound;
                    bSpaceFound = false;

                    for (unsigned char *q = pSpaces; q && *q && !bSpaceFound; ++q)
                      {
                        if (*q == c)
                          {
                            bSpaceFound = true;
                            break;
                          }
                      }
                    if (!bSpaceFound)
                      {
                        *p++ = c;
                        *p = '\0';

                        tlen++;
                      }
                  }
              }
          }
      }

    m_len = tlen;

    return m_sep;
  }

OSReturn_t Parser::parseSubstring(unsigned short len)
  {
    if (len == 0)
      return OSReturn::OS_BAD_PARAMETER;

    unsigned char *p;
    p = m_pToken;
    *p = '\0';

    unsigned char c;

    unsigned short i;
    for (i = 0; i < len; ++i)
      {
        c = *m_pCrt++;
        if (i < m_tokenSize-1)
          {
            *p++ = c;
          }
      }
    *p = '\0';

    return OSReturn::OS_OK;
  }

OSReturn_t Parser::parseHex(unsigned char *pChar)
  {
    return parseHex(pChar, m_pToken);
  }

OSReturn_t Parser::parseUnsigned(unsigned short *pShort)
  {
    return parseUnsigned(pShort, m_pToken);
  }
OSReturn_t Parser::parseSigned(signed long *pLong)
  {
    return parseSigned(pLong, m_pToken);
  }

OSReturn_t Parser::parseFixedPrec(signed long *pLong, unsigned short prec)
  {
    return parseFixedPrec(pLong, prec, true, m_pToken);
  }

OSReturn_t Parser::parseNibble(unsigned char nibble)
  {
    unsigned char ch;

    ch = 0;
    if ( '0' <= nibble && nibble <= '9')
      ch = (nibble - '0' );
    else if ( 'a' <= nibble && nibble <= 'f')
      ch = (nibble - ( unsigned char ) 'a' + 10 );
    else if ( 'A' <= nibble && nibble <= 'F')
      ch = (nibble - ( unsigned char ) 'A' + 10 );
    else
      return OSReturn::OS_ILLEGAL_CHARACTER;

    return ch;
  }

OSReturn_t Parser::parseHex(unsigned char *pChar, unsigned char *pStr)
  {
    unsigned char ch;

    ch = 0;
    for (int i = 0; i < 2; ++i)
      {
        ch <<= 4;

        unsigned char nibble;
        nibble = *pStr++;

        if ( '0' <= nibble && nibble <= '9')
          ch |= (nibble - '0' );
        else if ( 'a' <= nibble && nibble <= 'f')
          ch |= (nibble - (unsigned char)'a' + 10 );
        else if ( 'A' <= nibble && nibble <= 'F')
          ch |= (nibble - (unsigned char)'A' + 10 );
        else
          return OSReturn::OS_BAD_PARAMETER;
      }

    *pChar = ch;
    return OSReturn::OS_OK;
  }

OSReturn_t Parser::parseUnsigned(unsigned short *pShort, unsigned char *pStr)
  {
    unsigned short w;
    unsigned char c;

    w = 0;
    while ((c = *pStr) != '\0')
      {
        if ( '0' <= c && c <= '9')
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

OSReturn_t Parser::parseSigned(signed long *pLong, unsigned char *pStr)
  {
    return parseFixedPrec(pLong, 0, true, pStr);
  }

OSReturn_t Parser::parseUnsigned(unsigned long *pLong, unsigned char *pStr)
  {
    return parseFixedPrec((signed long *)pLong, 0, false, pStr);
  }

OSReturn_t Parser::parseFixedPrec(signed long *pLong, unsigned short prec,
    bool hasSign, unsigned char *pStr)
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
        if ( '0' <= c && c <= '9')
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

    for (unsigned int i = 0; i < prec; ++i)
      {
        l *= 10;
        c = *pStr;
        if (c != '\0')
          {
            if ( '0' <= c && c <= '9')
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

#endif /*OS_INCLUDE_PARSER*/
