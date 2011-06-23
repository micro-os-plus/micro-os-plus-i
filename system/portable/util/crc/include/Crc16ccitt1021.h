/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_CRC_CRC16CCITT1021_H_
#define UTIL_CRC_CRC16CCITT1021_H_

#include "portable/kernel/include/OS.h"

namespace util
{
  namespace crc
  {
    // Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
    // Initial value: 0x0

    class Crc16ccitt1021
    {
    public:
      Crc16ccitt1021();
      ~Crc16ccitt1021();

      static const uint16_t INITIAL_VALUE = 0x0000;
      static const uint16_t FINAL_VALUE = 0x0000;

      void
      setInitialValue(void);
      void
      compute(uint8_t b);
      void
      compute(uint8_t* pb, std::sizeof_t n);
      bool
      isCorrect(void);

      // ---- Static methods --------------------------------------------------
      static void
      compute(uint16_t& crc, uint8_t b);
      static void
      compute(uint16_t& crc, uint8_t* pb, std::sizeof_t n);
      static bool
      isCorrect(uint16_t crc);

    private:
      uint16_t m_crc;
    };

    // ----- Static inline methods --------------------------------------------

    inline bool
    Crc16ccitt1021::isCorrect(uint16_t crc)
    {
      return (crc == FINAL_VALUE);
    }

    // ----- Object inline methods --------------------------------------------

    void
    Crc16ccitt1021::setInitialValue(void)
    {
      m_crc = INITIAL_VALUE;
    }

    void
    Crc16ccitt1021::compute(uint8_t b)
    {
      compute(m_crc, b);
    }

    void
    Crc16ccitt1021::compute(uint8_t* pb, std::sizeof_t n)
    {
      compute(m_crc, pb, n);
    }

    bool
    Crc16ccitt1021::isCorrect(void)
    {
      return isCorrect(m_crc);
    }

  }
}

#endif /* UTIL_CRC_CRC16CCITT1021_H_ */
