/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICE_TEXAS_CC2400)

#include "devices/texas/include/cc2400.h"

namespace device
{
  namespace texas
  {
    namespace cc2400
    {
      Registers::Registers(spim_t& spi) :
        m_spi(spi)
      {
        OSDeviceDebug::putConstructor("device::texas::cc2400::Registers", this);
      }

      Status_t
      Registers::strobe(RegisterId_t reg)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | Operation::WRITE);

        return status;
      }

      Status_t
      Registers::writeWord(RegisterId_t reg, uint16_t value)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | Operation::WRITE);
        m_spi.writeWaitReadByte(value >> 8);
        m_spi.writeWaitReadByte(value & 0xFF);

        return status;
      }

      Status_t
      Registers::readWord(RegisterId_t reg, uint16_t* pValue)
      {
        Status_t status;
        uint16_t value;

        status = m_spi.writeWaitReadByte(reg | Operation::READ);

        value = (m_spi.writeWaitReadByte(0xFF) << 8);
        value |= m_spi.writeWaitReadByte(0xFF);

        if (pValue != 0)
          *pValue = value;

        return status;
      }

      Status_t
      Registers::readByte(RegisterId_t reg, uint8_t* pValue)
      {
        Status_t status;
        uint8_t value;

        status = m_spi.writeWaitReadByte(reg | Operation::READ);

        value = m_spi.writeWaitReadByte(0xFF);

        if (pValue != 0)
          *pValue = value;

        return status;
      }

      Status_t
      Registers::writeByte(RegisterId_t reg, uint8_t value)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | Operation::WRITE);
        m_spi.writeWaitReadByte(value);

        return status;
      }

      Status_t
      Registers::prepareReadWordContinuously(RegisterId_t reg)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | Operation::READ);

        return status;
      }

      Status_t
      Registers::prepareWriteWordContinuously(RegisterId_t reg)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | Operation::WRITE);

        return status;
      }

      uint16_t
      Registers::readWordContinuously(void)
      {
        uint16_t value;

        value = (m_spi.writeWaitReadByte(0xFF) << 8);
        value |= m_spi.writeWaitReadByte(0xFF);

        return value;
      }

      void
      Registers::writeWordContinuously(uint16_t value)
      {
        m_spi.writeWaitReadByte(value >> 8);
        m_spi.writeWaitReadByte(value & 0xFF);
      }

      Status_t
      Registers::prepareReadFifoContinuously(void)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(Register::FIFOREG | Operation::READ);

        return status;
      }

      Status_t
      Registers::prepareWriteFifoContinuously(void)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(Register::FIFOREG | Operation::WRITE);

        return status;
      }

      uint8_t
      Registers::readByteContinuously(void)
      {
        return m_spi.writeWaitReadByte(0xFF);
      }

      void
      Registers::writeByteContinuously(uint8_t value)
      {
        m_spi.writeWaitReadByte(value);
      }
    }

    // ----- ------------------------------------------------------------------

    Cc2400::Cc2400(cc2400::spim_t& spi) :
      registers(spi)
    {
      OSDeviceDebug::putConstructor("device::texas::Cc2400", this);
    }


    // TODO: add other functions
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_TEXAS_CC2400) */
