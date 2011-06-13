/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

#define OS_CFGINT_THREADS_ARRAY_SIZE                    (2)

#define OS_INCLUDE_OSTIMERSECONDS                       (1)

#define OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK          (1)
#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)
#define OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS (1)
#define OS_INCLUDE_OSSCHEDULER_CRITICALENTER_WITH_MASK  (1)

#define OS_INCLUDE_AVR32_UC3_SPI                        (1)
#define OS_INCLUDE_AVR32_UC3_PDCA                       (1)
#define OS_INCLUDE_AVR32_UC3_TWIM                       (1)
#define OS_INCLUDE_AVR32_UC3_MCI                        (1)
#define OS_INCLUDE_AVR32_UC3_GPIO                       (1)
#define OS_INCLUDE_AVR32_UC3_ADC                        (1)
#define OS_INCLUDE_AVR32_UC3_RTC                        (1)
#define OS_INCLUDE_AVR32_UC3_INTC                       (1)

// ----------------------------------------------------------------------------

#if defined(DEBUG)

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS       (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_THREADREGISTER               (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)
#define OS_DEBUG_PDCA                                 (1)
#endif /* DEBUG */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
