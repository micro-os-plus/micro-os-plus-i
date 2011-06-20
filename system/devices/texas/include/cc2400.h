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

      class OPERATION
      {
      public:
        static const RegisterOperation_t WRITE = 0x00;
        static const RegisterOperation_t READ = 0x80;
      };

      class REGISTER
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

    private:
      cc2400::Registers registers;
    };

  }
}
#endif /* DEVICE_TEXAS_CC2400_H_ */
