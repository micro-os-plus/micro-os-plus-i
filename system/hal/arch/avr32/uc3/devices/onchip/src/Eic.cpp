/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_EIC)

#include "hal/arch/avr32/uc3/devices/onchip/include/Eic.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Eic::Eic(eic::InterruptId_t id) :
          moduleRegisters(
              *reinterpret_cast<eic::ModuleRegisters*> (eic::ModuleRegisters::MEMORY_ADDRESS))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Eic", id, this);

      setInterrupt(id);

      m_pGpioConfigurationArray = NULL;
    }

    Eic::~Eic()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Eic", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    Eic::setInterrupt(eic::InterruptId_t id)
    {
      m_mask = computeMaskFromInterruptId(id);
      m_id = id;
    }

    void
    Eic::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Eic::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction
              ( m_pGpioConfigurationArray);
        }
    }

    void
    Eic::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Eic::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput( m_pGpioConfigurationArray);
        }
    }

    void
    Eic::configureTriggeredEdge(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readMode();
      mask &= ~m_mask;
      moduleRegisters.writeMode(mask);
    }

    void
    Eic::configureTriggeredLevel(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readMode();
      mask |= m_mask;
      moduleRegisters.writeMode(mask);
    }

    void
    Eic::configureTriggeredFallingEdge(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readEdge();
      mask &= ~m_mask;
      moduleRegisters.writeEdge(mask);
    }

    void
    Eic::configureTriggeredRisingEdge(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readEdge();
      mask |= m_mask;
      moduleRegisters.writeEdge(mask);
    }

    void
    Eic::configureTriggeredLowLevel(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readLevel();
      mask &= ~m_mask;
      moduleRegisters.writeLevel(mask);
    }

    void
    Eic::configureTriggeredHighLevel(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readLevel();
      mask |= m_mask;
      moduleRegisters.writeLevel(mask);
    }

    void
    Eic::configureNotFiltered(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readFilter();
      mask &= ~m_mask;
      moduleRegisters.writeFilter(mask);
    }

    void
    Eic::configureFiltered(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readFilter();
      mask |= m_mask;
      moduleRegisters.writeFilter(mask);
    }

    void
    Eic::configureSynchronous(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readAsync();
      mask &= ~m_mask;
      moduleRegisters.writeAsync(mask);
    }

    void
    Eic::configureAynchronous(void)
    {
      eic::Mask_t mask;

      mask = moduleRegisters.readAsync();
      mask |= m_mask;
      moduleRegisters.writeAsync(mask);
    }

    void
    Eic::configureScanPrescaller(eic::Prescaller_t prescaller)
    {
      uint16_t value;

      value = moduleRegisters.readScan();
      value &= ~0x001F00;
      value |= ((prescaller & 0x1F) << 8);

      moduleRegisters.writeScan(value);
    }

    void
    Eic::enableScan(void)
    {
      uint16_t value;

      value = moduleRegisters.readScan();
      value |= 0x1;
      moduleRegisters.writeScan(value);
    }

    void
    Eic::disableScan(void)
    {
      uint16_t value;

      value = moduleRegisters.readScan();
      value &= ~0x1;
      moduleRegisters.writeScan(value);
    }

    uint8_t
    Eic::readScanIndex(void)
    {
      uint32_t value;

      value = moduleRegisters.readScan();

      return (value >> 24);
    }

    // -----

    void
    Eic::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      if (m_id != eic::InterruptId::NMI)
        {
          // for this group, the interrupt line is identical to m_id
          Intc::registerInterruptHandler(handler,
              Intc::computeInterruptIndex(intc::Group::EIC, m_id),
              intc::GroupPriority::GROUP_01);
        }
      else
        {
          OSDeviceDebug::putString("Cannot register NMI handler");
        }
    }

  // --------------------------------------------------------------------------

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_EIC) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
