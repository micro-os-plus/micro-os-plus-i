/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICE_TEXAS_CC2400_H_
#define DEVICE_TEXAS_CC2400_H_

#include "portable/kernel/include/OS.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"
#include "portable/devices/generic/include/ChipSelectActiveLow.h"

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

namespace device
{
  namespace texas
  {
    namespace cc2400
    {
      //
#if defined(OS_CONFIG_FAMILY_AVR32UC3)
      typedef avr32::uc3::Spim spim_t;
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

      typedef uint8_t RegisterId_t;
      typedef uint8_t RegisterOperation_t;

      typedef uint8_t Status_t;
      typedef uint8_t TxPowerLevel_t;

      class TxPowerLevel
      {
      public:
        static const TxPowerLevel_t DBM_25 = 0; // -25 dBm
        static const TxPowerLevel_t DBM_15 = 1; // -15 dBm
        static const TxPowerLevel_t DBM_10 = 2; // -10 dBm
        static const TxPowerLevel_t DBM_7 = 3; // -7 dBm
        static const TxPowerLevel_t DBM_4_6 = 4; // -4.6 dBm
        static const TxPowerLevel_t DBM_2_8 = 5; // -2.8 dBm
        static const TxPowerLevel_t DBM_1_3 = 6; // -1.3 dBm
        static const TxPowerLevel_t DBM_0 = 7; // 0 dBm

      };
      class Status
      {
      public:
        static const Status_t XOSC16M_STABLE = 0x40; // Oscillator running
        static const Status_t SYNC_RECEIVED = 0x20; // RX sync word received
        static const Status_t CRC_OK = 0x08; // Next 2 FIFO bytes make CRC OK
        static const Status_t FS_LOCK = 0x04; // Freq. Synthesiser is in lock

        static bool
        isOscStable(Status_t status);
        static bool
        isSyncReceived(Status_t status);
        static bool
        isCrcOk(Status_t status);
        static bool
        isFsLock(Status_t status);
      };

      class Operation
      {
      public:
        static const RegisterOperation_t WRITE = 0x00;
        static const RegisterOperation_t READ = 0x80;
      };

      class Register
      {
      public:
        static const RegisterId_t MAIN = 0x00; // Read/Write
        static const RegisterId_t FSCTRL = 0x01; // R/W
        static const RegisterId_t FSDIV = 0x02; // R/W
        static const RegisterId_t MDMCTRL = 0x03; // R/W
        static const RegisterId_t AGCCTRL = 0x04; // R/W
        static const RegisterId_t FREND = 0x05; // R/W
        static const RegisterId_t RSSI = 0x06; // R/W
        static const RegisterId_t FREQEST = 0x07; // R/W
        static const RegisterId_t IOCFG = 0x08; // R/W
        // RESERVED
        // RESERVED
        static const RegisterId_t FSMTC = 0x0B; // R/W
        // RESERVED
        static const RegisterId_t MANAND = 0x0D; // R/W
        static const RegisterId_t FSMSTATE = 0x0E; // R/W
        static const RegisterId_t ADCTST = 0x0F; // R/W
        static const RegisterId_t RXBPFST = 0x10; // R/W
        static const RegisterId_t PAMTST = 0x11; // R/W
        static const RegisterId_t LMTST = 0x12; // R/W
        static const RegisterId_t MANOR = 0x13; // R/W
        static const RegisterId_t MDMTST0 = 0x14; // R/W
        static const RegisterId_t MDMTST1 = 0x15; // R/W
        static const RegisterId_t DACTST = 0x16; // R/W
        static const RegisterId_t AGCTST0 = 0x17; // R/W
        static const RegisterId_t AGCTST1 = 0x18; // R/W
        static const RegisterId_t AGCTST2 = 0x19; // R/W
        static const RegisterId_t FSTST0 = 0x1A; // R/W
        static const RegisterId_t FSTST1 = 0x1B; // R/W
        static const RegisterId_t FSTST2 = 0x1C; // R/W
        static const RegisterId_t FSTST3 = 0x1D; // R/W
        static const RegisterId_t MANFIDL = 0x1E; // Read
        static const RegisterId_t MANFIDH = 0x1F; // Read
        static const RegisterId_t GRMDM = 0x20; // R/W
        static const RegisterId_t GRDEC = 0x21; // R/W
        static const RegisterId_t PKTSTATUS = 0x22; // Read
        static const RegisterId_t INT = 0x23; // R/W
        // ...RESERVED
        static const RegisterId_t SYNCL = 0x2C; // R/W
        static const RegisterId_t SYNCH = 0x2D; // R/W
        static const RegisterId_t SXOSCON = 0x60; // Strobe
        static const RegisterId_t SFSON = 0x61; // Strobe
        static const RegisterId_t SRX_REG = 0x62; // Strobe
        static const RegisterId_t STX_REG = 0x63; // Strobe
        static const RegisterId_t SRFOFF = 0x64; // Strobe
        static const RegisterId_t SXOSCOFF = 0x65; // Strobe
        // .. RESERVED
        static const RegisterId_t FIFOREG = 0x70; // Special
      };

      class Registers
      {
      public:
        Registers(spim_t& spi);

        Status_t
        strobe(RegisterId_t reg);

        Status_t
        readWord(RegisterId_t reg, uint16_t* pValue);
        Status_t
        writeWord(RegisterId_t reg, uint16_t value);

        Status_t
        readByte(RegisterId_t reg, uint8_t* pValue);
        Status_t
        writeByte(RegisterId_t reg, uint8_t value);

