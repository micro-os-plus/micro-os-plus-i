/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_USBB_DEFINITIONS_H_
#define AVR32_UC3_USBB_DEFINITIONS_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

namespace avr32
{
  namespace uc3
  {
    namespace usbb
    {

      // ----- Type definitions -----------------------------------------------

      typedef uint16_t GeneralStatus_t;

      // ----- Port memory mapped registers -----------------------------------

      class ModuleRegisters
      {
      public:

        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::USB;

        // ----- Memory map ---------------------------------------------------

        regReadWrite_t udcon; //0x0000
        regReadOnly_t udint; //0x0004
        regWriteOnly_t udintclr; //0x0008
        regWriteOnly_t udintset; //0x000C
        regReadOnly_t udinte; //0x0010
        regWriteOnly_t udinteclr; //0x0014
        regWriteOnly_t udinteset; //0x0018
        regReadWrite_t uerst; // 0x001C
        regReadOnly_t udfnum; // 0x0020

        // Many more, currently not used
        regNotAllocated_t
            dummy24[(0x0800 - 0x0024) / sizeof(regNotAllocated_t)]; //0x0024-0x07FC

        regReadWrite_t usbcon; // 0x0800
        regReadOnly_t usbsta; // 0x0804
        regWriteOnly_t usbstaclr; //0x0808
        regWriteOnly_t usbstaset; //0x080C
        regReadOnly_t uvers; // 0x0818
        regReadOnly_t ufeatures; // 0x081C
        regReadOnly_t uaddrsize; // 0x0820
        regReadOnly_t uname[2]; // 0x0824
        regReadOnly_t usbfsm; // 0x082C


        // ----- General Methods ----------------------------------------------

        void
        writeGeneralControl(uint32_t mask);

        GeneralStatus_t
        readGeneralControl(void);

        GeneralStatus_t
        readGeneralStatus(void);

        void
        writeGeneralStatusClear(GeneralStatus_t mask);

        void
        writeGeneralStatusSet(GeneralStatus_t mask);

        uint32_t
        readVersion(void);

        uint32_t
        readFeatures(void);

        uint32_t
        readAddressSize(void);

        uint32_t
        readName(uint_t index);

        uint8_t
        readFiniteStateMachineStatus(void);

        // ----- Device Methods ----------------------------------------------

        uint32_t
        readDeviceGeneralControl(void);

        void
        writeDeviceGeneralControl(uint32_t mask);

        uint32_t
        readDeviceGlobalInterrupt(void);

        // Writing 1 to a bit will clear UDINT; writing 0 will be ignored
        void
        writeDeviceGlobalInterruptClear(uint32_t value);

        // Writing 1 to a bit will set UDINT; writing 0 will be ignored
        void
        writeDeviceGlobalInterruptSet(uint32_t value);

        uint32_t
        readDeviceGlobalInterruptEnable(void);

        // Writing 1 to a bit will clear UDINTE; writing 0 will be ignored
        void
        writeDeviceGlobalInterruptEnableClear(uint32_t value);

        // Writing 1 to a bit will set UDINTE; writing 0 will be ignored
        void
        writeDeviceGlobalInterruptEnableSet(uint32_t value);

        uint32_t
        readEndpointEnableReset(void);

        void
        writeEndpointEnableReset(uint32_t value);

        uint16_t
        readDeviceFrameNumber(void);

      };

      // ----- Inline methods -------------------------------------------------

      inline void
      ModuleRegisters::writeGeneralControl(uint32_t mask)
      {
        this->usbcon = mask;
      }

      inline GeneralStatus_t
      ModuleRegisters::readGeneralControl(void)
      {
        return this->usbcon;
      }

      inline GeneralStatus_t
      ModuleRegisters::readGeneralStatus(void)
      {
        return this->usbsta;
      }

      inline void
      ModuleRegisters::writeGeneralStatusClear(GeneralStatus_t mask)
      {
        this->usbstaclr = mask;
      }

      inline void
      ModuleRegisters::writeGeneralStatusSet(GeneralStatus_t mask)
      {
        this->usbstaset = mask;
      }

      inline uint32_t
      ModuleRegisters::readVersion(void)
      {
        return this->uvers;
      }

      inline uint32_t
      ModuleRegisters::readFeatures(void)
      {
        return this->ufeatures;
      }

      inline uint32_t
      ModuleRegisters::readAddressSize(void)
      {
        return this->uaddrsize;
      }

      inline uint32_t
      ModuleRegisters::readName(uint_t index)
      {
        return this->uname[index];
      }

      inline uint8_t
      ModuleRegisters::readFiniteStateMachineStatus(void)
      {
        return this->usbfsm;
      }

      // -----

      inline uint32_t
      ModuleRegisters::readDeviceGeneralControl(void)
      {
        return this->udcon;
      }

      inline void
      ModuleRegisters::writeDeviceGeneralControl(uint32_t mask)
      {
        this->udcon = mask;
      }

      inline uint32_t
      ModuleRegisters::readDeviceGlobalInterrupt(void)
      {
        return this->udint;
      }

      // Writing 1 to a bit will clear UDINT; writing 0 will be ignored
      inline void
      ModuleRegisters::writeDeviceGlobalInterruptClear(uint32_t value)
      {
        this->udintclr = value;
      }

      // Writing 1 to a bit will set UDINT; writing 0 will be ignored
      inline void
      ModuleRegisters::writeDeviceGlobalInterruptSet(uint32_t value)
      {
        this->udintset = value;
      }

      inline uint32_t
      ModuleRegisters::readDeviceGlobalInterruptEnable(void)
      {
        return this->udinte;
      }

      // Writing 1 to a bit will clear UDINTE; writing 0 will be ignored
      inline void
      ModuleRegisters::writeDeviceGlobalInterruptEnableClear(uint32_t value)
      {
        this->udinteclr = value;
      }

      // Writing 1 to a bit will set UDINTE; writing 0 will be ignored
      inline void
      ModuleRegisters::writeDeviceGlobalInterruptEnableSet(uint32_t value)
      {
        this->udinteset = value;
      }

      inline uint32_t
      ModuleRegisters::readEndpointEnableReset(void)
      {
        return this->uerst;
      }

      inline void
      ModuleRegisters::writeEndpointEnableReset(uint32_t value)
      {
        this->uerst = value;
      }

      inline uint16_t
      ModuleRegisters::readDeviceFrameNumber(void)
      {
        return this->udfnum;
      }

    // ----------------------------------------------------------------------

    }
  }
}

#endif /* AVR32_UC3_USBB_DEFINITIONS_H_ */
