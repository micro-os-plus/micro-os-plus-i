/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_TWIM_H_
#define AVR32_UC3_TWIM_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Twim_Definitions.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Gpio.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Intc.h"
#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

namespace avr32
{
  namespace uc3
  {
    class Twim
    {
    public:

      // ----- Type definitions -----------------------------------------------

      typedef uint32_t BusSpeedKbps_t;

      // NOTE: The slave address is the 7 bit address, right aligned,
      // NOT the 8 bit right aligned with the R/W bit appended.
      typedef uint16_t Address_t;

      class BusSpeed
      {
      public:
        const static BusSpeedKbps_t STANDARD_MODE = 100;
        const static BusSpeedKbps_t FAST_MODE = 400;
        const static BusSpeedKbps_t FAST_PLUS_MODE = 1000;
        const static BusSpeedKbps_t HIGH_MODE = 3400;
      };
    public:

      // ----- Constructors & Destructors -------------------------------------

      Twim(twim::ModuleId_t module);
      ~Twim();

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

      bool
      isEnabled(void);

      void
      disable();

      void
      configureSmbus(uint32_t value);

      // Better do not call it while a transfer is in progress
      OSReturn_t
      configureBusSpeedKbps(BusSpeedKbps_t speed);

      // This is the main transfer method, doing a write followed by a read.
      OSReturn_t
      writeByteArrayReadByteArray(Address_t slaveAddress,
          uint8_t* pOutgoingBytes, size_t outgoingBytesLength,
          uint8_t* pIncomingBytes, size_t incomingBytesSize,
          size_t* pIncomingBytesLength);

      // Sends only the address; if the device is not there, a NACK will
      // be returned
      OSReturn_t
      probeDevice(Address_t slaveAddress);

      // Write a single byte
      OSReturn_t
      writeByte(Address_t slaveAddress, uint8_t byte);

      // Write a byte array
      OSReturn_t
      writeByteArray(Address_t slaveAddress, uint8_t* pOutgoingBytes,
          size_t outgoingBytesLength);

      // Write a single byte and read a byte array
      OSReturn_t
      writeByteReadByteArray(Address_t slaveAddress, uint8_t byte,
          uint8_t* pIncomingBytes, size_t incomingBytesSize,
          size_t* pIncomingBytesLength);

      // Read into a byte array
      OSReturn_t
      readByteArray(Address_t slaveAddress, uint8_t* pIncomingBytes,
          size_t incomingBytesSize, size_t* pIncomingBytesLength);

      // ---
      void
      setGpioConfigurationArray(
          avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray);

      void
      registerInterruptHandler(intc::InterruptHandler_t handler);

      uint32_t
      getInputClockFrequencyHz(void);

      void
      interruptServiceRoutine(void);

    private:

      // ----- Private methods ------------------------------------------------

      void
      softwareReset(void);

      void
      disableAllInterrupts(void);

      void
      clearStatus(void);

      bool
      isIdle(void);

      static bool
      isAddressTenBit(Address_t address);

    public:

      // ----- Public members -------------------------------------------------

      twim::ModuleRegisters& moduleRegisters;

    private:

      // ----- Private members ------------------------------------------------

      twim::ModuleId_t m_module;
      avr32::uc3::gpio::PinPeripheralFunction_t* m_pGpioConfigurationArray;

      // --
      volatile uint32_t twim_it_mask;
      enum twim_transfer_status
      {
        TWI_SUCCESS = 0, //!< \brief TWI Transaction Success
        TWI_INVALID_ARGUMENT = -1, //!< \brief Invalid Argument Passed
        TWI_ARBITRATION_LOST = -2, //!< \brief Bus Arbitration Lost
        TWI_NO_CHIP_FOUND = -3, //!< \brief Slave Not Found
        TWI_RECEIVE_NACK = -4, //!< \brief Data No Acknowledgement Received
        TWI_SEND_NACK = -5, //!< \brief Data No Acknowledgement Send
        TWI_INVALID_CLOCK_DIV = -6
      //!< \brief Invalid Clock Divider Value
      };
      typedef enum twim_transfer_status twim_transfer_status_t;

