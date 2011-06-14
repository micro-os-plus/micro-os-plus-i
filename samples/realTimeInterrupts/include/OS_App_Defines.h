/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// override the board definitions, if needed
//#define OS_CFGLONG_OSCILLATOR_HZ                        (16000000UL)

#define OS_CFGINT_TICK_RATE_HZ                          (1000)
#define OS_CFGINT_TIMER_PRESCALLER                      (4)
#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (4)

#define OS_INCLUDE_OSTIMERSECONDS                       (1)
#define OS_INCLUDE_TASKREPORTSTACKS                     (1)

//#define OS_EXCLUDE_PREEMPTION                           (1)
//#define OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION          (1)

//#define OS_INCLUDE_OSSCHEDULER_YIELD_MASK_INTERRUPTS  (1)
//#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)
// do not disable INT3
#define OS_CFGINT_OSCRITICALSECTION_MASK                (0x7 << (16+1))

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    (1)
#define OS_INCLUDE_OS_BUSYWAITMICROS                    (1)

//#define OS_INCLUDE_OSTHREAD_SCHEDULERTICK                 (1)
#define OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK     (1)

//#define OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL  (1)

//#define OS_INCLUDE_OSTIMERTICKS_IMPLINIT_TIMERCOUNTER   (1)

#define OS_CFGINT_IDLE_THREAD_STACK_SIZE                  (800)

#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1
#define OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG            (1)

#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                  1
//#define OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER             1
#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTHREAD                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1

#define OS_INCLUDE_OSREALTIME                           (1)

//#define OSTHREAD_NOTIFY_MEASURE                           1

#define OS_APP_CONFIG_LED3                 AVR32_PIN_PX51

#define OS_TEST_PHASE 2

#if OS_TEST_PHASE == 1

#define OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED            (1)
#define OS_CONFIG_OSTIMERTICKS_LED_BIT                  AVR32_PIN_PX50

#else

#endif

//#define OS_INCLUDE_SYSTICK_CONTEXT_HANDLER_UNDER_CONSTRUCTION  (1)
//#define OS_INCLUDE_OSSCHEDULER_YIELD_UNDER_CONSTRUCTION (1)

#if defined(DEBUG)

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS          (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_THREADREGISTER               (1)
//#define OS_DEBUG_OSSCHEDULER_EVENTWAIT                  (1)
//#define OS_DEBUG_OSSCHEDULER_EVENTNOTIFY                (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSREADYLIST_REMOVE                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)
//#define OS_DEBUG_OSTIMER_INTERRUPTTICK                  (1)

#endif /* defined(DEBUG) */

#endif /* OS_APP_DEFINES_H_ */
