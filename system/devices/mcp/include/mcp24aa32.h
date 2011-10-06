/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICE_MCP_MCP24AA32_H_
#define DEVICE_MCP_MCP24AA32_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/Eeprom.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Twim.h"

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

namespace device
{
  namespace mcp
  {
    class Mcp24aa32 : public Eeprom
    {
    public:

      // ----- Public type definitions ----------------------------------------

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
      typedef avr32::uc3::Twim I2c_t;
      typedef avr32::uc3::Twim::Address_t I2cAddress_t;
#endif

      typedef int RetryLimit_t;

      // ----- Constructors & destructors -------------------------------------

      Mcp24aa32(I2c_t& device);
      ~Mcp24aa32();

      // ----- Public methods -------------------------------------------------

      void
      setI2cAddress(I2cAddress_t i2cAddress);

      I2cAddress_t
      getI2cAddress(void);

      virtual void
      powerUp(void);

      virtual void
      powerDown(void);

      virtual void
      initialise(void);

      virtual OSReturn_t
      probeDevice(void);

      virtual OSReturn_t
      readByteArray(Eeprom::Address_t address, uint8_t* pIncomingBytes,
          BytesCount_t incomingBytesSize, size_t* pIncomingBytesLength);

      virtual OSReturn_t
      writeByteArray(Address_t address, uint8_t* pOutgoingBytes,
          BytesCount_t outgoingBytesLength);

      void
      setRetryLimit(RetryLimit_t retryLimit);

      RetryLimit_t
      getRetryLimit(void);

    private:

      // ---- Private members -------------------------------------------------

      I2c_t& m_i2cDevice;
      I2cAddress_t m_i2cAddress;

      RetryLimit_t m_retryLimit;

      // ----------------------------------------------------------------------

    };

    // ----- Inline methods ---------------------------------------------------

    inline void
    Mcp24aa32::setI2cAddress(I2cAddress_t i2cAddress)
    {
      m_i2cAddress = i2cAddress;
    }

    inline Mcp24aa32::I2cAddress_t
    Mcp24aa32::getI2cAddress(void)
    {
      return m_i2cAddress;
    }

    inline void
    Mcp24aa32::setRetryLimit(RetryLimit_t retryLimit)
    {
      m_retryLimit = retryLimit;
    }

    inline Mcp24aa32::RetryLimit_t
    Mcp24aa32::getRetryLimit(void)
    {
      return m_retryLimit;
    }

  // ------------------------------------------------------------------------
  }
}

#endif /* DEVICE_MCP_MCP24AA32_H_ */
