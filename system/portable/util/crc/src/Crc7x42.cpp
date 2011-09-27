/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_CRC_CRC7X42)

#include "portable/kernel/include/OS.h"

#include "portable/util/crc/include/Crc7x42.h"

namespace util
{
  namespace crc
  {
    // ----- Constructor ------------------------------------------------------

    Crc7x42::Crc7x42()
    {
      OSDeviceDebug::putConstructor_P(PSTR("util::crc::Crc7x42"), this);
    }

    // ----- Destructor -------------------------------------------------------

    Crc7x42::~Crc7x42()
    {
      OSDeviceDebug::putDestructor_P(PSTR("util::crc::Crc7x42"), this);
    }

    // ----- Static methods ---------------------------------------------------

    // Compute the polynomial for a single byte
    void
    Crc7x42::computePolynomial(Value_t& crc, uint8_t b)
    {
      Value_t acc;

      acc = crc;
      for (uint_t i = 0; i < 8; i++)
        {
          acc <<= 1;
          if (((b << i) ^ acc) & 0x80)
            acc ^= 0x09;
        }
      // Update reference to return the newly computed value
      crc = acc;
    }

    // Compute the polynomial for an array of bytes
    void
    Crc7x42::computePolynomial(Value_t& crc, uint8_t* pb, std::size_t n)
    {
      std::size_t i;
      for (i = 0; i < n; ++i)
        {
          computePolynomial(crc, *pb++);
        }
    }

    // ------------------------------------------------------------------------
  }
}

#endif /* defined(OS_INCLUDE_UTIL_CRC_CRC7X42) */
