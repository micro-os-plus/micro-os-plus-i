/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_JSON_PARSER)

#include "portable/kernel/include/OS.h"

#include "portable/util/json/include/Parser.h"
#include "portable/util/json/include/Serialiser.h"

namespace util
{
  namespace json
  {
    // ----- Constructor ------------------------------------------------------

    Parser::Parser(const uchar_t* pBuf, std::size_t len)
    {
      OSDeviceDebug::putConstructor_P(PSTR("util::json::Parser"), this);

      setInput(pBuf, len);
    }

    // ----- Destructor -------------------------------------------------------

    Parser::~Parser()
    {
      OSDeviceDebug::putDestructor_P(PSTR("util::json::Parser"), this);
    }

    // ----- Object methods----------------------------------------------------

    void
    Parser::setInput(const uchar_t* pBuf, std::size_t len)
    {
      m_pBuf = pBuf;
      m_len = len;

      m_pCrt = pBuf;
      m_remLen = len;
    }

#if false
    uchar_t
    Parser::getCurrentChar(void)
      {
        OSDeviceDebug::putChar(*m_pCrt);
        return *m_pCrt;
      }
#endif

    void
    Parser::advanceCurrent(void)
    {
      --m_remLen;
      ++m_pCrt;
    }

    uchar_t
    Parser::skipSpaces(void)
    {
      for (; hasMoreContent(); advanceCurrent())
        {
          uchar_t ch;
          ch = getCurrentChar();
          if (ch != ' ' && ch != '\t')
            return ch;
        }

      return '\0';
    }

    OSReturn_t
    Parser::skipCharacter(uchar_t c)
    {
      uchar_t ch;

      ch = skipSpaces();

      if (m_remLen > 0)
        {
          if (ch == c)
            {
              advanceCurrent();

              return OSReturn::OS_OK;
            }
        }

      return OSReturn::OS_NOT_FOUND;
    }

    OSReturn_t
    Parser::parseVariableName(uchar_t* pName, std::size_t siz)
    {
      OSReturn_t r;

      r = skipCharacter('"');
      if (r != OSReturn::OS_OK)
        return r;

      bool isTooLarge;
      isTooLarge = false;

      for (; hasMoreContent(); advanceCurrent())
        {
          uchar_t ch;
          ch = getCurrentChar();

          if (ch == '"')
            {
              // Skip over terminating "
              advanceCurrent();
              break;
            }

          if (siz > 1)
            {
              *pName++ = ch;
              *pName = '\0';
              --siz;
            }
          else
            {
              isTooLarge = true;
            }
        }

      r = skipCharacter(':');
      if (r != OSReturn::OS_OK)
        return r;

      if (isTooLarge)
        return OSReturn::OS_NOT_ENOUGH_SPACE;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Parser::parseString(uchar_t* string, std::size_t siz)
    {
      OSReturn_t r;

      uchar_t ch;
      ch = skipSpaces();

      r = skipCharacter('"');
      if (r != OSReturn::OS_OK)
        return r;

      bool isTooLarge;
      isTooLarge = false;

      for (; hasMoreContent(); advanceCurrent())
        {
          ch = getCurrentChar();

          if (ch == '"')
            {
              // Skip over terminating "
              advanceCurrent();
              break;
            }

          if (siz > 1)
            {
              *string++ = ch;
              *string = '\0';
              --siz;
            }
          else
            {
              isTooLarge = true;
            }
        }

      if (isTooLarge)
        return OSReturn::OS_NOT_ENOUGH_SPACE;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Parser::parseNumber(uint32_t& num)
    {
      uint32_t n;
      n = 0;

      uchar_t ch;
      ch = skipSpaces();

      for (; hasMoreContent(); advanceCurrent())
        {
          uchar_t ch;
          ch = getCurrentChar();

          if ('0' <= ch && ch <= '9')
            {
              n = n * 10 + (ch - '0');
            }
          else
            break;
        }
      num = n;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Parser::parseNumber(uint64_t& num)
    {
      uint64_t n;
      n = 0;

      uchar_t ch;
      ch = skipSpaces();

      for (; hasMoreContent(); advanceCurrent())
        {
          uchar_t ch;
          ch = getCurrentChar();

          if ('0' <= ch && ch <= '9')
            {
              n = n * 10 + (ch - '0');
            }
          else
            break;
        }
      num = n;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Parser::parseNumber(int32_t& num)
    {
      int32_t n;
      n = 0;

      bool isNegative;
      isNegative = false;

      uchar_t ch;
      ch = skipSpaces();
      if (ch == '-')
        {
          isNegative = true;
          advanceCurrent();
        }

      for (; hasMoreContent(); advanceCurrent())
        {
          uchar_t ch;
          ch = getCurrentChar();

          if ('0' <= ch && ch <= '9')
            {
              n = n * 10 + (ch - '0');
            }
          else
            break;
        }

      if (isNegative)
        num = -n;
      else
        num = n;

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Parser::parseByteArray(uint8_t* pByteArray, std::size_t size,
        std::size_t *pLength)
    {
      OSReturn_t r;

      uint32_t n_32, i;

      uchar_t ch;
      ch = skipSpaces();

      r = skipCharacter('[');
      if (r != OSReturn::OS_OK)
        return r;

      for (i = 0; hasMoreContent(); i++)
        {
          if (i == size)
              return OSReturn::OS_NOT_ENOUGH_SPACE;

          r = parseNumber(n_32);
          if (r != OSReturn::OS_OK)
              return r;

          pByteArray[i] = (uint8_t) n_32;

          r = skipCharacter(',');
          if (r != OSReturn::OS_OK)
            break;
        }

      r = skipCharacter(']');
      if (r != OSReturn::OS_OK)
        return r;

      *pLength = i+1;

      return OSReturn::OS_OK;
    }

  // --------------------------------------------------------------------------
  }
}

#endif /* defined(OS_INCLUDE_UTIL_JSON_PARSER) */

