/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_ENDIANESS_BIGENDIAN_H_
#define UTIL_ENDIANESS_BIGENDIAN_H_

#include "portable/kernel/include/OS.h"

namespace util
{
  namespace endianness
  {
    class BigEndian
    {
    public:

      // ---- Static methods --------------------------------------------------

      static uint16_t
      readUnsigned16(const uint8_t* pByte);

      static void
      writeUnsigned16(uint16_t value, uint8_t* pByte);

      static uint32_t
      readUnsigned24(const uint8_t* pByte);

      static void
      writeUnsigned24(uint32_t value, uint8_t* pByte);

      static uint32_t
      readUnsigned32(const uint8_t* pByte);

      static void
      writeUnsigned32(uint32_t value, uint8_t* pByte);

    };
  }
}

#endif /* UTIL_ENDIANESS_BIGENDIAN_H_ */
