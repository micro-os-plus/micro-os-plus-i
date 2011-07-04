/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_JSON_SERIALISER)

#include "portable/kernel/include/OS.h"

#include "portable/util/json/include/Serialiser.h"

namespace util
{
  namespace json
  {
    // ----- Constructor ------------------------------------------------------

    Serialiser::Serialiser(std::ostream& out) :
      m_out(out)
    {
      OSDeviceDebug::putConstructor_P(PSTR("util::json::Serialiser"), this);
    }

    // ----- Destructor -------------------------------------------------------

    Serialiser::~Serialiser()
    {
      OSDeviceDebug::putDestructor_P(PSTR("util::json::Serialiser"), this);
    }

    // ----- Object methods----------------------------------------------------

    void
    Serialiser::putChar(uchar_t c)
    {
      m_out << c;
    }

    void
    Serialiser::putVariableName(const char* str)
    {
      m_out << '"' << str << "\":";
    }

    void
    Serialiser::putNumber(uint_t n)
    {
      m_out << std::dec << n;
    }

    void
    Serialiser::putNumber(ulong_t n)
    {
      m_out << std::dec << n;
    }

    void
    Serialiser::putNumberArray(uint8_t* pb, std::size_t len)
    {
      m_out.put(ARRAY_BEGIN);
      std::size_t i;
      for ( i = 0; i < len; ++i, ++pb)
        {
          if (i > 0){
              m_out << SEPARATOR;
          }
          m_out << std::dec << (uint_t)(*pb);
        }
      m_out.put(ARRAY_END);
    }

    void
    Serialiser::putQuotedString(const char* str)
    {
      // TODO: quote \" and other special chars
      m_out << '"';
      uchar_t c;
      for (; (c = *str) != '\0'; ++str)
        {
          if (c == '"' || c == '\\' || c == '/')
            {
              m_out.put('\\');
              m_out.put(c);
            }
          else if (c == '\b')
            {
              m_out.put('\\');
              m_out.put('b');
            }
          else if (c == '\f')
            {
              m_out.put('\\');
              m_out.put('f');
            }
          else if (c == '\n')
            {
              m_out.put('\\');
              m_out.put('n');
            }
          else if (c == '\r')
            {
              m_out.put('\\');
              m_out.put('r');
            }
          else if (c == '\t')
            {
              m_out.put('\\');
              m_out.put('t');
            }
          else if (' ' <= c && c <= '~')
            {
              // Printable characters
              m_out.put(c);
            }
          else
            {
              // All other non printable passed as 4 bytes hex
              m_out << "\\x" << std::hex;
              m_out.width(4);
              m_out << (uint_t)c;
            }
        }
      m_out << str;
      m_out << '"';
    }

    void
    Serialiser::putJsonString(const char* str)
    {
      m_out << '{';
      m_out << str;
      m_out << '}';
    }

  }
}

#endif /* defined(OS_INCLUDE_UTIL_JSON_SERIALISER) */
