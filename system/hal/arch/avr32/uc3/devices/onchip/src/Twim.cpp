/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_TWIM)

#include "hal/arch/avr32/uc3/devices/onchip/include/Twim.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Twim::Twim(twim::ModuleId_t module) :
          moduleRegisters(
              *reinterpret_cast<twim::ModuleRegisters*> (twim::ModuleRegisters::MEMORY_ADDRESS
                  + (module * twim::ModuleRegisters::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Twim", module, this);

      m_module = module;
    }

    Twim::~Twim()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Twim", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    Twim::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Twim::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction(
              m_pGpioConfigurationArray);
        }
    }

    void
    Twim::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Twim::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
        }
    }

    void
    Twim::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      OSCriticalSection::enter();
        {
          Intc::registerInterruptHandler(
              handler,
              Intc::computeInterruptIndex(
                  m_module == twim::ModuleId::TWIM0 ? intc::Group::TWIM0
                      : intc::Group::TWIM1, 0),
              m_module == twim::ModuleId::TWIM0 ? intc::GroupPriority::TWIM0
                  : intc::GroupPriority::TWIM1);
        }
      OSCriticalSection::exit();
    }

    OSReturn_t
    Twim::initialise(void)
    {
      OSCriticalSection::enter();
        {
          disableAllInterrupts();
          softwareReset();
        }
      OSCriticalSection::exit();

      clearStatus();

      return configureClockFrequencyHz(400000);
    }

    void
    Twim::configureSmbus(uint32_t value)
    {
      moduleRegisters.writeControl(AVR32_TWIM_CR_SMEN_MASK);
      moduleRegisters.writeSmbusTiming(value);
    }

    OSReturn_t
    Twim::configureClockFrequencyHz(ClockFrequencyHz_t speed)
    {
      uint32_t prescaler;
      uint8_t exponent = 0;

      prescaler = (getInputClockFrequencyHz() / speed / 2);

      // prescaler must fit in 8 bits, exponent must fit in 3 bits

      while ((prescaler > 0xFF) && (exponent <= 0x7))
        {
          // increase clock divider
          exponent++;
          // divide prescaler
          prescaler /= 2;
        }

      OSDeviceDebug::putDec(exponent);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(prescaler);
      OSDeviceDebug::putNewLine();

      if ((exponent > 0x7) || (prescaler > 0xFF))
        {
          OSDeviceDebug::putString("bad clock frequency ");
          OSDeviceDebug::putNewLine();

          return OSReturn::OS_BAD_PARAMETER;
        }

#define DATA_SETUP_AND_HOLD 0

      // set clock waveform generator register
      moduleRegisters.writeClockWaveformGenerator(
          (exponent << AVR32_TWIM_CWGR_EXP_OFFSET) | (DATA_SETUP_AND_HOLD
              << AVR32_TWIM_CWGR_DATA_OFFSET) | (prescaler
              << AVR32_TWIM_CWGR_STASTO_OFFSET) | ((prescaler - prescaler / 2)
              << AVR32_TWIM_CWGR_HIGH_OFFSET) | ((prescaler / 2)
              << AVR32_TWIM_CWGR_LOW_OFFSET));

      return OSReturn::OS_OK;
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_TWIM) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
