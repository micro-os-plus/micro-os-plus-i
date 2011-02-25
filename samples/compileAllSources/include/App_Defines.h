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

#define APP_CFGSTR_APPLICATION_NAME                     "compileAllSources"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        1
#define APP_CFGINT_VERSION_MINOR                        1
#define APP_CFGINT_VERSION_REVISION                     1393

// ----------------------------------------------------------------------------

#if defined(OS_CONFIG_BOARD_ATMEL_STK525)

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#elif defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)

#else
#error "Missing OS_CONFIG_BOARD_* board definition"
#endif

// ----------------------------------------------------------------------------

#endif /* APP_DEFINES_H_ */
