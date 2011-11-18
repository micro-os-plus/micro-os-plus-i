/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICE_TI_ADS1282)

#include "devices/ti/include/ads1282.h"

namespace device
{
  namespace ti
  {
    namespace ads1282
    {
      void
      adsBusyWaitUs(uint32_t microSeconds)
      {
        uint32_t ticks;
        uint32_t ccountt1, ccountt2, dcount;

        dcount = 0;

        ticks = microSeconds * 16;

        ccountt1 = Get_system_register(AVR32_COUNT);

        do
          {
            ccountt2 = Get_system_register(AVR32_COUNT);

            if (ccountt2 > ccountt1)
              {
                dcount += ccountt2 - ccountt1;
              }
            else
              {
                dcount += ccountt2 + (16000 - ccountt1);
              }
            ccountt1 = ccountt2;
          }
        while (dcount < ticks);
      }

      Ads1282::Ads1282(avr32::uc3::SpiMaster& spi,
          avr32::uc3::Gpio& gpioAds1282Drdy, avr32::uc3::Gpio& gpioAds1282Sync) :
        m_spi(spi), m_gpioAds1282Cs(OS_CFGPIN_ADS1282_CHIPSELECT_GPIO_PIN),
            m_gpioAds1282Sync(gpioAds1282Sync),
            m_gpioAds1282Drdy(gpioAds1282Drdy),
            m_gpioAds1282Reset(OS_CFGPIN_ADS1282_RESET_GPIO_PIN),
            m_gpioAds1282Pwdn(OS_CFGPIN_ADS1282_POWERDOWN_GPIO_PIN)
      {
#if defined(OS_DEBUG_DEVICE_TI_ADS1282)
        OSDeviceDebug::putConstructor("device::ti::Ads1282", this);
#endif
      }

      // ----- interrupt routine --------------------------------
      void
      Ads1282::drdyInterruptServiceRoutine(void)
      {
        isDrdyHighToLowFlag = true;

        // Ack and disable DRDY interrupt.
        m_gpioAds1282Drdy.clearInterruptRequest();
        m_gpioAds1282Drdy.disableInterrupt();
      }

      void
      Ads1282::powerUp(void)
      {
        OSDeviceDebug::putString("device::ti::Ads1282::powerUp()");
        OSDeviceDebug::putNewLine();

        if (m_pGpioConfigurationArray != NULL)
          {
            avr32::uc3::Gpio::configPeripheralModeAndFunction(
                m_pGpioConfigurationArray);
          }

        m_gpioAds1282Cs.configureModeGpio();
        m_gpioAds1282Cs.configureDirectionInput();

        m_gpioAds1282Sync.setPinLow();
        m_gpioAds1282Sync.configureModeGpio();
        m_gpioAds1282Sync.configureDirectionOutput();

        m_gpioAds1282Drdy.configureModeGpio();
        m_gpioAds1282Drdy.configureDirectionInput();

        m_gpioAds1282Reset.setPinHigh();
        m_gpioAds1282Reset.configureModeGpio();
        m_gpioAds1282Reset.configureDirectionOutput();

        m_gpioAds1282Pwdn.setPinLow();
        m_gpioAds1282Pwdn.configureModeGpio();
        m_gpioAds1282Pwdn.configureDirectionOutput();
      }

      void
      Ads1282::powerDown(void)
      {
        OSDeviceDebug::putString("device::ti::Ads1282::powerDown()");
        OSDeviceDebug::putNewLine();

        if (m_pGpioConfigurationArray != NULL)
          {
            avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
          }

        m_gpioAds1282Cs.configureModeGpio();
        m_gpioAds1282Cs.configureDirectionInput();

        m_gpioAds1282Sync.configureModeGpio();
        m_gpioAds1282Sync.configureDirectionInput();

        m_gpioAds1282Drdy.configureModeGpio();
        m_gpioAds1282Drdy.configureDirectionInput();

        m_gpioAds1282Reset.configureModeGpio();
        m_gpioAds1282Reset.configureDirectionInput();

        m_gpioAds1282Pwdn.configureModeGpio();
        m_gpioAds1282Pwdn.configureDirectionInput();
      }

