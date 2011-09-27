/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_AVR32_UC3_TWIM)

#include "hal/arch/avr32/uc3/devices/onchip/include/Twim.h"

//#define OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE  (1)
//#define OS_DEBUG_AVR32_UC3_TWIM_WRITEBYTEARRAYREADBYTEARRAY (1)

namespace avr32
{
  namespace uc3
  {

    // ----- Constructors & Destructors ---------------------------------------

    Twim::Twim(twim::ModuleId_t module) :
          moduleRegisters(
              *reinterpret_cast<twim::ModuleRegisters*> (twim::ModuleRegisters::MEMORY_ADDRESS
                  + (module * twim::ModuleRegisters::MEMORY_OFFSET)))
    {
      OSDeviceDebug::putConstructorWithIndex("avr32::uc3::Twim", module, this);
      OSDeviceDebug::putString("TWIM Version=");
      OSDeviceDebug::putHex(moduleRegisters.readVersion());
      OSDeviceDebug::putNewLine();

      m_module = module;
    }

    Twim::~Twim()
    {
      OSDeviceDebug::putDestructor("avr32::uc3::Twim", this);
    }

    // ----- Public Methods ---------------------------------------------------

    void
    Twim::powerUp(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Twim::powerUp()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configPeripheralModeAndFunction(
              m_pGpioConfigurationArray);
        }
    }

    void
    Twim::powerDown(void)
    {
      OSDeviceDebug::putString("avr32::uc3::Twim::powerDown()");
      OSDeviceDebug::putNewLine();

      if (m_pGpioConfigurationArray != NULL)
        {
          avr32::uc3::Gpio::configGpioModeInput(m_pGpioConfigurationArray);
        }
    }

    void
    Twim::registerInterruptHandler(intc::InterruptHandler_t handler)
    {
      //OSCriticalSection::enter();
        {
          if (m_module == twim::ModuleId::TWIM0)
            Intc::registerInterruptHandler(handler,
                Intc::computeInterruptIndex(intc::Group::TWIM0, 0),
                intc::GroupPriority::TWIM0);
          else
            Intc::registerInterruptHandler(handler,
                Intc::computeInterruptIndex(intc::Group::TWIM1, 0),
                intc::GroupPriority::TWIM1);
        }
      //OSCriticalSection::exit();
    }

    OSReturn_t
    Twim::initialise(void)
    {
      OSCriticalSection::enter();
        {
          disableAllInterrupts();
          enable();
          softwareReset();
        }
      OSCriticalSection::exit();

      clearStatus();

      return configureBusSpeedKbps(BusSpeed::STANDARD_MODE);
    }

    void
    Twim::configureSmbus(uint32_t value)
    {
      moduleRegisters.writeControl(AVR32_TWIM_CR_SMEN_MASK);
      moduleRegisters.writeSmbusTiming(value);
    }

    OSReturn_t
    Twim::configureBusSpeedKbps(BusSpeedKbps_t speed)
    {
      uint32_t prescaler;
      uint8_t exponent = 0;

      prescaler = (getInputClockFrequencyHz() / (speed * 1000) / 2);

      // prescaler must fit in 8 bits, exponent must fit in 3 bits

      while ((prescaler > 0xFF) && (exponent <= 0x7))
        {
          // increase clock divider
          exponent++;
          // divide prescaler
          prescaler /= 2;
        }

      OSDeviceDebug::putDec(exponent);
      OSDeviceDebug::putChar(',');
      OSDeviceDebug::putDec(prescaler);
      OSDeviceDebug::putNewLine();

      if ((exponent > 0x7) || (prescaler > 0xFF))
        {
          OSDeviceDebug::putString("bad clock frequency ");
          OSDeviceDebug::putNewLine();

          return OSReturn::OS_BAD_PARAMETER;
        }

#define DATA_SETUP_AND_HOLD 0

      // set clock waveform generator register
      moduleRegisters.writeClockWaveformGenerator(
          (exponent << AVR32_TWIM_CWGR_EXP_OFFSET) | (DATA_SETUP_AND_HOLD
              << AVR32_TWIM_CWGR_DATA_OFFSET) | (prescaler
              << AVR32_TWIM_CWGR_STASTO_OFFSET) | ((prescaler - prescaler / 2)
              << AVR32_TWIM_CWGR_HIGH_OFFSET) | ((prescaler / 2)
              << AVR32_TWIM_CWGR_LOW_OFFSET));

      return OSReturn::OS_OK;
    }

