/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

// ----------------------------------------------------------------------------

#define OS_CONFIG_ARCH_AVR8                             1
#define OS_CONFIG_FAMILY_AT90USB			1
#define OS_CONFIG_VARIANT_AT90USB1287                   1
#if !defined(OS_CONFIG_BOARD_ATMEL_STK525)
#define OS_CONFIG_BOARD_ATMEL_STK525			1
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ			(8000000UL)
#endif
#if !defined(OS_CFGINT_CLOCK_PRESCALER)
#define OS_CFGINT_CLOCK_PRESCALER			(1)
#endif

#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ                          (1000)
#endif

#if !defined(OS_CFGINT_TIMER_PRESCALLER)
#define OS_CFGINT_TIMER_PRESCALLER                      (64)
#endif
#if !defined(OS_CFGINT_TIMER_CLOCK_SELECT)
#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG                DDRD
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT)
#define OS_CONFIG_ACTIVE_LED_PORT			PORTD
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			PORTD4
#endif

// ----------------------------------------------------------------------------

// TODO: update PORT_INIT to PORT_CONFIG instead
#if !defined(OS_CONFIG_USB_LED_PORT_INIT)
#define OS_CONFIG_USB_LED_PORT_INIT                     DDRD
#endif
#if !defined(OS_CONFIG_USB_LED_PORT)
#define OS_CONFIG_USB_LED_PORT                          PORTD
#endif
#if !defined(OS_CONFIG_USB_LED_BIT)
#define OS_CONFIG_USB_LED_BIT                           PORTD7
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_CONFIG_BLINK_LED_PORT_INIT)
#define OS_CONFIG_BLINK_LED_PORT_INIT                   DDRD
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT)
#define OS_CONFIG_BLINK_LED_PORT                        PORTD
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT                         5
#endif

#if !defined(OS_CONFIG_DEBUG_LED1_PORT)
#define OS_CONFIG_DEBUG_LED1_PORT			PORTD
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT_INIT)
#define OS_CONFIG_DEBUG_LED1_PORT_INIT                  DDRD
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT			PORTD5
#endif

#if !defined(OS_CONFIG_DEBUG_LED2_PORT)
#define OS_CONFIG_DEBUG_LED2_PORT			PORTD
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT_INIT)
#define OS_CONFIG_DEBUG_LED2_PORT_INIT                  DDRD
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT			PORTD6
#endif

// ----------------------------------------------------------------------------

#endif /* HAL_BOARD_OS_DEFINES_H_ */
