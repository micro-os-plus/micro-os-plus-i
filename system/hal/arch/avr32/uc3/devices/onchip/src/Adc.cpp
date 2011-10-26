/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_ADC)

#include "hal/arch/avr32/uc3/devices/onchip/include/Adc.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Adc::Adc() :
          moduleRegisters(
              *reinterpret_cast<adc::ModuleRegisters*> (adc::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructor("avr32::uc3::Adc", this);
    }

    Adc::~Adc()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Adc", this);
    }

    // ----- Public Methods ---------------------------------------------------

    OSReturn_t
    Adc::initialise(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Adc::initialise()");
      OSDeviceDebug::putNewLine();

      uint32_t ratio, prescal;
      adc::Mask_t mask = 0;

      // Use 10 bits.
      mask &= ~(1 << adc::ADC_LOWRES_OFFSET);
      // set Sample/Hold time to max so that the ADC capacitor should be loaded entirely
      mask |= 0xF << adc::ADC_SHTIM_OFFSET;
      // set Startup to max so that the ADC capacitor should be loaded entirely
      mask |= 0x1F << adc::ADC_STARTUP_OFFSET;
      // enable sleep mode
      mask |= 0x1 << adc::ADC_SLEEP_OFFSET;

      // Lower the ADC clock at 1MHZ. At 5MHz there are errors.
      // compute ratio. (PRESCAL+1) * 2 = ratio.
      ratio = getInputClockFrequencyHz() / 1000000;
      prescal = (ratio / 2) - 1;
      mask |= prescal << adc::ADC_MR_PRESCAL_OFFSET;

      moduleRegisters.writeMode(mask);

      return OSReturn::OS_OK;
    }

    void
    Adc::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Adc::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction(
              m_pGpioConfigurationArray);
        }
    }

    void
    Adc::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Adc::powerDown()");
      OSDeviceDebug::putNewLine();

      // reset the ADC
      moduleRegisters.writeControl(adc::ADC_CR_SWRST_MASK);

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
        }
    }

    bool
    Adc::isConversionEnded(void)
    {
      adc::Mask_t mask = moduleRegisters.readStatus() & 0x000000FF;

      if (mask == m_mask)
        {
          return true;
        }

      return false;
    }

    avr32::uc3::adc::Mask_t
    Adc::getConvertedValue(avr32::uc3::adc::ChannelNumber_t channel)
    {
      return moduleRegisters.readChannelData(channel);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_ADC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
