/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_USBB_H_
#define AVR32_UC3_USBB_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Usbb_Definitions.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{
  namespace uc3
  {
    class Usbb
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t ClockFrequencyHz_t;
      typedef uint32_t BusSpeedKbps_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Usbb();
      ~Usbb();

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

      bool
      idVbusHigh(void);

    private:

      // ----- Private methods ------------------------------------------------

      //void
      //softwareReset(void);

      //      void
      //      disableAllInterrupts(void);

    public:

      // ----- Public members -------------------------------------------------

      usbb::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Usbb::getInputClockFrequencyHz(void)
    {
      return Pm::getPbbClockFrequencyHz();
    }

    //    inline void
    //    Usbb::softwareReset(void)
    //    {
    //      // Reset interface
    //      moduleRegisters.writeGeneralControl(0);
    //    }

    inline void
    Usbb::disable(void)
    {
      // Disable interface
      moduleRegisters.writeGeneralControl(0);
    }

    //    inline void
    //    Usbb::enable(void)
    //    {
    //      // Enable interface
    //      moduleRegisters.writeControl(0); // TODO: update
    //    }
    //
    //    inline void
    //    Usbb::disableAllInterrupts(void)
    //    {
    //      moduleRegisters.writeInterruptDisable(~0);
    //    }

    inline void
    Usbb::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

    inline bool
    Usbb::idVbusHigh(void)
    {
      return ((moduleRegisters.readGeneralStatus()
          & AVR32_USBB_USBSTA_VBUS_MASK) != 0);
    }

  // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_USBB_H_ */
