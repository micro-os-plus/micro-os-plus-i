/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_CRC_CRC16X1021_H_
#define UTIL_CRC_CRC16X1021_H_

#include "portable/kernel/include/OS.h"

namespace util
{
  namespace crc
  {
    // Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
    // Used in SD/MMC payload, XMODEM

    class Crc16x1021
    {
    public:
      // ----- Constants ------------------------------------------------------
      const static uint16_t INITIAL_VALUE = 0x0000;
      const static uint16_t FINAL_VALUE = 0x0000;

      // ----- Constructors and destructors -----------------------------------
      Crc16x1021();
      ~Crc16x1021();

      // ---- Object methods --------------------------------------------------
      void
      setInitialValue(void);
      void
      computePolynomial(uint8_t b);
      void
      computePolynomial(uint8_t* pb, std::size_t n);
      uint16_t
      getComputedValue(void);
      bool
      isFinalValueCorrect(void);

      // ---- Static methods --------------------------------------------------
      static void
      computePolynomial(uint16_t& crc, uint8_t b);
      static void
      computePolynomial(uint16_t& crc, uint8_t* pb, std::size_t n);
      static bool
      isFinalValueCorrect(uint16_t crc);

    private:
      uint16_t m_crc;
    };

    // ----- Static inline methods --------------------------------------------

    inline bool
    Crc16x1021::isFinalValueCorrect(uint16_t crc)
    {
      return (crc == FINAL_VALUE);
    }

    // ----- Object inline methods --------------------------------------------

    inline void
    Crc16x1021::setInitialValue(void)
    {
      m_crc = INITIAL_VALUE;
    }

    inline void
    Crc16x1021::computePolynomial(uint8_t b)
    {
      computePolynomial(m_crc, b);
    }

    inline void
    Crc16x1021::computePolynomial(uint8_t* pb, std::size_t n)
    {
      computePolynomial(m_crc, pb, n);
    }

    inline uint16_t
    Crc16x1021::getComputedValue(void)
    {
      return m_crc;
    }

    inline bool
    Crc16x1021::isFinalValueCorrect(void)
    {
      return isFinalValueCorrect(m_crc);
    }

    // ----- Static methods ---------------------------------------------------

  }
}

#endif /* UTIL_CRC_CRC16X1021_H_ */
