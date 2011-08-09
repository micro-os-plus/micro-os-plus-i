/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_EIC_H_
#define AVR32_UC3_EIC_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Eic_Definitions.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    class Eic
    {
    public:

      // ----- Type definitions -----------------------------------------------

    public:

      // ----- Constructors & Destructors -------------------------------------

      Eic(eic::InterruptId_t id);
      ~Eic();

    public:

      // ----- Public methods -------------------------------------------------

      void
      powerUp(void);
      void
      powerDown(void);

      // -----
      void
      enableInterrupt(void);
      void
      disableInterrupt(void);
      bool
      isInterruptEnabled(void);

      bool
      isInterruptRequested(void);
      void
      clearInterruptRequest(void);

      void
      configureTriggeredEdge(void);
      void
      configureTriggeredLevel(void);
      bool
      isTriggeredLevel(void);

      void
      configureTriggeredFallingEdge(void);
      void
      configureTriggeredRisingEdge(void);
      bool
      isTriggeredRisingEdge(void);

      void
      configureTriggeredLowLevel(void);
      void
      configureTriggeredHighLevel(void);
      bool
      isTriggeredHighLevel(void);

      void
      configureNotFiltered(void);
      void
      configureFiltered(void);
      bool
      isFiltered(void);

      void
      configureSynchronous(void);
      void
      configureAynchronous(void);
      bool
      isAsynchronous(void);

      void
      enableExternalInterrupt(void);
      void
      disableExternalInterrupt(void);
      bool
      isExternalInterruptEnabled(void);

      void
      configureScanPrescaller(eic::Prescaller_t prescaller);
      void
      enableScan(void);
      void
      disableScan(void);
      uint8_t
      readScanIndex(void);

      // -----
      void
      registerInterruptHandler(intc::InterruptHandler_t handler);

      // -----
      eic::Mask_t
      computeMaskFromInterruptId(eic::InterruptId_t id);

      void
      setInterrupt(eic::InterruptId_t id);

      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

    private:

      // ----- Private methods ------------------------------------------------


    public:

      // ----- Public members -------------------------------------------------

      eic::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      eic::Mask_t m_mask;
      eic::InterruptId_t m_id;

      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline void
    Eic::enableInterrupt(void)
    {
      moduleRegisters.writeInterruptEnable(m_mask);
    }

    inline void
    Eic::disableInterrupt(void)
    {
      moduleRegisters.writeInterruptDisable(m_mask);
    }

    inline bool
    Eic::isInterruptEnabled(void)
    {
      return ((moduleRegisters.readInterruptMask() & m_mask) != 0);
    }

    inline bool
    Eic::isInterruptRequested(void)
    {
      return ((moduleRegisters.readInterruptStatus() & m_mask) != 0);
    }

    inline void
    Eic::clearInterruptRequest(void)
    {
      moduleRegisters.writeInterruptClear(m_mask);
    }

    inline bool
    Eic::isTriggeredLevel(void)
    {
      return ((moduleRegisters.readMode() & m_mask) != 0);
    }

    inline bool
    Eic::isTriggeredRisingEdge(void)
    {
      return ((moduleRegisters.readEdge() & m_mask) != 0);
    }

    inline bool
    Eic::isTriggeredHighLevel(void)
    {
      return ((moduleRegisters.readLevel() & m_mask) != 0);
    }

    inline bool
    Eic::isFiltered(void)
    {
      return ((moduleRegisters.readFilter() & m_mask) != 0);
    }

    inline bool
    Eic::isAsynchronous(void)
    {
      return ((moduleRegisters.readAsync() & m_mask) != 0);
    }

    inline void
    Eic::enableExternalInterrupt(void)
    {
      moduleRegisters.writeEnable(m_mask);
    }

    inline void
    Eic::disableExternalInterrupt(void)
    {
      moduleRegisters.writeDisable(m_mask);
    }

    inline bool
    Eic::isExternalInterruptEnabled(void)
    {
      return ((moduleRegisters.readControl() & m_mask) != 0);
    }

    inline eic::Mask_t
    Eic::computeMaskFromInterruptId(eic::InterruptId_t id)
    {
      return ((1 << id) & 0x01FF);
    }

    inline void
    Eic::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }
  // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_EIC_H_ */
