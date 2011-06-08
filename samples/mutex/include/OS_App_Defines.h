/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----- Portable Definitions -------------------------------------------------

#define OS_CFGINT_TICK_RATE_HZ                          (1000)

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (12)

#define OS_INCLUDE_OSTIMERSECONDS                       (1)

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    (1)
#define OS_INCLUDE_OS_BUSYWAITMICROS                    (1)

#define OS_CFGINT_IDLE_THREAD_STACK_SIZE                  (800)

//#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1
//#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                  1
//#define OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER             1
#define OS_INCLUDE_CLOG                                 1
//#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTHREAD                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1

// used for activating the mutex mechanism
#define OS_INCLUDE_OSMUTEX                              (1)

// ----- Test phase -----------------------------------------------------------
#define TEST_PHASE 3

#if TEST_PHASE == 1

// Phase 1
// Default configuration. The first threads are favourised.

#elif TEST_PHASE == 2

// Phase 2
// Use mutex local round robin configuration.
// It should give quite good spread.

#define OS_INCLUDE_OSMUTEX_WAITING_THREADS              (1)

#elif TEST_PHASE == 3

// Phase 3
// Use global scheduler round robin.
// It should give almost perfect good spread.

#define OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY       (1)

#endif

// ----- Board Definitions ----------------------------------------------------

#if defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

//#define OS_EXCLUDE_PREEMPTION                           (1)
//#define OS_EXCLUDE_OSCRITICALSECTION_USE_STACK          (1)
#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)
#define OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS (1)
#define OS_INCLUDE_OSSCHEDULER_CRITICALENTER_WITH_MASK  (1)

#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#endif /* OS_CONFIG_BOARD_* */

// ----- Debug Definitions ----------------------------------------------------

#if defined(DEBUG)

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS       (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSTIMER_SLEEP                          (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_THREADREGISTER               (1)
//#define OS_DEBUG_OSSCHEDULER_EVENTWAIT                  (1)
//#define OS_DEBUG_OSSCHEDULER_EVENTNOTIFY                (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSREADYLIST_REMOVE                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)

//#define OS_DEBUG_OSMUTEXWAITINGTASKS                    (1)

#endif /* defined(DEBUG) */

#endif /* OS_APP_DEFINES_H_ */
