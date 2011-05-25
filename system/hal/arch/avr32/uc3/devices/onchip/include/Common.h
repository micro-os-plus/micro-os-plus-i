/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef AVR32_UC3_COMMON_H_
#define AVR32_UC3_COMMON_H_

//#include "portable/kernel/include/OS.h"

namespace avr32
{
  namespace uc3
  {
    typedef uint32_t volatile regReadWrite_t;
    typedef uint32_t const volatile regReadOnly_t;
    typedef uint32_t regWriteOnly_t;
    typedef uint32_t regNotAllocated_t;
  }
}

#endif /* AVR32_UC3_COMMON_H_ */
