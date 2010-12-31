/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

#define OS_CONFIG_ARCH_AVR                              1
#define OS_CONFIG_FAMILY_AT90USB			1
#define OS_CONFIG_VARIANT_AT90USB1287                   1
#if !defined(OS_CONFIG_BOARD_A0739)
#define OS_CONFIG_BOARD_A0739				1
#endif

#define OS_CONFIG_ACTIVE_LED_PORT_INIT                  DDRD
#define OS_CONFIG_ACTIVE_LED_PORT			PORTD
#define OS_CONFIG_ACTIVE_LED_BIT			PORTD4

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ			(8000000UL)
#endif
#if !defined(OS_CFGINT_CLOCK_PRESCALER)
#define OS_CFGINT_CLOCK_PRESCALER			(1)
#endif

#if !defined(OS_CONFIG_ACTIVE_LED_PORT_INIT)
#define OS_CONFIG_ACTIVE_LED_PORT_INIT                  DDRD
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT)
#define OS_CONFIG_ACTIVE_LED_PORT			PORTD
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			PORTD4
#endif

#if !defined(OS_CONFIG_USB_LED_PORT_INIT)
#define OS_CONFIG_USB_LED_PORT_INIT                     DDRD
#endif
#if !defined(OS_CONFIG_USB_LED_PORT)
#define OS_CONFIG_USB_LED_PORT                          PORTD
#endif
#if !defined(OS_CONFIG_USB_LED_BIT)
#define OS_CONFIG_USB_LED_BIT                           PORTD7
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_INIT)
#define OS_CONFIG_BLINK_LED_PORT_INIT                   DDRD
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT)
#define OS_CONFIG_BLINK_LED_PORT                        PORTD
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT                         PORTD5
#endif

#if !defined(OS_CONFIG_DEBUG_LED1_PORT_INIT)
#define OS_CONFIG_DEBUG_LED1_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT)
#define OS_CONFIG_DEBUG_LED1_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT                        PINB3
#endif

#if !defined(OS_CONFIG_DEBUG_LED2_PORT_INIT)
#define OS_CONFIG_DEBUG_LED2_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT)
#define OS_CONFIG_DEBUG_LED2_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT                        PINB1
#endif

#if !defined(OS_CONFIG_DEBUG_LED3_PORT_INIT)
#define OS_CONFIG_DEBUG_LED3_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED3_PORT)
#define OS_CONFIG_DEBUG_LED3_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED3_BIT)
#define OS_CONFIG_DEBUG_LED3_BIT                        PINB2
#endif

#endif /*HAL_BOARD_OS_DEFINES_H_*/
