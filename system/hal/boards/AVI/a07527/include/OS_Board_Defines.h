/*
 *	Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

#define OS_CONFIG_ARCH_AVR8                              1
#define OS_CONFIG_FAMILY_AT90USB			1
#define OS_CONFIG_VARIANT_AT90USB1287                   1
#if !defined(OS_CONFIG_BOARD_AVI_A07527)
#define OS_CONFIG_BOARD_AVI_A07527			1
#endif

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ			(16000000UL)
#endif
#if !defined(OS_CFGINT_CLOCK_PRESCALER)
#define OS_CFGINT_CLOCK_PRESCALER			(1)
#endif

#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ                          (1024)
#endif

#if !defined(OS_CFGINT_TIMER_PRESCALLER)
#define OS_CFGINT_TIMER_PRESCALLER                      (64)
#endif
#if !defined(OS_CFGINT_TIMER_CLOCK_SELECT)
#define OS_CFGINT_TIMER_CLOCK_SELECT                    (3)
#endif

#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG                DDRA
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_WRITE)
#define OS_CONFIG_ACTIVE_LED_PORT_WRITE			PORTA
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			7
#endif

#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG)
#define OS_CFGREG_OSUSBLED_LED_PORT_CONFIG                     DDRA
#endif
#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE)
#define OS_CFGREG_OSUSBLED_LED_PORT_WRITE                          PORTA
#endif
#if !defined(OS_CFGPIN_OSUSBLED_LED_PIN)
#define OS_CFGPIN_OSUSBLED_LED_PIN                           2
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_CONFIG)
#define OS_CONFIG_BLINK_LED_PORT_CONFIG                 DDRA
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT_WRITE)
#define OS_CONFIG_BLINK_LED_PORT_WRITE                  PORTA
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT                         6
#endif

#include "hal/boards/AVI/include/DebugLeds_B3B1B2_Defines.h"

#endif /* HAL_BOARD_OS_DEFINES_H_ */
