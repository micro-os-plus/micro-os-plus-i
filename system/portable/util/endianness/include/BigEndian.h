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
      readUnsigned16(uint8_t* pByte);

      static void
      writeUnsigned16(uint8_t* pByte, uint16_t value);

      static uint32_t
      readUnsigned32(uint8_t* pByte);

      static void
      writeUnsigned32(uint8_t* pByte, uint32_t value);

    };
  }
}

#endif /* UTIL_ENDIANESS_BIGENDIAN_H_ */
