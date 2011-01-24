/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OS_ARCH_DEFS_H_
#define HAL_OS_ARCH_DEFS_H_

typedef unsigned char OSStack_t;

#include "avr/pgmspace.h"

typedef PGM_VOID_P OSProgramPtr_t;

#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "avr/power.h"
#include "avr/sleep.h"
#include "avr/eeprom.h"

#undef TIMEOUT

#if !defined(OS_CFGINT_STACK_MINIMAL_SIZE)
#define OS_CFGINT_STACK_MINIMAL_SIZE  (85)
#endif

#if !defined(OS_CFGINT_BUSYWAIT_CALIBRATION)
#if defined(DEBUG)
/* -O1, since -O0 does not work */
#define OS_CFGINT_BUSYWAIT_CALIBRATION  (9293)
#else
/* -O3 */
#define OS_CFGINT_BUSYWAIT_CALIBRATION	(9025)
#endif
#endif

#define OS_S1B(_BIT)            (0x1<<(_BIT))
#define OS_SVB(_BIT,_VALUE)     (((_VALUE) & 0x1) << (_BIT))
//#define OS_CFV(_MODE,_SPEED)    ((((_MODE) & 0x3) << 2) | ((_SPEED) & 0x03))

#define OS_GPIO_PIN_CONFIG(_PORT_CONFIG, _BIT, _VALUE) ((_PORT_CONFIG) = ((_PORT_CONFIG) & ~(OS_SVB(_BIT, 0x1))) | (OS_SVB(_BIT, _VALUE)))
#define OS_GPIO_PIN_CONFIG_OUTPUT(_PORT_CONFIG, _BIT) ((_PORT_CONFIG) = ((_PORT_CONFIG) & ~(OS_SVB(_BIT, 0x1))) | (OS_SVB(_BIT, 0x1)))
#define OS_GPIO_PIN_CONFIG_INPUT(_PORT_CONFIG, _BIT) ((_PORT_CONFIG) = ((_PORT_CONFIG) & ~(OS_SVB(_BIT, 0x1))) | (OS_SVB(_BIT, 0x0)))
#define OS_GPIO_PIN_HIGH(_PORT, _BIT) ((_PORT) |= OS_S1B(_BIT))
#define OS_GPIO_PIN_LOW(_PORT, _BIT) ((_PORT) &= ~OS_S1B(_BIT))
#define OS_GPIO_PIN_TOGGLE(_PORT, _BIT) ((_PORT) ^= OS_S1B(_BIT))

#define OS_INCLUDE_NAKED_INIT (1)

#define OS_INCLUDE_OSSCHEDULER_CONTEXTSAVE_RESTORE (1)
#define OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT (1)

#define OS_INCLUDE_SEPARATE_PROGMEM (1)

#endif /*HAL_OS_ARCH_DEFS_H_*/
