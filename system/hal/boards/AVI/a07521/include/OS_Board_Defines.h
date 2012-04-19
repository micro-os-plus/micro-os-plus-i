/*
 *	Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

#define OS_CONFIG_ARCH_AVR8                              1
#define OS_CONFIG_FAMILY_AT90CAN			1
#define OS_CONFIG_VARIANT_AT90CAN128                    1
#if !defined(OS_CONFIG_BOARD_AVI_A07521)
#define OS_CONFIG_BOARD_AVI_A07521			1
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
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG                  DDRA
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_WRITE)
#define OS_CONFIG_ACTIVE_LED_PORT_WRITE			PORTA
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			7
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_CONFIG)
#define OS_CONFIG_BLINK_LED_PORT_CONFIG                   DDRA
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT_WRITE)
#define OS_CONFIG_BLINK_LED_PORT_WRITE                        PORTA
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT                         6
#endif

#include "hal/boards/AVI/include/DebugLeds_E1B1E0_Defines.h"

#endif /* HAL_BOARD_OS_DEFINES_H_ */