      void
      Ads1282::startConfiguration(void)
      {
        avr32::uc3::spi::BaudRateFactor_t baudRateFactor;

        baudRateFactor = m_spi.getInputClockFrequencyHz()
            / (device::ti::ads1282::SCLCK / 4);

        m_spi.powerUp();

        // set SPI baudrate, no delay between bytes, 8-bit word size
        m_spi.configureChipSelect(baudRateFactor, 0, avr32::uc3::spi::BITS_8);

        m_spi.enable();
      }

      void
      Ads1282::stopConfiguration(void)
      {
        m_spi.disable();
      }

      OSReturn_t
      Ads1282::powerOn(void)
      {
        m_gpioAds1282Pwdn.setPinHigh();

        OSScheduler::timerTicks.sleep(POWER_ON_TIME_MS);

        // Wait until DRDY is low.
        uint32_t i;
        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // wait for DRDY to be low again.
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

//        OSScheduler::timerSeconds.sleep(1);

        return OSReturn::OS_OK;
      }

      void
      Ads1282::powerOff(void)
      {
        m_gpioAds1282Pwdn.setPinLow();

        // Empirically.
        // TODO: change this when time module is implemented, to see
        // if there is at least 1 sec between stop - start.
        OSScheduler::timerSeconds.sleep(POWER_OFF_TIME_S);
      }

      void
      Ads1282::prepareSync(void)
      {
        m_gpioAds1282Sync.setPinHigh();

        // wait tSPWH > 2/fclck (0.488us)
        adsBusyWaitUs(T_SYNC_SPWH_US);

        m_gpioAds1282Sync.setPinLow();

        // wait tSPWH > 2/fclck (0.488us)
        adsBusyWaitUs(T_SYNC_SPWL_US);
      }

      void
      Ads1282::sync(void)
      {
        m_gpioAds1282Sync.setPinHigh();
      }

      OSReturn_t
      Ads1282::resetRegisters(void)
      {
        uint32_t i;
        m_gpioAds1282Reset.setPinLow();

        // wait T_RST > 2/fclck (0.488us)
        adsBusyWaitUs(T_RST_US);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        m_gpioAds1282Reset.setPinHigh();

        // wait for DRDY to be low again (64 Data Periods).
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

        return OSReturn::OS_OK;
      }

      void
      Ads1282::readDataContinuous(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);
      }

      void
      Ads1282::stopReadDataContinuous(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);
      }

      void
      Ads1282::readDataByCommand(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATA);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);
      }

      OSReturn_t
      Ads1282::calibrateOffset(void)
      {
        uint32_t i;

        m_gpioAds1282Sync.setPinHigh();

        // wait tSPWH > 2/fclck (0.488us)
        adsBusyWaitUs(T_SYNC_SPWH_US);

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        // send SYNC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SYNC);

        // wait T_DLY > 24/fclck (5.85us)
        //OS::busyWaitMicros(T_DLY_US);
        adsBusyWaitUs(T_DLY_US);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (64 Data Periods).
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

//        OSScheduler::timerSeconds.sleep(1);

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        // send OFSCAL
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::OFSCAL);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (16 Data Periods).
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

        m_gpioAds1282Sync.setPinLow();

        return OSReturn::OS_OK;
      }

      OSReturn_t
      Ads1282::calibrateGain(void)
      {
        uint32_t i;

        m_gpioAds1282Sync.setPinHigh();

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY_US);

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY_US);

        // send SYNC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SYNC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY_US);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (64 Data Periods).
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY_US);

        // send GANCAL
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::GANCAL);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY_US);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (16 Data Periods).
        for (i = 0; i < T_DR_MAX_TICKS && !isDrdyHighToLowFlag; i++)
          {
            OSScheduler::timerTicks.sleep(1);
          }

        if (i == T_DR_MAX_TICKS)
          {
            return OSReturn::OS_DISCONNECTED;
          }

        m_gpioAds1282Sync.setPinLow();

        return OSReturn::OS_OK;
      }

      RegisterValue_t
      Ads1282::readRegister(RegisterId_t reg)
      {
        RegisterValue_t regValue;

        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RREG + reg);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        // One register.
        m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        regValue = m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        return regValue;
      }

      void
      Ads1282::writeRegister(RegisterId_t reg, RegisterValue_t value)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::WREG + reg);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        // One register.
        m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);

        m_spi.writeWaitReadByte(value);

        // wait T_DLY > 24/fclck (5.85us)
        adsBusyWaitUs(T_DLY_US);
      }
    }
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_TI_ADS1282) */
