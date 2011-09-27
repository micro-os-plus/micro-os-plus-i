/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_CRC_CRC7X09_H_
#define UTIL_CRC_CRC7X09_H_

#include "portable/kernel/include/OS.h"

namespace util
{
  namespace crc
  {
    // Polynomial: x^7 + x^3 + 1 (0x42)
    // Used in SD/MMC commands

    class Crc7x42
    {
    public:
      // ----- Type definitions -----------------------------------------------

      typedef uint8_t Value_t;

      // ----- Constants ------------------------------------------------------
      const static Value_t INITIAL_VALUE = 0x00;
      const static Value_t FINAL_VALUE = 0x00;

      // ----- Constructors and destructors -----------------------------------
      Crc7x42();
      ~Crc7x42();

      // ---- Object methods --------------------------------------------------
      void
      setInitialValue(void);
      void
      computePolynomial(uint8_t b);
      void
      computePolynomial(uint8_t* pb, std::size_t n);
      Value_t
      getComputedValue(void);
      bool
      isFinalValueCorrect(void);

      // ---- Static methods --------------------------------------------------
      static void
      computePolynomial(Value_t& crc, uint8_t b);
      static void
      computePolynomial(Value_t& crc, uint8_t* pb, std::size_t n);
      static bool
      isFinalValueCorrect(Value_t crc);

    private:
      Value_t m_crc;
    };

    // ----- Static inline methods --------------------------------------------

    inline bool
    Crc7x42::isFinalValueCorrect(Value_t crc)
    {
      return (crc == FINAL_VALUE);
    }

    // ----- Object inline methods --------------------------------------------

    inline void
    Crc7x42::setInitialValue(void)
    {
      m_crc = INITIAL_VALUE;
    }

    inline void
    Crc7x42::computePolynomial(uint8_t b)
    {
      computePolynomial(m_crc, b);
    }

    inline void
    Crc7x42::computePolynomial(uint8_t* pb, std::size_t n)
    {
      computePolynomial(m_crc, pb, n);
    }

    inline Crc7x42::Value_t
    Crc7x42::getComputedValue(void)
    {
      return m_crc;
    }

    inline bool
    Crc7x42::isFinalValueCorrect(void)
    {
      return isFinalValueCorrect(m_crc);
    }

    // ----- Static methods ---------------------------------------------------

  }
}

#endif /* UTIL_CRC_CRC7X09_H_ */
