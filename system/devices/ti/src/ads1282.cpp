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
      Ads1282::Ads1282(avr32::uc3::SpiMaster& spi,
          avr32::uc3::Gpio& gpioAds1282Cs, avr32::uc3::Gpio& gpioAds1282Sync,
          avr32::uc3::Gpio& gpioAds1282Drdy,
          avr32::uc3::Gpio& gpioAds1282Reset,
          avr32::uc3::Gpio& gpioAds1282Pwdn, avr32::uc3::Gpio& gpioAds1282Sclk,
          avr32::uc3::Gpio& gpioAds1282Miso, avr32::uc3::Gpio& gpioAds1282Mosi) :
        m_spi(spi), m_gpioAds1282Cs(gpioAds1282Cs), m_gpioAds1282Sync(
            gpioAds1282Sync), m_gpioAds1282Drdy(gpioAds1282Drdy),
            m_gpioAds1282Reset(gpioAds1282Reset), m_gpioAds1282Pwdn(
                gpioAds1282Pwdn), m_gpioAds1282Sclk(gpioAds1282Sclk),
            m_gpioAds1282Miso(gpioAds1282Miso), m_gpioAds1282Mosi(
                gpioAds1282Mosi)
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
      Ads1282::init(void)
      {
        m_gpioAds1282Cs.configureModeGpio();
        m_gpioAds1282Cs.configureDirectionInput();

        m_gpioAds1282Sync.setPinLow();
        m_gpioAds1282Sync.configureModeGpio();
        m_gpioAds1282Sync.configureDirectionOutput();

        m_gpioAds1282Drdy.configureModeGpio();
        m_gpioAds1282Drdy.configureDirectionInput();
        //        m_gpioAds1282Drdy.enableGlitchFilter();

        m_gpioAds1282Reset.setPinHigh();
        m_gpioAds1282Reset.configureModeGpio();
        m_gpioAds1282Reset.configureDirectionOutput();

        m_gpioAds1282Pwdn.setPinLow();
        m_gpioAds1282Pwdn.configureModeGpio();
        m_gpioAds1282Pwdn.configureDirectionOutput();

        // set SPI pins
        m_gpioAds1282Sclk.configureModePeripheral();
        m_gpioAds1282Sclk.configurePeripheralFunction(
            avr32::uc3::gpio::PeripheralFunction::A);

        m_gpioAds1282Miso.configureModePeripheral();
        m_gpioAds1282Miso.configurePeripheralFunction(
            avr32::uc3::gpio::PeripheralFunction::A);

        m_gpioAds1282Mosi.configureModePeripheral();
        m_gpioAds1282Mosi.configurePeripheralFunction(
            avr32::uc3::gpio::PeripheralFunction::A);
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

      void
      Ads1282::powerOn(void)
      {
        OSCriticalSection::enter();
          {
            isDrdyHighToLowFlag = false;
            m_gpioAds1282Drdy.enableInterrupt();

            m_gpioAds1282Pwdn.setPinHigh();
          }
        OSCriticalSection::exit();

        // wait for DRDY to be low again (16 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;
      }

      void
      Ads1282::powerOff(void)
      {
        m_gpioAds1282Pwdn.setPinLow();

        // Empirically.
        OS::busyWaitMicros(POWER_OFF_TIME);
      }

      void
      Ads1282::sync(void)
      {
        m_gpioAds1282Sync.setPinHigh();

        // wait tSPWH > 2/fclck (0.488us)
        OS::busyWaitMicros(T_SYNC_SPWH);

        m_gpioAds1282Sync.setPinLow();
      }

      void
      Ads1282::resetRegisters(void)
      {
        m_gpioAds1282Reset.setPinLow();

        // wait T_RST > 2/fclck (0.488us)
        OS::busyWaitMicros(T_RST);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        m_gpioAds1282Reset.setPinHigh();

        // wait for DRDY to be low again (64 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;
      }

      void
      Ads1282::readDataContinuous(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);
      }

      void
      Ads1282::stopReadDataContinuous(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);
      }

      void
      Ads1282::readDataByCommand(void)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATA);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);
      }

      void
      Ads1282::calibrateOffset(void)
      {
        m_gpioAds1282Sync.setPinHigh();

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send SYNC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SYNC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (64 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;
        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send OFSCAL
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::OFSCAL);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (16 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;

        m_gpioAds1282Sync.setPinLow();
      }

      void
      Ads1282::calibrateGain(void)
      {
        m_gpioAds1282Sync.setPinHigh();

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send SYNC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SYNC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (64 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;

        // send SDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::SDATAC);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // send GANCAL
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::GANCAL);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        isDrdyHighToLowFlag = false;
        m_gpioAds1282Drdy.enableInterrupt();

        // send RDATAC
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RDATAC);

        // wait for DRDY to be low again (16 Data Periods).
        while (!isDrdyHighToLowFlag)
          ;

        m_gpioAds1282Sync.setPinLow();
      }

      RegisterValue_t
      Ads1282::readRegister(RegisterId_t reg)
      {
        RegisterValue_t regValue;

        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::RREG + reg);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // One register.
        m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        regValue = m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        OSDeviceDebug::putHex(regValue);

        return regValue;
      }

      void
      Ads1282::writeRegister(RegisterId_t reg, RegisterValue_t value)
      {
        m_spi.writeWaitReadByte(device::ti::ads1282::Commands::WREG + reg);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        // One register.
        m_spi.writeWaitReadByte(0);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);

        m_spi.writeWaitReadByte(value);

        // wait T_DLY > 24/fclck (5.85us)
        OS::busyWaitMicros(T_DLY);
      }
    }
  }
}

#endif /* defined(OS_INCLUDE_DEVICE_TI_ADS1282) */
