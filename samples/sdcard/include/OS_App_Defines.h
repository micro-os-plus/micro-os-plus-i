/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// override the board definitions, if needed
//#define OS_CFGLONG_OSCILLATOR_HZ                      (3686400UL)

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
//#define OS_CFGINT_TIMER_PRESCALLER                      (64)
//#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)

#define OS_CFGINT_TASKS_TABLE_SIZE                      (3)

#define OS_INCLUDE_OS_BUSYWAITMILLIS                    1

#define OS_INCLUDE_TIMER                                1
#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS                  1

#define OS_INCLUDE_CIRCULARBYTEBUFFER                   1

#define OS_INCLUDE_OSDEVICECHARACTER                    1
#define OS_INCLUDE_DEVICECHARACTERMULTIUSART1           1
#define OS_INCLUDE_DEVICECHARACTERUSART                 1
#define OS_INCLUDE_DEVICECHARACTERUSB                   1

#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF                1

#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF            1

#define OS_INCLUDE_DEVICECHARACTER_TYPE                 1

#define OS_INCLUDE_OSTASK_GETSTACKUSED                  1
#define OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER             1

#define OS_CFGBOOL_DEVCHAR_USB_COM_CONTEXT_SWITCH       (true)

#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_SHORT            1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           1

#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_SIMPLECLI                            1

#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_ISTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTASK                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1
#define OS_INCLUDE_OSTREAM_LONG                         1

#define OS_CONFIG_USBINT_LED_PORT_INIT                  DDRD
#define OS_CONFIG_USBINT_LED_PORT                       PORTD
#define OS_CONFIG_USBINT_LED_BIT                        PORTD6

#define OS_INCLUDE_MEMCARD                              1
#define OS_INCLUDE_MEMCARD_VALIDATEVOLTAGE              1
#define OS_INCLUDE_MEMCARD_MEMBER_OCR                   1
#define OS_INCLUDE_MEMCARD_MEMBER_VHS                   1
#define OS_INCLUDE_MEMCARD_MEMBER_CID                   1
#define OS_INCLUDE_MEMCARD_MEMBER_CSD                   1

#define OS_INCLUDE_TASKBLINK                            1

#if defined(DEBUG)
// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_DEBUG_OSDEVICECHARACTER_OPEN                 1

#endif

#endif /*OS_APP_DEFINES_H_ */
