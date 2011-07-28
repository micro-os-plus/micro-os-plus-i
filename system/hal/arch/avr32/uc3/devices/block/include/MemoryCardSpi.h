/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MEMORYCARDSPI_H_
#define AVR32_UC3_MEMORYCARDSPI_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceMemoryCard.h"
//#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Spi.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "portable/devices/generic/include/ChipSelectActiveLow.h"

namespace avr32
{
  namespace uc3
  {
    class MemoryCardSpi : public OSDeviceMemoryCard
    {
    public:

      // ---- Implementation class --------------------------------------------

      class Implementation : public OSDeviceMemoryCard::Implementation
      {
      public:
        Implementation(avr32::uc3::spi::ModuleId_t spiModuleId,
            avr32::uc3::gpio::PinNumber_t selectGpioNumber);
        virtual
        ~Implementation();

        //        void
        //        setOpenParameters(void);

        friend class OSDeviceMemoryCard;

      private:
        virtual OSReturn_t
        initialise();

        virtual OSReturn_t
        sendCommand(CommandCode_t commandCode, CommandArgument_t commandArg);

        virtual OSDeviceMemoryCard::Response_t
        readResponse(void);

        virtual OSReturn_t
        waitWhileBusy(void);

        virtual OSReturn_t
        configureBusWidth(BusWidth_t busWidth);

        virtual OSReturn_t
        configureBlockLength(BlockLength_t length);

        virtual OSReturn_t
        configureBlockCount(BlockCount_t count);

        virtual OSReturn_t
        configureClockFrequencyHz(uint32_t speed);

        virtual OSReturn_t
        selectCard(BusWidth_t busWidth);

        virtual bool
        isRxReady(void);

        virtual uint32_t
        readData(void);

        virtual bool
        isTxReady(void);

        virtual void
        writeData(uint32_t value);

        virtual void
        setHighSpeedMode(void);

        virtual bool
        isCrcError(void);

      private:

        // ---- Local methods -------------------------------------------------

        void
        initGpio(void);

      public:
        avr32::uc3::Spim m_spi;
        avr32::uc3::Gpio m_gpio;
        ChipSelectActiveLow m_cs;

        //Mci::ClockFrequencyHz_t m_speed;
        //mci::BusWidth_t m_busWidth;
        //mci::CardSlot_t m_cardSlot;

      };

    public:

      // ----- Constructors & Destructors -------------------------------------

      MemoryCardSpi(avr32::uc3::spi::ModuleId_t spiModuleId,
          avr32::uc3::gpio::PinNumber_t selectGpioNumber);

      virtual
      ~MemoryCardSpi();

    private:

      // ----- public members ------------------------------------------------

      Implementation implementation;

      // ----------------------------------------------------------------------
    };

  }
}

#endif /* AVR32_UC3_MEMORYCARDSPI_H_ */
