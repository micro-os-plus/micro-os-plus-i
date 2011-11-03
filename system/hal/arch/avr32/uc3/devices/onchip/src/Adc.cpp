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

      adc::Mask_t mask = 0;

      // Use 10 bits.
      mask &= ~(1 << adc::LOWRES_OFFSET);
      // set Sample/Hold time to max so that the ADC capacitor should be loaded entirely
      mask |= (0xF << adc::SHTIM_OFFSET);
      // set Startup to max so that the ADC capacitor should be loaded entirely
      mask |= (0x7F << adc::STARTUP_OFFSET);
      // enable sleep mode
      mask |= (0x1 << adc::SLEEP_OFFSET);
#if 0
      uint32_t ratio, prescal;

      // Lower the ADC clock at 1MHZ. At 5MHz there are errors.
      // compute ratio. (PRESCAL+1) * 2 = ratio.
      ratio = getInputClockFrequencyHz() / 1000000;
      prescal = (ratio / 2) - 1;
      mask |= (prescal << adc::MR_PRESCAL_OFFSET);
#else
      // Put prescaler to it's maximum value, otherwise
      // SNS_P_VDA_LVL is not measured right.
      mask |= 0xFF << (adc::MR_PRESCAL_OFFSET);
#endif
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

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
        }
    }

    bool
    Adc::isConversionEnded(void)
    {
      adc::Mask_t mask = (moduleRegisters.readStatus() & 0x000000FF);

      if (mask == m_mask)
        {
          return true;
        }

      return false;
    }

    avr32::uc3::adc::Mask_t
    Adc::getConvertedValue(avr32::uc3::adc::ChannelId_t channel)
    {
      return moduleRegisters.readChannelData(channel);
    }
  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_ADC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
