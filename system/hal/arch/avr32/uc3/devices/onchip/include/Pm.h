/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_PM_H_
#define AVR32_UC3_PM_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Pm_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    class Pm
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t ClockFrequencyHz_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Pm();
      ~Pm();

    public:

      // ----- Public methods -------------------------------------------------

      static ClockFrequencyHz_t
      getPbaClockFrequencyHz(void);

      static ClockFrequencyHz_t
      getPbbClockFrequencyHz(void);

    private:

      // ----- Private methods ------------------------------------------------


    public:

      // ----- Public members -------------------------------------------------

      pm::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------


      // ----------------------------------------------------------------------
    };

    inline Pm::ClockFrequencyHz_t
    Pm::getPbaClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBA_FREQUENCY_HZ;
    }

    inline Pm::ClockFrequencyHz_t
    getPbbClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBB_FREQUENCY_HZ;
    }
  }

}

#endif /* AVR32_UC3_PM_H_ */
