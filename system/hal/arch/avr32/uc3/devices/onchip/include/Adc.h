/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_ADC_H_
#define AVR32_UC3_ADC_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Adc_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    class Adc
    {
    public:
      Adc();
      ~Adc();

    public:

      // ----- Public methods -------------------------------------------------

      void
      powerUp(void);
      void
      powerDown(void);

      OSReturn_t
      initialise(void);

      void
      setChannelsMask(avr32::uc3::adc::ChannelsMask_t mask);

      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

      void
      startConversion(void);

      void
      softwareReset(void);

      void
      enableChannels(void);

      void
      disableChannels(void);

      bool
      isConversionEnded(void);

      avr32::uc3::adc::Mask_t
      getConvertedValue(avr32::uc3::adc::ChannelId_t channel);

      uint32_t
      getInputClockFrequencyHz(void);

      // -----

    public:

      // ----- Public members -------------------------------------------------

      adc::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

      avr32::uc3::adc::ChannelsMask_t m_mask;

      // ----------------------------------------------------------------------
    };

    inline void
    Adc::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

    inline uint32_t
    Adc::getInputClockFrequencyHz(void)
    {
      return Pm::getPbaClockFrequencyHz();
    }

    inline void
    Adc::setChannelsMask(avr32::uc3::adc::ChannelsMask_t mask)
    {
      m_mask = mask;
    }

    inline void
    Adc::startConversion(void)
    {
      moduleRegisters.writeControl(adc::START_MASK);
    }

    inline void
    Adc::softwareReset(void)
    {      // reset the ADC
      moduleRegisters.writeControl(adc::CR_SWRST_MASK);
    }

    inline void
    Adc::enableChannels(void)
    {
      moduleRegisters.writeChannelEnable(m_mask);
    }

    inline void
    Adc::disableChannels(void)
    {
      moduleRegisters.writeChannelDisable(m_mask);
    }
  }
}

#endif /* AVR32_UC3_ADC_H_ */