        Status_t
        prepareReadWordContinuously(RegisterId_t reg);
        Status_t
        prepareWriteWordContinuously(RegisterId_t reg);

        uint16_t
        readWordContinuously(void);
        void
        writeWordContinuously(uint16_t value);

        Status_t
        prepareReadFifoContinuously(void);
        Status_t
        prepareWriteFifoContinuously(void);

        uint8_t
        readByteContinuously(void);
        void
        writeByteContinuously(uint8_t value);

        void
        setSpiToRadioSpeed(void);
        void
        setSpiHighSpeed(void);

        void
        switchToTxMode();

        void
        initSpi();
      private:
        spim_t& m_spi;
      };
    }

    class Cc2400
    {
    public:
      Cc2400(cc2400::spim_t& spi, ChipSelectActiveLow& cs,
          avr32::uc3::Gpio& m_mdmClkEn, avr32::uc3::Gpio& m_mdmGio1,
          avr32::uc3::Gpio& m_mdmGio6, avr32::uc3::Gpio& m_mdmRxStrb,
          avr32::uc3::Gpio& m_mdmTxStrb, avr32::uc3::Gpio& m_mdmPktRdy,
          avr32::uc3::Gpio& m_mdmFifoRdy, avr32::uc3::Gpio& m_mdmPwrEnIo,
          avr32::uc3::Gpio& m_mdmPwrEnCore);

      // TODO: make registers public

      // power OFF the modem
      void
      powerOff(void);

      // power ON the modem in 2 phases
      void
      powerOnPhase1(void);
      void
      powerOnPhase2(void);

      // set all registers to default value
      void
      configureRegisters();

      // do the calibration, and return its status
      bool
      isCalibrationOk(bool isDebug);

      // clear FIFO buffer
      void
      clearFifoBuffer();

      // returns value of last RSSI measured by modem in RX mode
      int16_t
      readRssiValue(void);

      // returns current value of FSM
      uint16_t
      readFsmState(device::texas::cc2400::Status_t &status);

      void
      prepareTx(uint16_t frecv,
          device::texas::cc2400::TxPowerLevel_t txPowerLevel);

      void
      prepareRx(uint16_t frecv);

      void
      switchToTxMode(void);

      void
      switchToRxMode(void);

      void
      switchToIdleMode(void);

      bool
      isTxComplete();

      void
      enableFifoInterrupt(avr32::uc3::intc::InterruptHandler_t handler,
          avr32::uc3::gpio::InterruptMode_t mode);

      void
      disableFifoInterrupt(void);

      device::texas::cc2400::Registers registers;

    private:
      //  assert CS, writes a CC2400 register, de-assert CS
      device::texas::cc2400::Status_t
      writeRegUseCs(device::texas::cc2400::RegisterId_t reg, uint16_t value);

      // the offset of RSSI register
      static const int16_t RSSI_OFFSET = -54;

      ChipSelectActiveLow& m_cs;
      // Radio Interface (Modem) pins
      avr32::uc3::Gpio& m_mdmClkEn;
      avr32::uc3::Gpio& m_mdmGio1;
      avr32::uc3::Gpio& m_mdmGio6;
      avr32::uc3::Gpio& m_mdmRxStrb;
      avr32::uc3::Gpio& m_mdmTxStrb;
      avr32::uc3::Gpio& m_mdmPktRdy;
      avr32::uc3::Gpio& m_mdmFifoRdy;

      // Radio Interface (Power supplies) pins
      avr32::uc3::Gpio& m_mdmPwrEnIo;
      avr32::uc3::Gpio& m_mdmPwrEnCore;

    };

    // ===== Inline methods ===================================================

    inline void
    Cc2400::switchToTxMode(void)
    {
      m_mdmRxStrb.setPinLow();
    }

    inline void
    Cc2400::switchToRxMode(void)
    {
      m_mdmTxStrb.setPinLow();
    }

    inline void
    Cc2400::switchToIdleMode(void)
    {
      m_mdmTxStrb.setPinLow();
      m_mdmRxStrb.setPinLow();
    }

    inline bool
    Cc2400::isTxComplete(void)
    {
      return (!m_mdmPktRdy.isPinHigh());
    }

    inline void
    Cc2400::disableFifoInterrupt(void)
    {
      m_mdmFifoRdy.clearInterruptRequested();
      m_mdmFifoRdy.disableInterrupt();
    }

    namespace cc2400
    {
      inline bool
      Status::isOscStable(Status_t status)
      {
        return (status & XOSC16M_STABLE) != 0;
      }

      inline bool
      Status::isSyncReceived(Status_t status)
      {
        return (status & SYNC_RECEIVED) != 0;
      }

      inline bool
      Status::isCrcOk(Status_t status)
      {
        return (status & CRC_OK) != 0;
      }

      inline bool
      Status::isFsLock(Status_t status)
      {
        return (status & FS_LOCK) != 0;
      }

      inline void
      Registers::setSpiToRadioSpeed(void)
      {
        m_spi.configChipSelect(16, 0, avr32::uc3::spi::BITS_8);
      }

      inline void
      Registers::setSpiHighSpeed(void)
      {
        m_spi.configChipSelect(1, 0, avr32::uc3::spi::BITS_8);
      }

      inline void
      Registers::initSpi(void)
      {
        m_spi.init();
        // set SPI max speed, no delay between bytes, 8-bit word size
        setSpiHighSpeed();
        m_spi.enable();
      }

    }
  }
}
#endif /* DEVICE_TEXAS_CC2400_H_ */
