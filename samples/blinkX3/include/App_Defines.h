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

#define APP_CFGSTR_APPLICATION_NAME                     "blinkX3"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        2
#define APP_CFGINT_VERSION_MINOR                        1
#define APP_CFGINT_VERSION_REVISION                     1332

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
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

// ----------------------------------------------------------------------------

#endif /* APP_DEFINES_H_ */
