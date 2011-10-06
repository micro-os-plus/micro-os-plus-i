/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICE_TI_ADS1282_H_
#define DEVICE_TI_ADS1282_H_

#include "portable/kernel/include/OS.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"

#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

namespace device
{
  namespace ti
  {
    namespace ads1282
    {
      //
#if defined(OS_CONFIG_FAMILY_AVR32UC3)
      typedef avr32::uc3::SpiMaster spim_t;
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

      typedef uint8_t RegisterId_t;
      typedef uint8_t RegisterValue_t;
      typedef uint8_t RegisterOffset_t;

      static const uint32_t SCLCK = (4096 * 1000);

      // TDLY > 24/fclck (5.85us)
      static const uint32_t T_DLY_US = (24 * 1000 * 1000) / SCLCK + 1;

      // SYNC pulse width, high. Min = 2/fclk.
      static const uint32_t T_SYNC_SPWH_US = (2 * 1000 * 1000) / SCLCK + 1;

      // RESET low. Min = 2/fclk.
      static const uint32_t T_RST_US = (2 * 1000 * 1000) / SCLCK + 1;

      // 1 second.
      static const uint32_t POWER_OFF_TIME_S = 1;

      // 62.98046875/fDATA + 468/fCLK: 70*4 ms
      static const uint32_t T_DR_MAX_MS = 70 * 4;
      static const uint32_t T_DR_MAX_TICKS = (T_DR_MAX_MS
          * OS_CFGINT_TICK_RATE_HZ) / 1000 + 1;

      class Register
      {
      public:
        const static RegisterId_t ID = 0x00; // Read
        const static RegisterId_t CONFIG0 = 0x01; // R/W
        const static RegisterId_t CONFIG1 = 0x02; // R/W
        const static RegisterId_t HPF0 = 0x03; // R/W
        const static RegisterId_t HPF1 = 0x04; // R/W
        const static RegisterId_t OFC0 = 0x05; // R/W
        const static RegisterId_t OFC1 = 0x06; // R/W
        const static RegisterId_t OFC2 = 0x07; // R/W
        const static RegisterId_t FSC0 = 0x08; // R/W
        const static RegisterId_t FSC1 = 0x09; // R/W
        const static RegisterId_t FSC2 = 0x0A; // R/W

        const static RegisterOffset_t ModeOffset = 6;
        const static RegisterOffset_t DataRateOffset = 3;
        const static RegisterOffset_t PhaseOffset = 2;
        const static RegisterOffset_t FiltrOffset = 0;
        const static RegisterOffset_t MuxOffset = 4;
        const static RegisterOffset_t PgaChopOffset = 3;
        const static RegisterOffset_t GainOffset = 0;
      };

      class Commands
      {
      public:
        const static RegisterId_t WAKEUP = 0x00;
        const static RegisterId_t STANDBY = 0x02;
        const static RegisterId_t SYNC = 0x04;
        const static RegisterId_t RESET = 0x06;
        const static RegisterId_t RDATAC = 0x10;
        const static RegisterId_t SDATAC = 0x11;
        const static RegisterId_t RDATA = 0x12;
        const static RegisterId_t RREG = 0x20;
        const static RegisterId_t WREG = 0x40;
        const static RegisterId_t OFSCAL = 0x60;
        const static RegisterId_t GANCAL = 0x61;
      };

      class Ads1282
      {
      public:
        Ads1282(avr32::uc3::SpiMaster& spi, avr32::uc3::Gpio& gpioAds1282Drdy);

        void
        setGpioConfigurationArray(
            avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

        void
        powerUp(void);

        void
        powerDown(void);

        void
        startConfiguration();

        void
        stopConfiguration();

        OSReturn_t
        powerOn(void);

        void
        powerOff(void);

        void
        sync();

        OSReturn_t
        resetRegisters(void);

        void
        readDataContinuous(void);

        void
        stopReadDataContinuous(void);

        void
        readDataByCommand(void);

        OSReturn_t
        calibrateOffset(void);

        OSReturn_t
        calibrateGain(void);

        RegisterValue_t
        readRegister(RegisterId_t reg);

        void
        writeRegister(RegisterId_t reg, RegisterValue_t value);

        bool
        isDrdyHigh(void);

      public:
        // Handler for DRDY gpio pin interrupt. It only starts the SPI transfer. It has the highest interrupt priority level.
        void
        drdyInterruptServiceRoutine(void);

      private:

        spim_t& m_spi;

        avr32::uc3::Gpio m_gpioAds1282Cs;
        avr32::uc3::Gpio m_gpioAds1282Sync;
        avr32::uc3::Gpio& m_gpioAds1282Drdy;
        avr32::uc3::Gpio m_gpioAds1282Reset;
        avr32::uc3::Gpio m_gpioAds1282Pwdn;

        volatile bool isDrdyHighToLowFlag;

        avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;
      };

      // ===== Inline methods ===================================================

      inline bool
      Ads1282::isDrdyHigh(void)
      {
        return m_gpioAds1282Drdy.isPinHigh();

      }

      inline void
      Ads1282::setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
      {
        m_pGpioConfigurationArray = pGpioConfigurationArray;
      }
    }
  }
}
#endif /* DEVICE_TI_ADS1282_H_ */
