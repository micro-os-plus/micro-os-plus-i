/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICE_AD_AD5620)

#include "devices/ad/include/ad5620.h"

namespace device
{
  namespace ad
  {

    // ----- Constructors & Destructors ---------------------------------------

    Ad5620::Ad5620(ad5620::ChipSelectActiveLow_t& cs, ad5620::SpiMaster_t& spi) :
      m_cs(cs), m_spi(spi)
    {
      OSDeviceDebug::putConstructor("device::ad::Ad5620", this);

      // In the manual there is a reference that some models start at
      // mid-value, but common ones start at zero.
      // Just set it explicitly to be sure.
#if defined(OS_CFGBOOL_DEVICE_AD_AD5620_START_WITH_MIDDLE_VALUE)
      m_shadowValue = ad5620::Value::MIDDLE;
#else
      m_shadowValue = ad5620::Value::MIN;
#endif /* defined(OS_CFGBOOL_DEVICE_AD_AD5620_START_WITH_MIDDLE_VALUE) */
    }

    Ad5620::~Ad5620()
    {
      OSDeviceDebug::putDestructor("device::ad::Ad5620", this);
    }

    // ----- Public methods ---------------------------------------------------

    void
    Ad5620::powerUp(void)
    {
      m_cs.powerUp();
      m_spi.powerUp();

      // Start with the maximum clock allowed by the device
      m_spi.configureClockFrequencyHz(ad5620::ClockFrequencyHz::MAX);

      // SPI Bus Protocol Mode 1 (CPOL=0, CPHA=0)
      m_spi.configurePolarityAndPhase(0, 0);

      // Start with the previous value (MIDDLE value at start-up)
      writeValue(m_shadowValue);
    }

    void
    Ad5620::powerDown(ad5620::PowerDownMode_t mode)
    {
      // The chip has two bits in the value register to
      // configure the behaviour of the output line in power down mode
      m_cs.assert();
        {
          ad5620::Value_t tmp;
          tmp = (m_shadowValue << ad5620::Value::SHIFT_BITS) | (mode
              & ad5620::PowerDownMode::MASK);

          m_spi.writeWaitReadByte(tmp >> 8);
          m_spi.writeWaitReadByte(tmp & 0xFF);
        }
      m_cs.deassert();

      m_spi.powerDown();
      m_cs.powerDown();
    }

    void
    Ad5620::configureClockFrequencyHz(
        ad5620::ClockFrequencyHz_t clockFrequencyHz)
    {
      m_spi.configureClockFrequencyHz(clockFrequencyHz);
    }

    OSReturn_t
    Ad5620::writeSignedValue(ad5620::SignedValue_t signedValue)
    {
      OSReturn_t ret;
      ret = OSReturn::OS_OK;

      if (signedValue < ad5620::Value::SIGNED_MIN)
        {
          signedValue = ad5620::Value::SIGNED_MIN;
          ret = OSReturn::OS_OUT_OF_RANGE;
        }
      else if (signedValue > ad5620::Value::SIGNED_MAX)
        {
          signedValue = ad5620::Value::SIGNED_MAX;
          ret = OSReturn::OS_OUT_OF_RANGE;
        }

      ad5620::Value_t value;
      value = ad5620::Value::MIDDLE;

      value += signedValue;

      writeValue(value);

      return ret;
    }

    void
    Ad5620::writeValue(ad5620::Value_t value)
    {
      // Validate input value
      value = (value & ad5620::Value::VALIDATION_MASK);

      m_cs.assert();
        {
          ad5620::Value_t tmp;
          tmp = (value << ad5620::Value::SHIFT_BITS)
              | ad5620::PowerDownMode::NORMAL_OPERATION;

          m_spi.writeWaitReadByte(tmp >> 8);
          m_spi.writeWaitReadByte(tmp & 0xFF);
        }
      m_cs.deassert();

      // Store value for further usage
      m_shadowValue = value;
    }

    // Return the new value
    ad5620::Value_t
    Ad5620::incrementValue(void)
    {
      if (m_shadowValue < ad5620::Value::MAX)
        writeValue(m_shadowValue + 1);

      return m_shadowValue;
    }

    // Return the new value
    ad5620::Value_t
    Ad5620::decrementValue(void)
    {
      if (m_shadowValue > 0)
        writeValue(m_shadowValue - 1);

      return m_shadowValue;
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_DEVICE_AD_AD5620) */

