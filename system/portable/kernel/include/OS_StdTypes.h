/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OS_STDTYPES_H_
#define OS_STDTYPES_H_

// ----------------------------------------------------------------------------

#include "portable/kernel/include/OS_Defines.h"

// ----------------------------------------------------------------------------
// First of all define all usual types.
#include <stdint.h>
// From now on, we can use:
//      uint8_t, int8_t
//      uint16_t, int16_t
//      uint32_t, int32_t

// This one are probably less standard, but quite useful
typedef unsigned char uchar_t;

#if !defined(OS_CONFIG_ARCH_AVR8)

typedef unsigned int uint_t;

#else /* defined(OS_CONFIG_ARCH_AVR8) */

// The above does not work on AVR8, overloading triggers error
#if (__SIZEOF_INT__ == 2)
#define uint_t uint16_t
#elif (__SIZEOF_INT__ == 4)
#define uint_t uint16_t
#else
#error "Unsupported size of int"
#endif

#endif /* !defined(OS_CONFIG_ARCH_AVR8) */

typedef unsigned long ulong_t;

#if defined(OS_INCLUDE_CSTDDEF)
#include <cstddef>
#else
namespace std
{
  typedef unsigned int size_t;
}
#endif
// From now on, we can use:
//      std::size_t

// ----------------------------------------------------------------------------

#endif /* OS_STDTYPES_H_ */
