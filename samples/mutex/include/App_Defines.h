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

#define APP_CFGSTR_APPLICATION_NAME                     "mutex"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        1
#define APP_CFGINT_VERSION_MINOR                        1
#define APP_CFGINT_VERSION_REVISION                     1877

// ----------------------------------------------------------------------------

//extern void delayWithYield(schedTicks_t n);

#if defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)

#define APP_CONFIG_LED_PORT             GPIOC
#define APP_CONFIG_LED_PORT_CONFIG      GPIOC->CRH
#define APP_CONFIG_LED_PORT_TOGGLE      GPIOC->ODR

#define APP_CONFIG_LED1                 10
#define APP_CONFIG_LED2                 9
#define APP_CONFIG_LED3                 8

#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#define APP_CONFIG_LED_PORT_WRITE       PORTD
#define APP_CONFIG_LED_PORT_CONFIG      DDRD
#define APP_CONFIG_LED_PORT_TOGGLE      PORTD

#define APP_CONFIG_LED1                 7
#define APP_CONFIG_LED2                 6
#define APP_CONFIG_LED3                 5

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#define APP_CONFIG_LED_PORT_WRITE       GPIOX
#define APP_CONFIG_LED_PORT_CONFIG      GPIOX
#define APP_CONFIG_LED_PORT_TOGGLE      GPIOX

#define APP_CONFIG_LED1                 AVR32_PIN_PX50
#define APP_CONFIG_LED2                 AVR32_PIN_PX54
#define APP_CONFIG_LED3                 AVR32_PIN_PX51

#define APP_CONFIG_LED_ISACTIVE_LOW     (1)


#else
#error "Missing OS_CONFIG_BOARD_* board definition"
#endif

#define APP_CFGINT_BLINK_TICKS          (OS_CFGINT_TICK_RATE_HZ * 3)
#define APP_CFGINT_NESTED_TICKS         (APP_CFGINT_BLINK_TICKS * 7)

#define APP_INCLUDE_DUMP_TASKS          1
#define APP_CFGINT_DUMP_INTERVAL_SECONDS (10)
#define APP_CFGINT_DUMP_INCREASE_RATE_PROCENTS (25)
#define APP_CFGINT_DUMP_MAX_INTERVAL_SECONDS (300)

//#define APP_EXCLUDE_NAKED_ISR
#define APP_ISR_ACTION_BUSYWAIT         1
//#define APP_EXCLUDE_ISR_ACTION
//#define APP_INCLUDE_LENGTHEN_SYSTICK    1
//#define APP_INCLUDE_BUSY_WAIT           1
#define APP_BUSY_PROCENTAGE             40
#define APP_EVENT_DUMMY                 0x1111
#define APP_CFGINT_NOTIFIES             5

#define APP_CFGINT_TASKBLINK_LEDBIT                     APP_CONFIG_LED1

// the maximum number of tasks
#define APP_CONFIG_TASKS_NUM_MAX                         10
#define APP_CONFIG_ERROR_CODE                            (-1)

#endif /* APP_DEFINES_H_ */
