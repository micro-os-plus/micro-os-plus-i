/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_USART)

#include "hal/arch/avr32/uc3/devices/onchip/include/Usart.h"

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Usart::Usart(usart::ModuleId_t module) :
          moduleRegisters(
              *reinterpret_cast<usart::ModuleRegisters*> (usart::ModuleRegisters::MEMORY_ADDRESS
                  + ((module % 4) * usart::ModuleRegisters::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Usart", module, this);
    }

    Usart::~Usart()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Usart", this);
    }

    // ----- Public Methods -----------------------------------------------------

    // --------------------------------------------------------------------------

    namespace usart
    {

      // ----- Constructors & Destructors -------------------------------------

      SpiMaster::SpiMaster(usart::ModuleId_t module) :
            moduleRegisters(
                *reinterpret_cast<usart::ModuleRegisters*> (usart::ModuleRegisters::MEMORY_ADDRESS
                    + ((module % 4) * usart::ModuleRegisters::MEMORY_OFFSET)))

      {
        OSDeviceDebug::putConstructorWithIndex("avr32::uc3::usart::SpiMaster",
            module, this);
      }

      SpiMaster::~SpiMaster()
      {
        OSDeviceDebug::putDestructor("avr32::uc3::usart::SpiMaster", this);
      }

      // ----- Public methods -------------------------------------------------

      void
      SpiMaster::powerUp(void)
      {
        reset();

        // Clear control, disable most things
        moduleRegisters.writeControl(
            AVR32_USART_CR_RSTRX_MASK | AVR32_USART_CR_RSTTX_MASK
                | AVR32_USART_CR_RSTSTA_MASK | AVR32_USART_CR_RSTIT_MASK
                | AVR32_USART_CR_RSTNACK_MASK | AVR32_USART_CR_RTSDIS_MASK);

        uint32_t mode;
        mode = 0;
          {
            // configure to 8 bits
            mode |= (AVR32_USART_MR_CHRL_8 << AVR32_USART_MR_CHRL_OFFSET);

            // SPI master mode.
            mode |= AVR32_USART_MR_MODE_SPI_MASTER
                << AVR32_USART_MR_MODE_OFFSET;

            // Default SPI mode 0
            // SPI clock phase 1
            mode |= (1 << AVR32_USART_MR_SYNC_OFFSET);
            // SPI clock polarity 0
            mode |= (0 << AVR32_USART_MR_MSBF_OFFSET);

            // Channel mode normal
            mode |= (AVR32_USART_MR_CHMODE_NORMAL
                << AVR32_USART_MR_CHMODE_OFFSET);

            // Drive SCK pin
            mode |= AVR32_USART_MR_CLKO_MASK;
          }
        moduleRegisters.writeMode(mode);

        // Configure Max clock frequency
        moduleRegisters.writeBaudRateGenerator(40);

        // Setup complete; enable communication.
        // Enable input and output.
        moduleRegisters.writeControl(
            AVR32_USART_CR_RXEN_MASK | AVR32_USART_CR_TXEN_MASK);
      }

      void
      SpiMaster::configurePolarityAndPhase(uint8_t polarity, uint8_t phase)
      {
        uint32_t mode;
        mode = moduleRegisters.readMode();
          {
            // Clear the two bits
            mode &= ~(AVR32_USART_MR_SYNC_MASK | AVR32_USART_MR_MSBF_MASK);

            // SPI clock phase
            mode |= ((phase & 1) << AVR32_USART_MR_SYNC_OFFSET);
            // SPI clock polarity
            mode |= ((polarity & 1) << AVR32_USART_MR_MSBF_OFFSET);
          }
        moduleRegisters.writeMode(mode);
      }

      void
      SpiMaster::powerDown(void)
      {
        uint32_t mode;
        mode = moduleRegisters.readMode();
          {
            // Do not drive SCK pin
            mode &= ~AVR32_USART_MR_CLKO_MASK;
          }
        moduleRegisters.writeMode(mode);

        clearControl();
      }

      void
      SpiMaster::configureClockFrequencyHz(
          usart::ClockFrequencyHz_t clockFrequency)
      {
        clockFrequency = clockFrequency;

        uint32_t clkdiv;

        clkdiv = getInputClockFrequencyHz() / clockFrequency;
        if (clkdiv < 4)
          clkdiv = 4;
        else if (clkdiv > ((1 << AVR32_USART_BRGR_CD_SIZE) - 1))
          clkdiv = ((1 << AVR32_USART_BRGR_CD_SIZE) - 1);

        OSDeviceDebug::putString("Clkdiv=");
        OSDeviceDebug::putDec(clkdiv);
        OSDeviceDebug::putNewLine();

        // configure USCLKS = CLK_USART
        uint32_t mode;
        mode = moduleRegisters.readMode();
        mode &= ~AVR32_USART_MR_USCLKS_MASK;
        mode |= (AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET);
        moduleRegisters.writeMode(mode);

        moduleRegisters.writeBaudRateGenerator(clkdiv);
      }

      uint8_t
      SpiMaster::writeWaitReadByte(uint8_t value)
      {
        // Reset status bits
        //moduleRegisters.writeControl(AVR32_USART_CR_RSTSTA_MASK);

        while (!moduleRegisters.isTransmitterReady())
          ;
        moduleRegisters.writeTransmitHolding(value);

        while (!moduleRegisters.isReceiverReady())
          ;
        return moduleRegisters.readReceiveHolding();
      }

      void
      SpiMaster::reset(void)
      {
        moduleRegisters.writeInterruptDisable(0xFFFFFFFF);

        moduleRegisters.readChannelStatus();

        moduleRegisters.writeMode(0);

        moduleRegisters.writeReceiveTimeOut(0);
        moduleRegisters.writeTransmitterTimeGuard(0);
      }

      void
      SpiMaster::clearControl(void)
      {
        // Clear control, disable most things
        moduleRegisters.writeControl(
            AVR32_USART_CR_RSTRX_MASK | AVR32_USART_CR_RSTTX_MASK
                | AVR32_USART_CR_RSTSTA_MASK | AVR32_USART_CR_RSTIT_MASK
                | AVR32_USART_CR_RSTNACK_MASK | AVR32_USART_CR_RTSDIS_MASK);
      }

    // ----------------------------------------------------------------------
    }

  // --------------------------------------------------------------------------

  }
}
#endif /* defined(OS_INCLUDE_AVR32_UC3_USART) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
