/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MCI_H_
#define AVR32_UC3_MCI_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Mci_Definitions.h"

namespace avr32
{
  namespace uc3
  {
    class Mci
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t Speed_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Mci();
      ~Mci();

    public:

      // ----- Public methods -------------------------------------------------

      void
      init(Speed_t speed, mci::BusWidth_t busWidth, mci::CardSlot_t cardSlot);

      mci::StatusRegister_t
      sendCommand(mci::CommandCode_t cmd, mci::CommandArgument_t arg);

    private:

      // ----- Private methods ------------------------------------------------

      void
      reset(void);

      void
      disable(void);

      void
      enable(void);

      void
      initSpeed(Speed_t speed);

      void
      initSdCardBusWidthAndSlot(mci::BusWidth_t busWidth,
          mci::CardSlot_t cardSlot);

      void
      initDataTimeout(mci::TimeoutMultiplier_t multiplier,
          mci::TimeoutCycles_t cycles);

      void
      clearModeBits(void);

      void
      clearConfiguration(void);

      void
      enableModeBits(uint32_t);

      mci::StatusRegister_t
      getStatusRegister(void);

      bool
      isReady(void);

      void
      disableAllInterrupts(void);

    public:

      // ----- Public members -------------------------------------------------

      mci::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      mci::StatusRegister_t shadowStatusRegister;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline void
    Mci::reset(void)
    {
      // Reset interface
      moduleRegisters.writeControl(1 << AVR32_MCI_CR_SWRST);
    }

    inline void
    Mci::disable(void)
    {
      // Disable interface and disable power save mode
      moduleRegisters.writeControl(
          (1 << AVR32_MCI_CR_MCIDIS) | (1 << AVR32_MCI_CR_PWSDIS));
    }

    inline void
    Mci::enable(void)
    {
      // Enable interface and enable power save
      moduleRegisters.writeControl(
          (1 << AVR32_MCI_CR_MCIEN) | (1 << AVR32_MCI_CR_PWSEN));
    }

    inline void
    Mci::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(0xFFFFFFFF);
    }

    inline void
    Mci::clearConfiguration(void)
    {
      moduleRegisters.writeConfiguration(0);
    }

    inline void
    Mci::clearModeBits(void)
    {
      moduleRegisters.writeMode(0);
    }

    inline void
    Mci::initDataTimeout(mci::TimeoutMultiplier_t multiplier,
        mci::TimeoutCycles_t cycles)
    {
      moduleRegisters.writeDataTimeout(
          ((multiplier & 0x7) << AVR32_MCI_DTOR_DTOMUL_OFFSET)
              | ((cycles & 0xF) << AVR32_MCI_DTOR_DTOCYC_OFFSET));
    }

  // ------------------------------------------------------------------------
  }
}
#endif /* AVR32_UC3_MCI_H_ */
