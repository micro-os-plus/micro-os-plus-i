/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICE_TEXAS_CC2400_H_
#define DEVICE_TEXAS_CC2400_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"

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
        static const RegisterId_t MAIN = 0x01; // Read/Write
        static const RegisterId_t FSCTRL = 0x02; // R/W

        // TODO: add the other
        static const RegisterId_t MANFIDL = 0x1E; // Read
        static const RegisterId_t MANFIDH = 0x1F; // Read
        // ...
        static const RegisterId_t SXOSCON = 0x60; // Strobe
        // ...
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

      private:
        spim_t& m_spi;
      };
    }

    class Cc2400
    {
    public:
      Cc2400(cc2400::spim_t& spi);

      // power ON and initialise all the configuration registers
      void
      PowerOn(void);

      // do the calibration, and return its status
      uint8_t
      calibrate(void);

      // clear FIFO buffer
      void
      clearFifoBuffer();

      // Set the packet which is going to be used in
      // the following transmission test
      void
      rpcSetRadioPacketTx(uint16_t packetSize, uint16_t *packet);

      // Start transmission of the packet,
      // already recorded using rpcSetRadioPacketTx.
      void
      rpcRadioStartTx(uint16_t frecv, cc2400::TxPowerLevel_t txPowerLevel);

      // Start transmission of the packet,
      // already recorded using rpcSetRadioPacketTx.
      void
      rpcRadioStartRx(uint16_t frecv, bool isLnaOn, uint16_t packetSize);

      // Requests the packet received in the last reception phase
      void
      rpcGetRadioPacketRx();

      // TODO: this is TCC Output Call
      // Send the packet requested by rpcGetRadioPacketRx.
      void
      rpcGetRadioPacketRx(uint16_t &packetSize, uint8_t *packet,
          int16_t &rssiValue, bool &isCrcOk);

    private:
      cc2400::Registers registers;
    };

    // ===== Inline methods ===================================================

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

    }
  }
}
#endif /* DEVICE_TEXAS_CC2400_H_ */