    OSReturn_t
    Twim::writeByteArrayReadByteArray(Address_t slaveAddress,
        uint8_t* pOutgoingBytes, size_t outgoingBytesLength,
        uint8_t* pIncomingBytes, size_t incomingBytesSize,
        size_t* pIncomingBytesLength)
    {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_WRITEBYTEARRAYREADBYTEARRAY)
      OSDeviceDebug::putString("I2C addr=");
      OSDeviceDebug::putHex(slaveAddress);
      OSDeviceDebug::putString(", wl=");
      OSDeviceDebug::putDec(outgoingBytesLength);
      OSDeviceDebug::putString(", rl=");
      OSDeviceDebug::putDec(incomingBytesSize);
      OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_WRITEBYTEARRAYREADBYTEARRAY) */

      // Avoid null pointer references
      if (pOutgoingBytes == NULL)
        {
          //OSDeviceDebug::putString(" null_out ");
          outgoingBytesLength = 0;
        }

      if (pIncomingBytes == NULL)
        {
          //OSDeviceDebug::putString(" null_in ");
          incomingBytesSize = 0;
        }

      if (!isEnabled())
        return OSReturn::OS_NOT_ENABLED;

      bool isTenBit;
      isTenBit = isAddressTenBit(slaveAddress);

      // Reset the TWIM module to clear the THR register
      enable();
      softwareReset();
      disable();

      // Disable the TWIM interrupts
      disableAllInterrupts();
      clearStatus();

      OSCriticalSection::enter();
        {
          // point to outgoing buffer
          twim_tx_data = pOutgoingBytes;

          // set the number of bytes to transmit
          twim_tx_nb_bytes = outgoingBytesLength;
#if false
          OSDeviceDebug::putString(" this");
          OSDeviceDebug::putPtr((void*)this);
          OSDeviceDebug::putString(" ptx");
          OSDeviceDebug::putPtr((void*)&twim_tx_nb_bytes);
          OSDeviceDebug::putChar(',');
          OSDeviceDebug::putDec(twim_tx_nb_bytes);
          OSDeviceDebug::putChar(' ');
#endif
          // point to incoming buffer
          twim_rx_data = pIncomingBytes;

          // set the number of bytes to receive
          twim_rx_nb_bytes = incomingBytesSize;

          // Initialize bus transfer status
          transfer_status = TWI_SUCCESS;

          twim_rx_ef_bytes = 0;

          if (incomingBytesSize == 0)
            {
              // Write only call

              // Set next transfer to false
              twim_next = false;

              // set the command to start the write transfer
              moduleRegisters.writeCommand(
                  (slaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET)
                      | ((outgoingBytesLength & 0xFF)
                          << AVR32_TWIM_CMDR_NBYTES_OFFSET)
                      | (AVR32_TWIM_CMDR_VALID_MASK)
                      | (AVR32_TWIM_CMDR_START_MASK)
                      | (AVR32_TWIM_CMDR_STOP_MASK) | ((isTenBit ? 1 : 0)
                      << AVR32_TWIM_CMDR_TENBIT_OFFSET) | (0
                      << AVR32_TWIM_CMDR_READ_OFFSET));

              moduleRegisters.writeNextCommand(0);

              //OSDeviceDebug::putString(" 3 ");

              if (outgoingBytesLength != 0)
                {
                  // enable TXRDY interrupts
                  twim_it_mask = AVR32_TWIM_IER_TXRDY_MASK;
                }
            }
          else
            {
              if (outgoingBytesLength == 0)
                {
                  // Read only call

                  // Set next transfer to false
                  twim_next = false;

                  // set the command to start the read transfer
                  moduleRegisters.writeCommand(
                      (slaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET)
                          | ((incomingBytesSize & 0xFF)
                              << AVR32_TWIM_CMDR_NBYTES_OFFSET)
                          | (AVR32_TWIM_CMDR_VALID_MASK)
                          | (AVR32_TWIM_CMDR_START_MASK)
                          | (AVR32_TWIM_CMDR_STOP_MASK) | ((isTenBit ? 1 : 0)
                          << AVR32_TWIM_CMDR_TENBIT_OFFSET)
                          | (AVR32_TWIM_CMDR_READ_MASK));

                  moduleRegisters.writeNextCommand(0);

                  //OSDeviceDebug::putString(" 4 ");

                  // mask RXRDY interrupts
                  twim_it_mask = AVR32_TWIM_IER_RXRDY_MASK;
                }
              else
                {
                  // Write followed by read call

                  // set the command to start the write transfer
                  moduleRegisters.writeCommand(
                      (slaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET)
                          | ((outgoingBytesLength & 0xFF)
                              << AVR32_TWIM_CMDR_NBYTES_OFFSET)
                          | (AVR32_TWIM_CMDR_VALID_MASK)
                          | (AVR32_TWIM_CMDR_START_MASK) | (0
                          << AVR32_TWIM_CMDR_STOP_OFFSET) | ((isTenBit ? 1 : 0)
                          << AVR32_TWIM_CMDR_TENBIT_OFFSET) | (0
                          << AVR32_TWIM_CMDR_READ_OFFSET));

                  // set the command to start the read transfer
                  moduleRegisters.writeNextCommand(
                      (slaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET)
                          | ((incomingBytesSize & 0xFF)
                              << AVR32_TWIM_CMDR_NBYTES_OFFSET)
                          | (AVR32_TWIM_CMDR_VALID_MASK)
                          | (AVR32_TWIM_CMDR_START_MASK)
                          | (AVR32_TWIM_CMDR_STOP_MASK) | ((isTenBit ? 1 : 0)
                          << AVR32_TWIM_CMDR_TENBIT_OFFSET)
                          | (AVR32_TWIM_CMDR_READ_MASK));

                  // mask NACK and TXRDY and RXRDY interrupts
                  twim_it_mask = AVR32_TWIM_IER_TXRDY_MASK
                      | AVR32_TWIM_IER_RXRDY_MASK;

                  //OSDeviceDebug::putString(" 6 ");
                  //OSDeviceDebug::putHex(moduleRegisters.readNextCommand());

                }
            }
          //OSDeviceDebug::putString(" 7 ");

          // Enable NACK anc CCOMP too
          twim_it_mask |= AVR32_TWIM_IER_ANAK_MASK | AVR32_TWIM_IER_DNAK_MASK
              | AVR32_TWIM_IER_CCOMP_MASK;
          // update IMR through IER
          moduleRegisters.writeInterruptEnable(twim_it_mask);

          //OSDeviceDebug::putString(" 8 ");
          enable();
        }
      OSCriticalSection::exit();

