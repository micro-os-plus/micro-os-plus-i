/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

// 644p variant includes 324p and 164p

#define OS_CONFIG_ARCH_AVR8                              1
#define OS_CONFIG_FAMILY_ATMEGA                         1
#define OS_CONFIG_VARIANT_ATMEGA644P                    1
#if !defined(OS_CONFIG_BOARD_METRILOG_M514)
#define OS_CONFIG_BOARD_METRILOG_M514			1
#endif

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ			(7372800UL)
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

#include "hal/boards/Metrilog/include/DebugLeds_B6B7B5_Defines.h"

#endif /*HAL_BOARD_OS_DEFINES_H_*/
