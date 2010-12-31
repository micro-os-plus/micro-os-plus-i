/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_OS_DEFINES_H_
#define APP_OS_DEFINES_H_

// place holder to override the board definitions, if needed

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
//#define OS_CFGINT_TIMER_PRESCALLER                      (64)
//#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (2)

#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS             (1)

#define OS_INCLUDE_TIMER                                (1)
#define OS_INCLUDE_TASKBLINK                            (1)
#define OS_INCLUDE_TASKPITPALAC                         (1)

#if defined(DEBUG)

// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                      1
//#define OS_CONFIG_DEBUG_DEVICE_USART                    1

#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_EVENTWAIT                  (1)
//#define OS_DEBUG_OSTIMER_SLEEP                          (1)
//#define OS_DEBUG_OSTIMER_INSERT                         (1)
//#define OS_DEBUG_OSTIMER_INTERRUPTTICK                  (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSREADYLIST_REMOVE                     (1)
#define OS_DEBUG_TASKBLINK_DOT                          (1)

#endif

#endif /*APP_OS_DEFINES_H_ */