#if true
      //OSDeviceDebug::putString(" wb ");

      // send data
      while ((transfer_status == TWI_SUCCESS) && !isIdle())
        {
          OSDeviceDebug::putChar('w');
          // we cannot use yield in inits :-(
          if (OSScheduler::isRunning())
            OSScheduler::yield();
        }

      //OSDeviceDebug::putString(" we ");
#endif

      if (pIncomingBytesLength != NULL)
        {
//          OSDeviceDebug::putString(" ptr");
//          OSDeviceDebug::putPtr(pIncomingBytesLength);
//          OSDeviceDebug::putChar(' ');

          *pIncomingBytesLength = twim_rx_ef_bytes;
        }

      if (transfer_status == TWI_RECEIVE_NACK)
        {
          OSDeviceDebug::putString(" NACK ");
          return OSReturn::OS_NACK;
        }

      if (transfer_status != TWI_SUCCESS) // TWI_ARBITRATION_LOST)
        {
          OSDeviceDebug::putString(" ERROR ");
          return OSReturn::OS_ERROR;
        }

      //OSDeviceDebug::putString(" OK ");
      return OSReturn::OS_OK;
    }

    // ----- Interrupt service routine ----------------------------------------

    void
    Twim::interruptServiceRoutine(void)
    {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
      OSDeviceDebug::putChar('#');
      //OSDeviceDebug::putPtr((void*) this);
      //OSDeviceDebug::putChar(' ');
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

      // get masked status register value
      twim::Status_t status;
      status = moduleRegisters.readStatus();

      uint16_t imr;
      imr = moduleRegisters.readInterruptMask();

#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
      OSDeviceDebug::putHex(status);
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

      // Is this a NACK?
      if (((status & (AVR32_TWIM_SR_ANAK_MASK | AVR32_TWIM_SR_DNAK_MASK
          | AVR32_TWIM_SR_ARBLST_MASK)) != 0) && ((imr
          & (AVR32_TWIM_IMR_ANAK_MASK | AVR32_TWIM_IMR_DNAK_MASK)) != 0))
        {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putString(" nak ");
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

          // if we get a nak, clear the valid bit in cmdr,
          // otherwise the command will be resent.

          transfer_status = (status & (AVR32_TWIM_IER_ANAK_MASK
              | AVR32_TWIM_IER_DNAK_MASK)) ? TWI_RECEIVE_NACK
              : TWI_ARBITRATION_LOST;

          moduleRegisters.writeCommand(
              moduleRegisters.readCommand() & ~AVR32_TWIM_CMDR_VALID_MASK);

          clearStatus();
          disableAllInterrupts();
        }
      else if ((status & AVR32_TWIM_SR_CCOMP_MASK) != 0 && ((imr
          & AVR32_TWIM_IER_CCOMP_MASK) != 0))
        {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putString(" cmd ");
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

          moduleRegisters.writeStatusClear(AVR32_TWIM_SR_CCOMP_MASK);
        }
      // Is this a RXRDY?
      else if ((status & AVR32_TWIM_SR_RXRDY_MASK) != 0 && ((imr
          & AVR32_TWIM_IMR_RXRDY_MASK) != 0))
        {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putString(" rx");
          OSDeviceDebug::putDec(twim_rx_nb_bytes);
          OSDeviceDebug::putChar(' ');
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

          uchar_t ch;

          ch = moduleRegisters.readReceiveHolding();
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putHex(ch);
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

          // get data from Receive Holding Register
          *twim_rx_data = ch;
          twim_rx_data++;

          twim_rx_ef_bytes++;

          // decrease received bytes number
          twim_rx_nb_bytes--;

          // receive complete
          if (twim_rx_nb_bytes == 0)
            {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
              OSDeviceDebug::putString(" drx ");
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
              // finish the receive operation
              moduleRegisters.writeInterruptDisable(AVR32_TWIM_IDR_RXRDY_MASK);
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
              OSDeviceDebug::putHex(moduleRegisters.readInterruptMask());
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
            }
        }

      // Is this a TXRDY?
      else if ((status & AVR32_TWIM_SR_TXRDY_MASK) != 0 && ((imr
          & AVR32_TWIM_IMR_TXRDY_MASK) != 0))
        {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putString(" tx");
          OSDeviceDebug::putDec(twim_tx_nb_bytes);
          OSDeviceDebug::putChar(' ');
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */

#if false
          OSDeviceDebug::putString(" ptx");
          OSDeviceDebug::putPtr((void*)&twim_tx_nb_bytes);
          OSDeviceDebug::putChar(',');
          OSDeviceDebug::putDec(twim_tx_nb_bytes);
          OSDeviceDebug::putChar(' ');
#endif

          // no more bytes to transmit
          if (twim_tx_nb_bytes == 0)
            {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
              OSDeviceDebug::putString(" dtx ");
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
              // finish the transmit operation
              moduleRegisters.writeInterruptDisable(AVR32_TWIM_IDR_TXRDY_MASK);
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
              OSDeviceDebug::putHex(moduleRegisters.readInterruptMask());
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
            }
          else
            {
              uchar_t ch;
              ch = *twim_tx_data++;
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
              OSDeviceDebug::putHex(ch);
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
              // put the byte in the Transmit Holding Register
              moduleRegisters.writeTransmitHolding(ch);
              // decrease transmited bytes number
              twim_tx_nb_bytes--;
            }
        }
      else if ((status & AVR32_TWIM_SR_SMBALERT_MASK) != 0 && ((imr
          & AVR32_TWIM_IMR_SMBALERT_MASK) != 0))
        {
          moduleRegisters.writeStatusClear(AVR32_TWIM_SR_SMBALERT_MASK);
        }
      else
        {
#if defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE)
          OSDeviceDebug::putChar('?');
#endif /* defined(DEBUG) && defined(OS_DEBUG_AVR32_UC3_TWIM_INTERUPTSERVICEROUTINE) */
        }
    }

  // --------------------------------------------------------------------------

  }
}

#endif /* defined(OS_INCLUDE_AVR32_UC3_TWIM) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
