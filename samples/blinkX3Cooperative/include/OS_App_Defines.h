/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----- Portable Definitions -------------------------------------------------

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (3)

// Mandatory here, since we test the cooperative multitasking
#define OS_EXCLUDE_PREEMPTION                           (1)

// First time we test with standard timer interrupts, to avoid
// context save/restore.
// After this works properly, proceed with naked interrupts.
//#define OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR               (1)

// ----- Board Definitions ----------------------------------------------------

#if defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#define OS_EXCLUDE_OSCRITICALSECTION_USE_STACK          (1)
#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)

#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#endif /* OS_CONFIG_BOARD_* */

// ----- Debug Definitions ----------------------------------------------------

#if defined(DEBUG)

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS          (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_THREADREGISTER               (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)

#endif /* DEBUG */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
