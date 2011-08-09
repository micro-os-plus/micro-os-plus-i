/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_template_H_
#define AVR32_UC3_template_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Template_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    class Template
    {
    public:
      // ----- Type definitions -----------------------------------------------

      typedef uint32_t ClockFrequencyHz_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Template(template::ModuleId_t module);
      ~Template();

    public:

      // ----- Public methods -------------------------------------------------

      void
      powerUp(void);

      void
      powerDown(void);

      // May be called several times, if conditions change
      void
      initialise();

      void
      enable(void);

      void
      disable();

      // Allow to reuse the same instance for another module
      void
      setModule(template::ModuleId_t module);

      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioArray);

      uint32_t
      getInputClockFrequencyHz(void);


    private:

      // ----- Private methods ------------------------------------------------

      void
      softwareReset(void);

      void
      disableAllInterrupts(void);

    public:

      // ----- Public members -------------------------------------------------

      usart::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      //template::StatusRegister_t m_shadowStatusRegister;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Template::getInputClockFrequencyHz(void)
    {
      //return OS_CFGLONG_PBA_FREQUENCY_HZ;
      return OS_CFGLONG_PBB_FREQUENCY_HZ;
    }

    inline void
    Template::softwareReset(void)
    {
      // Reset interface
      moduleRegisters.writeControl(0);
    }

    inline void
    Template::disable(void)
    {
      // Disable interface and disableInterface power save mode
      moduleRegisters.writeControl(0);
    }

    inline void
    Template::enable(void)
    {
      // Enable interface and enableInterface power save
      moduleRegisters.writeControl(0);
    }

    inline void
    Template::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(0xFFFFFFFF);
    }

    // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_template_H_ */
