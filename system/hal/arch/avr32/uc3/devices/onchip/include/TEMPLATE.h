/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_template_H_
#define AVR32_UC3_template_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Template_Definitions.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

/*
 * Template for device drivers.
 *
 * Usage:
 *      new Template(moduleId);
 *      setGpioConfigurationArray(...);
 *      ...
 *      powerUp();
 *      initialise();
 *      registerInterruptHandler(...);
 *      [ configureClockFrequencyHz(...) ] - optional
 *      enable();
 *      ...
 *      ...
 *      disable();
 *      powerDown();
 *
 */
namespace avr32
{
  namespace uc3
  {
    class Template
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t ClockFrequencyHz_t;
      typedef uint32_t BusSpeedKbps_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Template(TEMPLATE::ModuleId_t module);
      ~Template();

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

      bool
      isEnabled(void);

      OSReturn_t
      configureClockFrequencyHz(ClockFrequencyHz_t speed);

      OSReturn_t
      configureBusSpeedKbps(BusSpeedKbps_t speed);

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

    public:

      // ----- Public members -------------------------------------------------

      usart::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Template::getInputClockFrequencyHz(void)
    {
      // TODO: use the appropriate one

      Pm::getPbaClockFrequencyHz();
      //Pm::getPbaClockFrequencyHz();
    }

    inline void
    Template::softwareReset(void)
    {
      // Reset interface
      moduleRegisters.writeControl(0); // TODO: update
    }

    inline void
    Template::disable(void)
    {
      // Disable interface
      moduleRegisters.writeControl(0); // TODO: update
    }

    inline void
    Template::enable(void)
    {
      // Enable interface
      moduleRegisters.writeControl(0); // TODO: update
    }

    inline void
    Template::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(~0);
    }

    inline void
    Template::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

  // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_template_H_ */
