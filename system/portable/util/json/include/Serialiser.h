/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_JSON_SERIALISER_H_
#define UTIL_JSON_SERIALISER_H_

#include "portable/kernel/include/OS.h"
#include "portable/stdlib/include/ostream"

namespace util
{
  namespace json
  {
    class Serialiser
    {
    public:

      // ----- Constants ------------------------------------------------------

      const static uchar_t SEPARATOR = ',';
      const static uchar_t ARRAY_BEGIN = '[';
      const static uchar_t ARRAY_END = ']';
      const static uchar_t OBJECT_BEGIN = '{';
      const static uchar_t OBJECT_END = '}';

      // ----- Constructors and destructors -----------------------------------

      Serialiser(std::ostream& out);
      ~Serialiser();

      // ---- Public methods --------------------------------------------------

      void
      putChar(uchar_t c);

      void
      putVariableName(const char* str);
      void
      putNumber(uint_t n);
      void
      putNumber(ulong_t n);
      void
      putNumber(int n);
      void
      putNumberArray(uint8_t* pb, std::size_t len);
      void
      putQuotedString(const char* str);
      void
      putUnhandledString(const char* str);
      void
      putBoolean(bool flag);
      void
      putNull(void);

    private:

      std::ostream& m_out;
    };
  }
}

#endif /* UTIL_JSON_SERIALISER_H_ */
