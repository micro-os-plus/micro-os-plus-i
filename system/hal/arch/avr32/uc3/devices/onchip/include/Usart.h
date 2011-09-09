/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_USART_H_
#define AVR32_UC3_USART_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Usart_Definitions.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"

namespace avr32
{
  namespace uc3
  {
    class Usart
    {
      // ----------------------------------------------------------------------

    public:

      // ----- Type definitions -----------------------------------------------

    public:

      // ----- Constructors & Destructors -------------------------------------

      Usart(usart::ModuleId_t module);
      ~Usart();

    public:

      // ----- Public methods -------------------------------------------------

      uint32_t
      getInputClockFrequencyHz(void);

      void
      initialise(usart::ModuleId_t module);

      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

    private:

      // ----- Private methods ------------------------------------------------

      void
      softwareReset(void);

      void
      disableInterface(void);

      void
      enableInterface(void);

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
    Usart::getInputClockFrequencyHz(void)
    {
      return OS_CFGLONG_PBA_FREQUENCY_HZ;
    }

    inline void
    Usart::softwareReset(void)
    {
      // Reset interface
      moduleRegisters.writeControl(0);
    }

    inline void
    Usart::disableInterface(void)
    {
      // Disable interface and disableInterface power save mode
      moduleRegisters.writeControl(0);
    }

    inline void
    Usart::enableInterface(void)
    {
      // Enable interface and enableInterface power save
      moduleRegisters.writeControl(0);
    }

    inline void
    Usart::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(0xFFFFFFFF);
    }

    inline void
    Usart::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

    // ------------------------------------------------------------------------

    namespace usart
    {
      class SpiMaster
      {
        // --------------------------------------------------------------------

      public:

        // ----- Constructors & Destructors -----------------------------------

        SpiMaster(usart::ModuleId_t module);
        ~SpiMaster();

      public:

        // ----- Public methods -----------------------------------------------

        void
        powerUp(void);

        void
        powerDown(void);

        void
        configureClockFrequencyHz(usart::ClockFrequencyHz_t clockFrequency);

        void
        configurePolarityAndPhase(uint8_t polarity, uint8_t phase);

        uint8_t
        writeWaitReadByte(uint8_t value);

        uint32_t
        getInputClockFrequencyHz(void);

        void
        reset(void);

        void
        clearControl();

        void
        setGpioConfigurationArray(
            avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

      public:

        // ----- Public members -----------------------------------------------

        usart::ModuleRegisters& moduleRegisters;

      private:

        avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

        // --------------------------------------------------------------------

      };

      inline uint32_t
      SpiMaster::getInputClockFrequencyHz(void)
      {
        return OS_CFGLONG_PBA_FREQUENCY_HZ;
      }

      inline void
      SpiMaster::setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
      {
        m_pGpioConfigurationArray = pGpioConfigurationArray;
      }

    }
  }
}

#endif /* AVR32_UC3_USART_H_ */
