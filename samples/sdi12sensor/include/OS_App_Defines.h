/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// override the board definitions, if needed
//#define OS_CFGLONG_OSCILLATOR_HZ                        (7372800UL)

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
//#define OS_CFGINT_TIMER_PRESCALLER                      (64)
//#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (3)

#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS			1
#define OS_INCLUDE_32KHZ_TIMER                          1

#define OS_INCLUDE_OSTASK_SLEEP                         1
#define OS_INCLUDE_OSTASK_GETSTACKUSED			1

#define OS_INCLUDE_SDI12SENSOR                          1

#define OS_INCLUDE_OSTASK_VIRTUALWATCHDOG              1
#define OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS         (5)

#define OS_INCLUDE_OSTIMERSECONDS_UPTIME                1

#define OS_EXCLUDE_PCINT3_VECT                          1

#define OS_INCLUDE_OSEVENTFLAGS                         1
#define OS_INCLUDE_OSEVENTFLAGS_WORD                    1

#if defined(DEBUG)
// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1
#endif /*DEBUG*/

#endif /*OS_APP_DEFINES_H_ */
