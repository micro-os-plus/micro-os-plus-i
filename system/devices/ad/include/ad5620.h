/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICE_AD_AD5620_H_
#define DEVICE_AD_AD5620_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Usart.h"
#include "portable/devices/generic/include/ChipSelectActiveLow.h"

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

namespace device
{
  namespace ad
  {
    namespace ad5620
    {
      // ----- Type definitions -----------------------------------------------

      typedef ChipSelectActiveLow ChipSelectActiveLow_t;

      typedef uint16_t Value_t;
      typedef int16_t SignedValue_t;

      class Value
      {
      public:
        const static Value_t VALIDATION_MASK = 0x0FFF;
        const static uint_t SHIFT_BITS = 2;

        const static Value_t MIN = 0;
        const static Value_t MAX = VALIDATION_MASK;
        const static Value_t MIDDLE = (MIN + ((MAX - MIN) / 2));

        const static SignedValue_t SIGNED_MIN = MIN - MIDDLE;
        const static SignedValue_t SIGNED_MAX = MAX - MIDDLE;

      };

      typedef uint16_t PowerDownMode_t;

      class PowerDownMode
      {
      public:
        const static uint_t SHIFT_BITS = 14;

        const static PowerDownMode_t NORMAL_OPERATION = (0x0 << SHIFT_BITS);
        const static PowerDownMode_t _1K_TO_GND = (0x1 << SHIFT_BITS);
        const static PowerDownMode_t _100K_TO_GND = (0x2 << SHIFT_BITS);
        const static PowerDownMode_t THREE_STATE = (0x3 << SHIFT_BITS);

        const static PowerDownMode_t MASK = (0x3 << SHIFT_BITS);
      };

      typedef uint32_t ClockFrequencyHz_t;

      class ClockFrequencyHz
      {
      public:
        const static ClockFrequencyHz_t MAX = 30000000L; // 30 MHz
      };

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

      typedef avr32::uc3::Gpio Gpio_t;
      typedef avr32::uc3::usart::SpiMaster SpiMaster_t;

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

    }

    class Ad5620
    {
      // ----------------------------------------------------------------------

    public:

      // ----- Constructors & Destructors -------------------------------------

      Ad5620(ad5620::ChipSelectActiveLow_t& cs, ad5620::SpiMaster_t& spi);
      ~Ad5620();

    public:

      // ----- Public methods -------------------------------------------------

      void
      powerUp(void);

      void
      powerDown(ad5620::PowerDownMode_t mode);

      void
      configureClockFrequencyHz(ad5620::ClockFrequencyHz_t clockFrequency);

      void
      writeValue(ad5620::Value_t value);

      OSReturn_t
      writeSignedValue(ad5620::SignedValue_t signedValue);

      ad5620::Value_t
      getPreviouslyWrittenValue(void);

      ad5620::SignedValue_t
      getPreviouslyWrittenSignedValue(void);

      // Also return new value
      ad5620::Value_t
      incrementValue(void);

      // Also return new value
      ad5620::Value_t
      decrementValue(void);

      ad5620::Value_t
      getMiddleValue(void);

      void
      setMiddleValue(ad5620::Value_t value);

      void
      setMinSignedValue(ad5620::SignedValue_t value);

      void
      setMaxSignedValue(ad5620::SignedValue_t value);

      // ----- Private members ------------------------------------------------

      ad5620::ChipSelectActiveLow_t& m_cs;
      ad5620::SpiMaster_t& m_spi;

      ad5620::Value_t m_shadowValue;
      ad5620::Value_t m_middleValue;

      ad5620::SignedValue_t m_minSignedValue;
      ad5620::SignedValue_t m_maxSignedValue;

      // ----------------------------------------------------------------------

    };

    inline ad5620::Value_t
    Ad5620::getMiddleValue(void)
    {
      return m_middleValue;
    }

    inline void
    Ad5620::setMinSignedValue(ad5620::SignedValue_t value)
    {
      m_minSignedValue = value;
    }

    inline void
    Ad5620::setMaxSignedValue(ad5620::SignedValue_t value)
    {
      m_maxSignedValue = value;
    }

    inline ad5620::Value_t
    Ad5620::getPreviouslyWrittenValue(void)
    {
      return m_shadowValue;
    }

  }
}

#endif /* DEVICE_AD_AD5620_H_ */
