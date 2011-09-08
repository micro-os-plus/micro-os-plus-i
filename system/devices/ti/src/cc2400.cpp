/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICE_TI_CC2400)

#include "devices/ti/include/cc2400.h"

namespace device
{
  namespace ti
  {
    namespace cc2400
    {
      Registers::Registers(spim_t& spi) :
        m_spi(spi)
      {
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
        OSDeviceDebug::putConstructor("device::ti::cc2400::Registers", this);
#endif
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

    Cc2400::Cc2400(cc2400::spim_t& spi, ChipSelectActiveLow& cs,
        avr32::uc3::Gpio& mdmClkEn, avr32::uc3::Gpio& mdmGio1,
        avr32::uc3::Gpio& mdmGio6, avr32::uc3::Gpio& mdmRxStrb,
        avr32::uc3::Gpio& mdmTxStrb, avr32::uc3::Gpio& mdmPktRdy,
        avr32::uc3::Gpio& mdmFifoRdy, avr32::uc3::Gpio& mdmPwrEnIo,
        avr32::uc3::Gpio& mdmPwrEnCore) :
      registers(spi), m_cs(cs), m_mdmClkEn(mdmClkEn), m_mdmGio1(mdmGio1),
          m_mdmGio6(mdmGio6), m_mdmRxStrb(mdmRxStrb), m_mdmTxStrb(mdmTxStrb),
          m_mdmPktRdy(mdmPktRdy), m_mdmFifoRdy(mdmFifoRdy), m_mdmPwrEnIo(
              mdmPwrEnIo), m_mdmPwrEnCore(mdmPwrEnCore)
    {
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putConstructor("device::ti::Cc2400", this);
#endif
    }

    void
    Cc2400::powerOnPhase1(void)
    {
      // make sure RX and TX pins are low
      m_mdmRxStrb.setPinLow();
      m_mdmTxStrb.setPinLow();

      // Turn on power supplies, by setting MDM_PWR_EN_IO, MDM_PWR_EN_CORE and PA_PWR_EN
      m_mdmPwrEnCore.setPinHigh();
      m_mdmPwrEnIo.setPinHigh();
    }
    void
    Cc2400::powerOnPhase2(void)
    {
      // Send MAIN register, having set the flags: RESET and XOSC16M_BYPASS set (value 0x8003)
      uint16_t mainValue;
      mainValue = 0x8003;

      registers.setSpiHighSpeed();
      device::ti::cc2400::Status_t status;

#if false

      // code for testing SPI communication between MCU and Radio

      uint16_t rssi;

      //device::ti::cc2400::RegisterId_t reg = device::ti::cc2400::Register::RSSI;
      device::ti::cc2400::RegisterId_t reg = device::ti::cc2400::Register::GRMDM;

      m_cs.assert();
      status = registers.readWord(reg, &rssi);
      m_cs.deassert();
      OSDeviceDebug::putString("st: ");
      OSDeviceDebug::putDec((uint16_t) status);
      OSDeviceDebug::putNewLine();
      OSDeviceDebug::putString("1: ");
      OSDeviceDebug::putDec((uint16_t) rssi);
      OSDeviceDebug::putNewLine();

      m_cs.assert();
      status = registers.writeWord(reg, 0x5A5A);
      m_cs.deassert();
      OSDeviceDebug::putString("st: ");
      OSDeviceDebug::putDec((uint16_t) status);
      OSDeviceDebug::putNewLine();

      m_cs.assert();
      status = registers.readWord(reg, &rssi);
      m_cs.deassert();
      OSDeviceDebug::putString("st: ");
      OSDeviceDebug::putDec((uint16_t) status);
      OSDeviceDebug::putNewLine();
      OSDeviceDebug::putString("3: ");
      OSDeviceDebug::putDec((uint16_t) rssi);
      OSDeviceDebug::putNewLine();
#endif

      status = writeRegUseCs(device::ti::cc2400::Register::MAIN, 0x0000);
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("status reset: ");
      OSDeviceDebug::putDec((uint16_t) status);
      OSDeviceDebug::putNewLine();
#endif

      status = writeRegUseCs(device::ti::cc2400::Register::MAIN, 0x8000);
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("status reset: ");
      OSDeviceDebug::putDec((uint16_t) status);
      OSDeviceDebug::putNewLine();
      readFsmState(status);
#endif
    }

    void
    Cc2400::powerOff(void)
    {
      device::ti::cc2400::Status_t status;

      // make sure RX and TX pins are low
      m_mdmTxStrb.setPinLow();
      m_mdmRxStrb.setPinLow();

      registers.setSpiHighSpeed();

      // send SRFOFF, just to make sure
      readFsmState(status);
      if ((status & device::ti::cc2400::Status::FS_LOCK) != 0)
        {
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
          OSDeviceDebug::putString("SRFOFF");
#endif
          m_cs.assert();
            {
              registers.strobe(device::ti::cc2400::Register::SRFOFF);
            }
          m_cs.deassert();
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
          OSDeviceDebug::putString("wait...");
#endif
          do
            {
              m_cs.assert();
                {
                  readFsmState(status);
                }
              m_cs.deassert();
            }
          while ((status & device::ti::cc2400::Status::FS_LOCK) != 0);
        }

#if false
      // can not stop XOSC;
      readFsmState(status);
      if ((status & device::ti::cc2400::Status::XOSC16M_STABLE) != 0)
        {
          OSDeviceDebug::putString("SXOSCOFF");
          m_cs.assert();
            {
              registers.strobe(device::ti::cc2400::Register::SXOSCOFF);
            }
          m_cs.deassert();
          OSDeviceDebug::putString("wait...");
          do
            {
              m_cs.assert();
                {
                  readFsmState(status);
                }
              m_cs.deassert();
            }
          while ((status & device::ti::cc2400::Status::XOSC16M_STABLE) != 0);
        }
#endif

#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString(" done");
#endif
      // Turn off power supplies, by setting MDM_PWR_EN_IO, MDM_PWR_EN_CORE and PA_PWR_EN
      m_mdmPwrEnIo.setPinLow();
      m_mdmPwrEnCore.setPinLow();
    }

    void
    Cc2400::configureRegisters(void)
    {
      device::ti::cc2400::Status_t status;
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("configure Registers");
      OSDeviceDebug::putNewLine();
#endif
      // set max speed
      registers.setSpiHighSpeed();

      //      Set the following configuration registers:
      //      MAIN = 0x001, XOSC16M_EN
      //status = writeRegUseCs(device::ti::cc2400::Register::MAIN,
      //    (uint16_t) 0x0001);
      //      FSCTRL = 0x0010, default value
      status = writeRegUseCs(device::ti::cc2400::Register::FSCTRL,
          (uint16_t) 0x0010);
      //      FSDIV = 2401, set on first channel 2401Mhz
      status = writeRegUseCs(device::ti::cc2400::Register::FSDIV,
          (uint16_t) 2401);
      //      MDMCTRL = 0x0040, frequency deviation of 250kHz
      status = writeRegUseCs(device::ti::cc2400::Register::MDMCTRL,
          (uint16_t) 0x0040);
      //      AGCCTRL unchanged
      //      FREND = 0x000F, max output power
      status = writeRegUseCs(device::ti::cc2400::Register::FREND,
          (uint16_t) 0x000F);
      //      RSSI = 0x7FF2, set to 4 bits that are used to calculate the average RSSI
      status = writeRegUseCs(device::ti::cc2400::Register::RSSI,
          (uint16_t) 0x7FF3);
      //      FREQEST read only
      //      IOCFG = 0x17E0, configure GIO1 pin with LOCK_STATUS function,
      // and GIO6 with CRC_OK function
      //      IOCFG = 0x7858, configure GIO6 pin with LOCK_STATUS function,
      // and GIO1 with CRC_OK function
      status = writeRegUseCs(device::ti::cc2400::Register::IOCFG,
          (uint16_t) 0x17E0);
      //      FSMTC = 0x7A94 (default value)
      status = writeRegUseCs(device::ti::cc2400::Register::FSMTC,
          (uint16_t) 0x7A94);
      //      MANAND = 0x7FFF (default value, except first bit,
      // which must be set 0 during initialization)
      status = writeRegUseCs(device::ti::cc2400::Register::MANAND,
          (uint16_t) 0x7FFF);
      //      FSMSTATE unchanged
      //      ADCTST read only
      //      RXBPFTST unchanged; set to 0x5335, by ???
      //      PAMTST = 0x0803, as in TI's example
      status = writeRegUseCs(device::ti::cc2400::Register::PAMTST,
          (uint16_t) 0x0803);
      //      LMTST = 0x2B22, default value, also as in TI's example
      // set to 0x2BA3 by ???
      status = writeRegUseCs(device::ti::cc2400::Register::LMTST,
          (uint16_t) 0x2B22);
      //      MANOR unchanged
      //      MDMTST0 = 0x134B, as in TI's example
      status = writeRegUseCs(device::ti::cc2400::Register::MDMTST0,
          (uint16_t) 0x134B);
      //      MDMTST1 = 0x004B, as in TI's example
      status = writeRegUseCs(device::ti::cc2400::Register::MDMTST1,
          (uint16_t) 0x004B);
      //      DACTST = 0, default value as in TI's example
      status = writeRegUseCs(device::ti::cc2400::Register::DACTST,
          (uint16_t) 0x0000);
      //      AGCTST0 unchanged
      //      AGCTST1 unchanged
      //      AGCTST2 unchanged
      //      FSTST0 = 0xA210, as in TI's example, default value is 0xA210
      status = writeRegUseCs(device::ti::cc2400::Register::FSTST0,
          (uint16_t) 0xA210);
      //      FSTST1 = 0x1002, default value as in TI's example
      status = writeRegUseCs(device::ti::cc2400::Register::FSTST1,
          (uint16_t) 0x1002);
      //      FSTST2 = 0x0600, default value
      //      FSTST3 = 0x02CC, default value
      //      MANFIDL unchanged
      //      MANFIDH unchanged
      //      GRMDM = 0x0F70, set buffered mode, 32 preamble bytes, 4 SYNC bytes,
      // CRC generation and checking ok,
      // NRZ line-coding format, GFSK modulation, no Gaussian filtered
      status = writeRegUseCs(device::ti::cc2400::Register::GRMDM,
          (uint16_t) 0x0F70);
      //      GRDEC unchanged
      //      PKTSTATUS read only
      //      INT = 30, default value
      status = writeRegUseCs(device::ti::cc2400::Register::INT, (uint16_t) 16);

      // Wait until crystal oscillator is stable

      readFsmState(status);
      m_cs.assert();
        {
          registers.strobe(device::ti::cc2400::Register::SXOSCON);
        }
      m_cs.deassert();
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("wait osc...");
#endif
      do
        {
          readFsmState(status);
        }
      while ((status & device::ti::cc2400::Status::XOSC16M_STABLE) == 0);
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString(" conf reg done");
      OSDeviceDebug::putNewLine();
      readFsmState(status);
#endif
    }

    void
    Cc2400::clearFifoBuffer()
    {
      // Clear FIFO by reading 32 bytes (in FIFO buffer could be unread bytes)
      //        this could take at 16Mhz, 0.5usec/byte -> 16 usec
      m_cs.assert();
        {
          registers.prepareReadFifoContinuously();
          for (int i = 0; i < 32; i++)
            {
              registers.readByteContinuously();
            }
        }
      m_cs.deassert();
    }

    bool
    Cc2400::isCalibrationOk(bool isDebug = true)
    {
      device::ti::cc2400::Status_t status;
      // Start calibration and PLL lock by setting both micro-controller pins
      // connected to RX and TX Modem pins
      m_mdmTxStrb.setPinLow();
      m_mdmRxStrb.setPinLow();

      //#ifdef APP_CFGINT_NODERADIO_DEBUG
      if (isDebug)
        readFsmState(status);

      //  m_cs.assert();
      //    {
      //      status = registers.strobe(device::ti::cc2400::Register::SFSON);
      //    }
      //  m_cs.deassert();

      m_mdmTxStrb.setPinHigh();
      m_mdmRxStrb.setPinHigh();

      if (isDebug)
        {
          OSDeviceDebug::putString("wait calib, st:");
          OSDeviceDebug::putHex((uint16_t) status);
          OSDeviceDebug::putNewLine();

        }
      // Wait until GIO1 is set (max 100usec)
      while (!m_mdmGio1.isPinHigh())
        ;
      //  do
      //    {
      //      readFsmState(status);
      //    }
      //  while (0 == (status & 0x04));;
      //readFsmState();
      // if GIO1 not set return error
      if (m_mdmGio1.isPinHigh())
        {
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
          OSDeviceDebug::putString("calib ok");
          OSDeviceDebug::putNewLine();
#endif
          return true;
        }
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("calib failed");
      OSDeviceDebug::putNewLine();
#endif
      return false;
    }

    // returns value of last RSSI measured by modem in RX mode
    int16_t
    Cc2400::readRssiValue(void)
    {
      device::ti::cc2400::Status_t status;
      uint16_t aux;

      m_cs.assert();
        {

          status = registers.readWord(device::ti::cc2400::Register::RSSI, &aux);
        }
      m_cs.deassert();

      int8_t aux8;
      aux8 = (int8_t) (aux >> 8);

      int16_t rssiValue;
      rssiValue = aux8;

#if false
      OSDeviceDebug::putString("reg:");
      OSDeviceDebug::putHex((uint16_t) aux);
      OSDeviceDebug::putString(" 1:");
      OSDeviceDebug::putHex((uint16_t) aux8);
      OSDeviceDebug::putString(" 2:");
      OSDeviceDebug::putHex((uint16_t) rssiValue);
#endif

      rssiValue += RSSI_OFFSET;

#if false
      OSDeviceDebug::putString(" 3:");
      OSDeviceDebug::putHex((uint16_t) rssiValue);
      OSDeviceDebug::putNewLine();
#endif
      return rssiValue;
    }

    uint16_t
    Cc2400::readFsmState(device::ti::cc2400::Status_t &status)
    {
      uint16_t fsm;
      //device::ti::cc2400::Status_t status;

      m_cs.assert();
        {
          status = registers.readWord(device::ti::cc2400::Register::FSMSTATE,
              &fsm);
        }
      m_cs.deassert();

      fsm = fsm & 0x1F;
#if defined(OS_DEBUG_DEVICE_TI_CC2400)
      OSDeviceDebug::putString("fsm:");
      OSDeviceDebug::putHex((uint16_t) fsm);
      OSDeviceDebug::putNewLine();
      OSDeviceDebug::putString("status:");
      OSDeviceDebug::putHex((uint8_t) status);
      OSDeviceDebug::putNewLine();
#endif
      return fsm;
    }

    void
    Cc2400::prepareTx(uint16_t frecv,
        device::ti::cc2400::TxPowerLevel_t txPowerLevel)
    {
      device::ti::cc2400::Status_t status;

      // Set frequency by sending on SPI register FSDIV with frequency value

      status = writeRegUseCs(device::ti::cc2400::Register::FSDIV, frecv);

      // Set output power level by sending on SPI register FREND with txPowerLevel value
      status = writeRegUseCs(device::ti::cc2400::Register::FREND,
          (uint16_t) txPowerLevel);

      // Set FIFO_THRESHOLD by sending on SPI register INT with the value of 16
      //status = writeRegUseCs(device::ti::cc2400::Register::INT, (uint16_t) 16);

    }

    void
    Cc2400::prepareRx(uint16_t frecv)
    {
      device::ti::cc2400::Status_t status;
      // IOCFG = 0x17E0, configure GIO1 pin with LOCK_STATUS function, and GIO6 with CRC_OK function
      // already configured that in NodeRadio::configureRegisters(void)

      // Set frequency by sending on SPI register FSDIV with frequency-1 value
      status = writeRegUseCs(device::ti::cc2400::Register::FSDIV, frecv - 1);

      // Set FIFO_THRESHOLD by sending on SPI register INT with the value of 16
      // already set in configureRegisters()
      //status = writeRegUseCs(device::ti::cc2400::Register::INT, (uint16_t) 16);
    }

    void
    Cc2400::enableFifoInterrupt(avr32::uc3::intc::InterruptHandler_t handler,
        avr32::uc3::gpio::InterruptMode_t mode)
    {

      m_mdmFifoRdy.registerInterruptHandler(handler);
      m_mdmFifoRdy.configureInterruptMode(mode);
      m_mdmFifoRdy.enableInterrupt();
    }

    void
    Cc2400::enablePktInterrupt(avr32::uc3::intc::InterruptHandler_t handler,
        avr32::uc3::gpio::InterruptMode_t mode)
    {

      m_mdmPktRdy.registerInterruptHandler(handler);
      m_mdmPktRdy.configureInterruptMode(mode);
      m_mdmPktRdy.enableInterrupt();
    }

    //  assert CS, writes a CC2400 register, de-assert CS
    device::ti::cc2400::Status_t
    Cc2400::writeRegUseCs(device::ti::cc2400::RegisterId_t reg, uint16_t value)
    {
      device::ti::cc2400::Status_t status;
      m_cs.assert();
        {
          status = registers.writeWord(reg, value);
        }
      m_cs.deassert();
      return status;
    }
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_TI_CC2400) */
