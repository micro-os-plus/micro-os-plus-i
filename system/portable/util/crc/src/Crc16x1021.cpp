/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_CRC_CRC16X1021)

#include "portable/kernel/include/OS.h"

#include "portable/util/crc/include/Crc16x1021.h"

namespace util
{
  namespace crc
  {
    // ----- Constructor ------------------------------------------------------

    Crc16x1021::Crc16x1021()
    {
      OSDeviceDebug::putConstructor_P(PSTR("Crc16x1021"), this);
    }

    // ----- Destructor -------------------------------------------------------

    Crc16x1021::~Crc16x1021()
    {
      OSDeviceDebug::putDestructor_P(PSTR("Crc16x1021"), this);
    }

    // ----- Static methods ---------------------------------------------------

    // Compute the polynomial for a single byte
    void
    Crc16x1021::computePolynomial(uint16_t& crc, uint8_t b)
    {
      uint16_t acc;

      acc = crc ^ ((uint16_t) b << 8);
      uint_t i;
      for (i = 0; i < 8; i++)
        {
          if (acc & 0x8000)
            acc = (acc << 1) ^ 0x1021;
          else
            acc <<= 1;
        }
      // Update reference to return the newly computed value
      crc = acc;
    }

    // Compute the polynomial for an array of bytes
    void
    Crc16x1021::computePolynomial(uint16_t& crc, uint8_t* pb, std::size_t n)
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

#endif /* defined(OS_INCLUDE_UTIL_CRC_CRC16X1021) */
