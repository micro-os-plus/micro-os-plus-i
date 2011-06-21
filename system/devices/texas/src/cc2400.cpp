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

    void
    Cc2400::PowerOn(void)
    {
//      Turn on power supplies, by setting MDM_PWR_EN_IO, MDM_PWR_EN_CORE and PA_PWR_EN

//      Wait 150usec (while stabilising power supplies)

//      Send MAIN register, having set the flags: RESET and XOSC16M_BYPASS set (value 0x8003)

//      Set the following configuration register:
//      FSCTRL = 0x0010, default value
//      FSDIV = 2401, set on first channel 2401Mhz
//      MDMCTRL = 0x0040, frequency deviation of 250kHz
//      AGCCTRL unchanged
//      FREND = 0x0008, minimum output power: -25dBm
//      RSSI = 0x00F3, set to maximum (8) the number of samples that are used to calculate the average signal amplitude; on
//      FREQEST read only
//      IOCFG = 0xF960, configure GIO6 pin with LOCK_STATUS function, and GIO1 with CRC_OK function
//      FSMTC unchanged
//      MANAND = 0xFFFF
//      FSMSTATE unchanged
//      ADCTST read only
//      RXBPFTST unchanged
//      PAMTST = 0x0803, as in TI's example
//      LMTST = 0x2B22, default value, also as in TI's example
//      MANOR unchanged
//      MDMTST0 =0x134B, as in TI's example
//      MDMTST1 = 0x004B, as in TI's example
//      DACTST = 0, default value as in TI's example
//      AGCTST0 unchanged
//      AGCTST1 unchanged
//      AGCTST2 unchanged
//      FSTST0 = 0xA210, as in TI's example, default value is 0xA200
//      FSTST1 = 0x1002, default value as in TI's example
//      FSTST2 = 0x0600, default value
//      FSTST3 = 0x02CC, default value
//      MANFIDL unchanged
//      MANFIDH unchanged
//      GRMDM = 0x0F71, set buffered mode, 32 preamble bytes, 4 SYNC bytes, CRC generation and checking ok, NRZ line-coding format, GFSK modulation
//      GRDEC unchanged
//      PKTSTATUS read only
//      INT = 30, default value
//      SYNCL unchanged
//      SYNCH unchanged
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

      // clearFifo();

      // Set FIFO_THRESHOLD by sending on SPI register INT with the value of 16

      // returnVal = calibrate();
      // if returnVal < 0 log "Calibration Failed"; exit

      // Set Power Amplifier in TX mode, by setting PAEN=1 and EN=0
      // Send on SPI (as a single word) the address of FIFO register (1byte) and leave CS asserted
      // Prepare PDCA transfer
      // Set Transmission mode, by resetting RX pin (leave TX pin set)
      // wait 200usec (time for preamble to be sent 32bytes x 8 usec=256usec and
        // sync word 4bytes x 8usec = 32usec)
      // start PDCA transfer
      // when PDCA transmission channel interrupt is raised,
      // means there are still some bytes(aprox 11 bytes = (256 + 32 - 200)/8) to be sent out on radio

      // Note: If we want to put inside the packet the timestamp of sending, then
      // the FIFO signal will fall after preamble was sent
      // so timestamp of sending the 16th byte(when FIFO signal in reception will fall)
      // can be computed and added
      //
      // TODO: do implementation
    }

    // Start transmission of the packet,
    // already recorded using rpcSetRadioPacketTx.
    void
    Cc2400::rpcRadioStartRx(uint16_t frecv, bool isLnaOn, uint16_t packetSize)
    {
      // IOCFG = 0xF960, configure GIO6 pin with LOCK_STATUS function, and GIO1 with CRC_OK function

      // Set frequency by sending on SPI register FSDIV with frequency-1 value
      // Clear FIFO by reading 32 bytes (in FIFO buffer could be unread bytes)

      // clearFifo();

      // Set FIFO_THRESHOLD by sending on SPI register INT with the value of 16

      // returnVal = calibrate();
      // if returnVal < 0 log "Calibration Failed"; exit

      // Prepare PDCA transfer for (packetSize-30) bytes

      // Set Power Amplifier in RX mode, by setting pins PAEN = 0, EN = 1 and HGM = isLnaOn

      // Set Reception mode, by resetting TX pin (leave RX pin set)

      // (in about 415usec) wait for FIFO pin to go low (meaning modem received 16 data bytes)
      //        this moment "must" be compared against the timestamp from the packet

      // read RSSI value, by reading register RSSI

      // Send on SPI (as a single word) the address of FIFO register (1byte) and leave CS asserted
      // Start PDCA transfer (so PDCA transfer started when (about) 20bytes are already received in FIFO buffer)

      // if PDCA interrupt is raised (means there are still (32-20) data bytes to be received on radio

      // (turn off SPI's chip select signal)
      // read RSSI value, by reading register RSSI

      // read the last 32 bytes of data (including the last 2 CRC bytes)
      // check if CRC_OK signal(GIO1pin) is set

      // TODO: do implementation
    }

    void
    Cc2400::clearFifoBuffer()
    {
      // Clear FIFO by reading 32 bytes (in FIFO buffer could be unread bytes)
      //        this could take at 16Mhz, 0.5usec/byte -> 16 usec

    }

    uint8_t
    Cc2400::calibrate(void)
    {
      // Start calibration and PLL lock by setting both micro-controller pins connected to RX and TX Modem pins

      // Wait until GIO6 is set (max 100usec)
      // if GIO6 not set return -1;//error
      return 0;
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
