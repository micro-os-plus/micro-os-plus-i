/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OS_DEFS_H_
#define HAL_ARCH_OS_DEFS_H_

#if !defined (OS_BUILD_ASSEMBLY_CODE)
typedef unsigned long OSStack_t;
typedef unsigned long* pfnVector_t;

typedef unsigned long OSResetBits_t;
typedef void* OSProgramPtr_t;

typedef unsigned long OSInterruptsMask_t;

#endif

#define OS_INCLUDE_CSTDDEF                              (1)

#define PROGMEM
#define PSTR(_S_) ((const char*)_S_)
#define putString_P putString
#define putConstructor_P putConstructor
#define putDestructor_P putDestructor

#define OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG            (1)

#define OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_LONG            (1)
#define OS_INCLUDE_OSDEVICEDEBUG_PUTHEX_INT             (1)

#define OS_INLINE_INTERRUPTS_ENABLE_DISABLE             (1)

//#define OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT      (1)
#define OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING   (1)

// AVR32 compiler messes SP and R7 when optimisation level is not -O0
// so it is not possible to use the system stack
#define OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK   (1)

#if !defined(OS_CFGINT_STACK_MINIMAL_SIZE)
#define OS_CFGINT_STACK_MINIMAL_SIZE                    (400)
#endif

#endif /* HAL_ARCH_OS_DEFS_H_ */
