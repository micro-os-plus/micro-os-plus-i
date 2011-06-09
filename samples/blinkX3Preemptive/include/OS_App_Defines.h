/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (3)

// Only for tests, when ready should not be used
//#define OS_EXCLUDE_PREEMPTION                           (1)

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    1

// ----- Board Definitions ----------------------------------------------------

#if defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#define OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK          (1)
#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)

#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#endif /* OS_CONFIG_BOARD_* */

// ----- Debug Definitions ----------------------------------------------------

#if defined(DEBUG)
// One of the following must be defined in Eclipse project or Makefile
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS       (1)

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
