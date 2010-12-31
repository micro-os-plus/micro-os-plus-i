/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_OS_CONFIG_H_
#define APP_OS_CONFIG_H_

// override the board definitions, if needed
//#define OS_CFGLONG_OSCILLATOR_HZ                        (8000000UL)

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
//#define OS_CFGINT_TIMER_PRESCALLER                      (64)
//#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (3)

#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS                  1

#define OS_INCLUDE_CIRCULARBYTEBUFFER                   1

#define OS_CFGINT_DEVICECHARACTERMULTIUSART1_BAUD_RATE  (19200)

//#define OS_INCLUDE_OSDEVICECHARACTER                    1
#define OS_INCLUDE_OSDEVICECHARACTERMULTIUSART1         1
#define OS_INCLUDE_OSDEVICECHARACTERUSB                 1

#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1

#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF          1

#define OS_INCLUDE_OSTASK_GETSTACKUSED                  1
#define OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER             1

#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           1

#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_SIMPLECLI                            1

#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_ISTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTASK                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1

#define OS_INCLUDE_TASKPITPALAC                         1

#if defined(DEBUG)
// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1
#define OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE           1

#endif /* DEBUG */

#endif /*APP_OS_CONFIG_H_ */
