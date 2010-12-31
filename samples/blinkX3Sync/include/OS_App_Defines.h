/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_OS_DEFINES_H_
#define APP_OS_DEFINES_H_

// override the board definitions, if needed
//#define OS_CFGLONG_OSCILLATOR_HZ                        (8000000UL)

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
//#define OS_CFGINT_TIMER_PRESCALLER                      (64)
//#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (4)

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    1

#if defined(DEBUG)
// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1
#endif /*DEBUG*/

#endif /*APP_OS_DEFINES_H_ */
