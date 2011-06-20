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

        status = m_spi.writeWaitReadByte(reg | OPERATION::WRITE);

        return status;
      }

      Status_t
      Registers::writeWord(RegisterId_t reg, uint16_t value)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | OPERATION::WRITE);
        m_spi.writeWaitReadByte(value >> 8);
        m_spi.writeWaitReadByte(value & 0xFF);

        return status;
      }

      Status_t
      Registers::readWord(RegisterId_t reg, uint16_t* pValue)
      {
        Status_t status;
        uint16_t value;

        status = m_spi.writeWaitReadByte(reg | OPERATION::READ);

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

        status = m_spi.writeWaitReadByte(reg | OPERATION::READ);

        value = m_spi.writeWaitReadByte(0xFF);

        if (pValue != 0)
          *pValue = value;

        return status;
      }

      Status_t
      Registers::writeByte(RegisterId_t reg, uint8_t value)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | OPERATION::WRITE);
        m_spi.writeWaitReadByte(value);

        return status;
      }

      Status_t
      Registers::prepareReadWordContinuously(RegisterId_t reg)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | OPERATION::READ);

        return status;
      }

      Status_t
      Registers::prepareWriteWordContinuously(RegisterId_t reg)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(reg | OPERATION::WRITE);

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

        status = m_spi.writeWaitReadByte(REGISTER::FIFOREG | OPERATION::READ);

        return status;
      }

      Status_t
      Registers::prepareWriteFifoContinuously(void)
      {
        Status_t status;

        status = m_spi.writeWaitReadByte(REGISTER::FIFOREG | OPERATION::WRITE);

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

    // Set the packet which is going to be used in
    // the following transmission test
    void
    Cc2400::rpcSetRadioPacketTx(uint16_t packetSize, uint16_t *packet)
    {
      // record packet and packetSize
    }

    // Start transmission of the packet,
    // already recorded using rpcSetRadioPacketTx.
    void
    Cc2400::rpcRadioStartTx(uint16_t frecv, cc2400::TxPowerLevel_t txPowerLevel)
    {
      // IOCFG = 0xF960, configure GIO6 pin with LOCK_STATUS function, and GIO1 with CRC_OK function

      // Set frequency by sending on SPI register FSDIV with frequency value

      // Set output power level by sending on SPI register FREND with txPowerLevel value

      // Clear FIFO by reading 32 bytes at max value

      // Set FIFO_THRESHOLD by sending on SPI register INT with the value of 16

      // Start calibration and PLL lock by setting both micro-controller pins connected to RX and TX Modem pins

      // Wait until GIO6 is set (max 100usec)
      // if GIO6 not set log "Calibration Failed"; exit

      // Set Power Amplifier in TX mode, by setting PAEN=1 and EN=0
      // Send on SPI (as a single word) the address of FIFO register (1byte) and leave CS asserted
      // Prepare PDCA transfer
      // Set Transmission mode, by resetting RX pin
      // wait 200usec (time preamble to be sent 32bytes x 8 usec=256usec and
        // sync word 4bytes x 8usec = 32usec
      // start PDCA transfer

      // TODO: finish design and implementation
    }

    // Start transmission of the packet,
    // already recorded using rpcSetRadioPacketTx.
    void
    Cc2400::rpcRadioStartRx(uint16_t frecv, bool isLnaOn, uint16_t packetSize)
    {
      // TODO: finish design and implementation
    }

    // Requests the packet received in the last reception phase
    void
    Cc2400::rpcGetRadioPacketRx()
    {
      // TODO: finish design and implementation
    }

    // TODO: this is TCC Output Call
    // Send the packet requested by rpcGetRadioPacketRx.
    void
    Cc2400::rpcGetRadioPacketRx(uint16_t &packetSize, uint8_t *packet,
        int16_t &rssiValue, bool &isCrcOk)
    {
      // TODO: finish design and implementation
    }

    // TODO: add other functions
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_TEXAS_CC2400) */
