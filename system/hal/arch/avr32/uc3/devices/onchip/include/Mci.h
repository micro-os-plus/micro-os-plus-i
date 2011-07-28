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

      typedef uint32_t ClockFrequencyHz_t;

      typedef uint32_t BlockLength_t;
      typedef uint16_t BlockCount_t;

    public:

      // ----- Constructors & Destructors -------------------------------------

      Mci();
      ~Mci();

    public:

      // ----- Public methods -------------------------------------------------

      uint32_t
      getInputClockFrequencyHz(void);

      void
      initialise(mci::CardSlot_t cardSlot);

      mci::StatusRegister_t
      sendCommand(mci::CommandWord_t cmdWord, mci::CommandArgument_t cmdArg);

      void
      configureSdCardBusWidthAndSlot(mci::BusWidth_t busWidth,
          mci::CardSlot_t cardSlot);

      void
      configureBlockLengthBytes(uint32_t length);

      void
      configureBlockCount(BlockCount_t cnt);

      void
      configureClockFrequencyHz(ClockFrequencyHz_t speed);

      void
      configureBusWidth(mci::BusWidth_t busWidth);

      bool
      isBusy(void);

      bool
      isRxReady(void);

      uint32_t
      readData(void);

      bool
      isTxReady(void);

      bool
      isCrcError(void);

      void
      writeData(uint32_t value);

      // ---
      mci::StatusRegister_t
      getStatusRegister(void);

    private:

      // ----- Private methods ------------------------------------------------

      void
      softwareReset(void);

      void
      disableInterface(void);

      void
      enableInterface(void);

      void
      initDataTimeout(mci::TimeoutMultiplier_t multiplier,
          mci::TimeoutCycles_t cycles);

      void
      clearModeBits(void);

      void
      clearConfiguration(void);

      void
      enableModeBits(uint32_t);

      bool
      isCommandReady(void);

      void
      disableAllInterrupts(void);

    public:

      // ----- Public members -------------------------------------------------

      mci::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      mci::StatusRegister_t m_shadowStatusRegister;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Mci::getInputClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBB_FREQUENCY_HZ;
    }

    inline void
    Mci::softwareReset(void)
    {
      // Reset interface
      moduleRegisters.writeControl(1 << AVR32_MCI_CR_SWRST);
    }

    inline void
    Mci::disableInterface(void)
    {
      // Disable interface and disableInterface power save mode
      moduleRegisters.writeControl(
          (1 << AVR32_MCI_CR_MCIDIS) | (1 << AVR32_MCI_CR_PWSDIS));
    }

    inline void
    Mci::enableInterface(void)
    {
      // Enable interface and enableInterface power save
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
