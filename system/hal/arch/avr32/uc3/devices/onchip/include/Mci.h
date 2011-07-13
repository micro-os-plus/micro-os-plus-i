/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_MCI_H_
#define AVR32_UC3_MCI_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Mci.h"

namespace avr32
{
  namespace uc3
  {
    namespace mci
    {
      // ----- Type definitions -----------------------------------------------

      // ----- Port memory mapped registers -----------------------------------

      class PortRegisters
      {
      public:
        // --------------------------------------------------------------------

        const static uint32_t MEMORY_ADDRESS =
            avr32::uc3::PeripheralAddressMap::MCI;
        //const static uint32_t MEMORY_OFFSET = 0x100;

        // ----- Memory map ---------------------------------------------------

        regWriteOnly_t cr; //0x0000
        regReadWrite_t mr; //0x0004
        regReadWrite_t dtor; //0x0008
        regReadWrite_t sdcr; //0x000C

        regReadWrite_t argr; //0x0010
        regWriteOnly_t cmdr; //0x0014
        regReadWrite_t blkr; //0x0018
        regReadWrite_t cstor; //0x001C

        regReadOnly_t rspr; //0x0020
        regReadOnly_t rspr1; //0x0024
        regReadOnly_t rspr2; //0x0028
        regReadOnly_t rspr3; //0x002C

        regReadOnly_t rdr; //0x0030
        regWriteOnly_t tdr; //0x0034
        regNotAllocated_t dummy38[2]; //0x0038-0x003C

        regReadOnly_t sr; //0x0040
        regWriteOnly_t ier; //0x0044
        regWriteOnly_t idr; //0x0048
        regReadOnly_t imr; //0x004C

        regReadWrite_t dma; //0x0050
        regReadWrite_t cfg; //0x0054
        regNotAllocated_t dummy58[35]; //0x0058-0x00E0

        regReadWrite_t wpmr; //0x00E4
        regReadOnly_t wpsr; //0x00E8
        regNotAllocated_t dummyEC[4]; //0x00EC-0x00F8

        regReadOnly_t version; //0x00FC
      }
    }

    class Mci
    {
    public:
      Mci();

    public:
      volatile mci::PortRegisters& portRegisters;


    };
  }
}
#endif /* AVR32_UC3_MCI_H_ */
