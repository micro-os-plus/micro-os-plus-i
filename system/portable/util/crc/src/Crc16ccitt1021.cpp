/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_UTIL_CRC_CRC16CCITT1021)

#include "portable/kernel/include/OS.h"

#include "portable/util/crc/include/Crc16ccitt1021.h"

namespace util
  {
    namespace crc
      {
        Crc16ccitt1021::Crc16ccitt1021()
          {
            OSDeviceDebug::putConstructor_P(PSTR("Crc16ccitt1021"), this);
          }

        Crc16ccitt1021::~Crc16ccitt1021()
          {
            OSDeviceDebug::putDestructor_P(PSTR("Crc16ccitt1021"), this);
          }

        // ----- Static methods -----------------------------------------------

        void Crc16ccitt1021::compute(uint16_t& crc, uint8_t b)
          {
            int i;
            uint16_t _crc;

            _crc = crc ^ ((uint16_t) b << 8);
            for (i = 0; i < 8; i++)
              {
                if (_crc & 0x8000)
                _crc = (_crc << 1) ^ 0x1021;
                else
                _crc <<= 1;
              }

            // Update reference to return new computed value
            crc = _crc;
          }

        void Crc16ccitt1021::compute(uint16_t& crc, uint8_t* pb, std::sizeof_t n)
          {
            int i;
            for (i = 0; i < n; ++i)
              {
                compute(crc, *pb++);
              }
          }
      }
  }

#endif /* defined(OS_INCLUDE_UTIL_CRC_CRC16CCITT1021) */
