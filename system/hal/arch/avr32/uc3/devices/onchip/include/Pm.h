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

    public:

      // ----- Constructors & Destructors -------------------------------------

      Pm();
      ~Pm();

    public:

      // ----- Public methods -------------------------------------------------

    private:

      // ----- Private methods ------------------------------------------------


    public:

      // ----- Public members -------------------------------------------------

      pm::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------


      // ----------------------------------------------------------------------
    };

  }

}

#endif /* AVR32_UC3_PM_H_ */
