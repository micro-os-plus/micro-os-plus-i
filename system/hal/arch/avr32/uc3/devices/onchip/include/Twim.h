/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_TWIM_H_
#define AVR32_UC3_TWIM_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Twim_Definitions.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"

namespace avr32
{
  namespace uc3
  {
    class Twim
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t ClockFrequencyHz_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Twim(twim::ModuleId_t module);
      ~Twim();

    public:

      // ----- Public methods -------------------------------------------------

      void
      powerUp(void);

      void
      powerDown(void);

      // May be called several times, if conditions change
      OSReturn_t
      initialise(void);

      void
      enable(void);

      void
      disable();

      void
      configureSmbus(uint32_t value);

      OSReturn_t
       configureClockFrequencyHz(ClockFrequencyHz_t speed);

      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

      void
      registerInterruptHandler(intc::InterruptHandler_t handler);

      uint32_t
      getInputClockFrequencyHz(void);

      void
      interruptServiceRoutine(void);

    private:

      // ----- Private methods ------------------------------------------------

      void
      softwareReset(void);

      void
      disableAllInterrupts(void);

      void
      clearStatus(void);

    public:

      // ----- Public members -------------------------------------------------

      twim::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      twim::ModuleId_t m_module;
      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;


      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Twim::getInputClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBA_FREQUENCY_HZ;
      //return OS_CFGLONG_PBB_FREQUENCY_HZ;
    }

    inline void
    Twim::softwareReset(void)
    {
      // Reset interface. The interface must be enabled
      moduleRegisters.writeControl(AVR32_TWIM_CR_SWRST_MASK);
    }

    inline void
    Twim::disable(void)
    {
      // Disable interface
      moduleRegisters.writeControl(AVR32_TWIM_CR_MDIS_MASK);
    }

    inline void
    Twim::enable(void)
    {
      // Enable interface
      moduleRegisters.writeControl(AVR32_TWIM_CR_MEN_MASK);
    }

    inline void
    Twim::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(~0);
    }

    inline       void
    Twim::clearStatus(void)
    {
      moduleRegisters.writeStatusClear(~0);
    }

    inline void
    Twim::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

  // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_TWIM_H_ */
