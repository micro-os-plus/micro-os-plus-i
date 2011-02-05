/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_OS_DEFINES_H_
#define APP_OS_DEFINES_H_

// override the board definitions, if needed
#define OS_CFGLONG_OSCILLATOR_HZ                        (16000000UL)

#define OS_CFGINT_TICK_RATE_HZ                          (1000)
#define OS_CFGINT_TIMER_PRESCALLER                      (4)
#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (3)

//#define OS_EXCLUDE_PREEMPTION                           (1)

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    (1)
#define OS_INCLUDE_OS_BUSYWAITMICROS                    (1)

//#define OS_INCLUDE_OSTASK_SCHEDULERTICK                 (1)
#define OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK     (1)

#define OS_TEST_PHASE 2

#if OS_TEST_PHASE == 1

#define OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED            (1)
#define OS_CONFIG_OSTIMERTICKS_LED_BIT                  AVR32_PIN_PX50

#else

#endif


//#define OS_INCLUDE_SYSTICK_CONTEXT_HANDLER_UNDER_CONSTRUCTION  (1)
//#define OS_INCLUDE_OSSCHEDULER_YIELD_UNDER_CONSTRUCTION (1)

#if defined(DEBUG)
// One of the following must be defined in Eclipse project or Makefile
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_INCLUDE_OSSCHEDULER_TIMER_MARK_SECONDS       (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_TASKREGISTER               (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)

#endif /*DEBUG*/

#endif /*APP_OS_DEFINES_H_ */
