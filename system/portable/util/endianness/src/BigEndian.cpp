/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_ENDIANNESS_BIGENDIAN)

#include "portable/kernel/include/OS.h"

#include "portable/util/endianness/include/BigEndian.h"

namespace util
  {
    namespace endianness
      {

        // ----- Static methods ---------------------------------------------------

        uint16_t
        BigEndian::readUnsigned16(const uint8_t* pByte)
          {
            uint16_t value;
            value = 0;

            //value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;

            return value;
          }

        void
        BigEndian::writeUnsigned16(uint16_t value,  uint8_t* pByte)
          {
            // Start from low byte and proceed in reverse order
            pByte += 2;

            uint8_t b;

            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            //value >>= 8;
          }

        uint32_t
        BigEndian::readUnsigned24(const uint8_t* pByte)
          {
            uint32_t value;
            value = 0;

            //value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;

            return value;
          }

        void
        BigEndian::writeUnsigned24(uint32_t value, uint8_t* pByte)
          {
            // Start from low byte and proceed in reverse order
            pByte += 3;

            uint8_t b;

            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            //value >>= 8;
          }

        uint32_t
        BigEndian::readUnsigned32(const uint8_t* pByte)
          {
            uint32_t value;
            value = 0;

            //value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;
            value <<= 8;
            value |= *pByte++;

            return value;
          }

        void
        BigEndian::writeUnsigned32(uint32_t value, uint8_t* pByte)
          {
            // Start from low byte and proceed in reverse order
            pByte += 4;

            uint8_t b;

            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            value >>= 8;
            b = (value & 0xFF);
            *--pByte = b;
            //value >>= 8;
          }

        // ------------------------------------------------------------------------
      }
  }

#endif /* defined(OS_INCLUDE_UTIL_BIGENDIAN) */
