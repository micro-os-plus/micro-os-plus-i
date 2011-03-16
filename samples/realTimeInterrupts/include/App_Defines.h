/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

// ----------------------------------------------------------------------------

// Application versioning and greeting definitions
// More definitions will be computer in uOS.h

#define APP_CFGSTR_APPLICATION_NAME                     "realTimeInterrupts"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        1
#define APP_CFGINT_VERSION_MINOR                        2
#define APP_CFGINT_VERSION_REVISION                     1622

// ----------------------------------------------------------------------------

#if defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)

#define APP_CONFIG_LED_PORT             GPIOC
#define APP_CONFIG_LED_PORT_CONFIG      GPIOC->CRH
#define APP_CONFIG_LED_PORT_TOGGLE      GPIOC->ODR

#define APP_CONFIG_LED1                 10
#define APP_CONFIG_LED2                 9
#define APP_CONFIG_LED3                 8

#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#define APP_CONFIG_LED_PORT             PORTD
#define APP_CONFIG_LED_PORT_CONFIG      DDRD
#define APP_CONFIG_LED_PORT_TOGGLE      PORTD

#define APP_CONFIG_LED1                 7
#define APP_CONFIG_LED2                 6
#define APP_CONFIG_LED3                 5

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#define APP_CONFIG_LED_PORT             GPIOX
#define APP_CONFIG_LED_PORT_CONFIG      GPIOX
#define APP_CONFIG_LED_PORT_TOGGLE      GPIOX

#define APP_CONFIG_LED1                 AVR32_PIN_PX50
#define APP_CONFIG_LED2                 AVR32_PIN_PX54
#define APP_CONFIG_LED3                 AVR32_PIN_PX51

#define APP_CONFIG_LED_ISACTIVE_LOW     (1)


#else
#error "Missing OS_CONFIG_BOARD_* board definition"
#endif

#define APP_CFGINT_TASKBLINK_TICKS              (OS_CFGINT_TICK_RATE_HZ)
#define APP_CFGINT_TASKBLINKREALTIME_TICKS      (2666)

#define APP_CFGINT_DUMP_INTERVAL_SECONDS (10)
#define APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS (25)
#define APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS (300)

//#define APP_EXCLUDE_NAKED_ISR
//#define APP_INCLUDE_TASKBLINKREALTIME_ISRACTIONBUSYWAIT         1

//#define APP_EXCLUDE_ISR_ACTION
#define APP_INCLUDE_LENGTHEN_SYSTICK    1
//#define APP_INCLUDE_BUSY_WAIT           1
#define APP_BUSY_PROCENTAGE             40
#define APP_EVENT_DUMMY                 0x1111
#define APP_CFGINT_NOTIFIES             25

#define APP_CFGVAR_TASKBLINKREALTIME_TIMER              (AVR32_TC1)
#define APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL       (0)
#define APP_CFGINT_TASKBLINKREALTIME_TIMERIRQID         (AVR32_TC1_IRQ0)
#define APP_CFGINT_TASKBLINKREALTIME_TIMERIRQLEVEL      (3)
#define APP_CFGINT_TASKBLINKREALTIME_TIMERCLOCKSELECT   (3)
#define APP_CFGINT_TASKBLINKREALTIME_TIMERPRESCALLER    (4)

#define APP_CFGINT_TASKBLINKREALTIME_EVENT              (0x4321)

#define APP_CFGINT_TASKBLINK_LEDBIT                     APP_CONFIG_LED1
#define APP_CFGINT_TASKBLINKREALTIME_LEDBIT             APP_CONFIG_LED2

#define APP_CFGREG_TASKBLINKREALTIME_ISRLEDPORTCONFIG   OS_CONFIG_ACTIVE_LED_PORT_CONFIG
#define APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT          APP_CONFIG_LED3

//#define APP_EXCLUDE_TASKBLINKREALTIME_TASKMAIN_LED      (1)

// ----------------------------------------------------------------------------

#endif /* APP_DEFINES_H_ */