      volatile twim_transfer_status_t transfer_status;
      volatile bool twim_next; // =false
      volatile uint8_t *twim_rx_data; // = NULL;
      volatile uint32_t twim_rx_nb_bytes; // = 0;
      const volatile uint8_t *twim_tx_data; // = NULL;
      volatile uint32_t twim_tx_nb_bytes; // = 0;
      volatile uint32_t twim_rx_ef_bytes; // = 0;

      // ----------------------------------------------------------------------
    };

    // ----- Inline methods ---------------------------------------------------

    inline uint32_t
    Twim::getInputClockFrequencyHz(void)
    {
      return Pm::getPbaClockFrequencyHz();
    }

    inline void
    Twim::softwareReset(void)
    {
      // Reset interface. The interface must enabled for this to work
      moduleRegisters.writeControl(AVR32_TWIM_CR_SWRST_MASK);
    }

    inline void
    Twim::disable(void)
    {
      // Disable interface
      moduleRegisters.writeControl(AVR32_TWIM_CR_MDIS_MASK);
    }

    inline void
    Twim::enable(void)
    {
      // Enable interface
      moduleRegisters.writeControl(AVR32_TWIM_CR_MEN_MASK);
    }

    inline bool
    Twim::isEnabled(void)
    {
      return ((moduleRegisters.readStatus() & AVR32_TWIM_SR_MENB_MASK) != 0);
    }

    inline bool
    Twim::isIdle(void)
    {
      return ((moduleRegisters.readStatus() & AVR32_TWIM_SR_IDLE_MASK) != 0);
    }

    inline void
    Twim::disableAllInterrupts(void)
    {
      moduleRegisters.writeInterruptDisable(~0);
    }

    inline void
    Twim::clearStatus(void)
    {
      moduleRegisters.writeStatusClear(~0);
    }

    inline bool
    Twim::isAddressTenBit(Address_t address)
    {
      return (address > 0xFF);
    }

    inline void
    Twim::setGpioConfigurationArray(
        avr32::uc3::gpio::PinPeripheralFunction_t* pGpioConfigurationArray)
    {
      m_pGpioConfigurationArray = pGpioConfigurationArray;
    }

    inline OSReturn_t
    Twim::probeDevice(Address_t slaveAddress)
    {
      return writeByteArrayReadByteArray(slaveAddress, NULL, 0, NULL, 0, NULL);
    }

    inline OSReturn_t
    Twim::writeByte(Address_t slaveAddress, uint8_t byte)
    {
      return writeByteArrayReadByteArray(slaveAddress, &byte, 1, NULL, 0, NULL);
    }

    inline OSReturn_t
    Twim::writeByteArray(Address_t slaveAddress, uint8_t* pOutgoingBytes,
        size_t outgoingBytesLength)
    {
      return writeByteArrayReadByteArray(slaveAddress, pOutgoingBytes,
          outgoingBytesLength, NULL, 0, NULL);
    }

    inline OSReturn_t
    Twim::writeByteReadByteArray(Address_t slaveAddress, uint8_t byte,
        uint8_t* pIncomingBytes, size_t incomingBytesSize,
        size_t* pIncomingBytesLength)
    {
      return writeByteArrayReadByteArray(slaveAddress, &byte, 1,
          pIncomingBytes, incomingBytesSize, pIncomingBytesLength);
    }

    inline OSReturn_t
    Twim::readByteArray(Address_t slaveAddress, uint8_t* pIncomingBytes,
        size_t incomingBytesSize, size_t* pIncomingBytesLength)
    {
      return writeByteArrayReadByteArray(slaveAddress, NULL, 0, pIncomingBytes,
          incomingBytesSize, pIncomingBytesLength);
    }

  // ------------------------------------------------------------------------
  }
}

#endif /* AVR32_UC3_TWIM_H_ */
