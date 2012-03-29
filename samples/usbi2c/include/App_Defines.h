/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

// ----------------------------------------------------------------------------

// Application versioning and greeting definitions
// More definitions will be computed in uOS.h

#define APP_CFGSTR_APPLICATION_NAME                     "USB-I2C"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        2
#define APP_CFGINT_VERSION_MINOR                        3
#define APP_CFGINT_VERSION_REVISION                     1741

// ----------------------------------------------------------------------------

#if defined(OS_CONFIG_BOARD_AVI_A07523)

#elif defined(OS_CONFIG_BOARD_AVI_A0739)

#define APP_CONFIG_IMUXEN_PORT_INIT     DDRC
#define APP_CONFIG_IMUXEN_PORT          PORTC
#define APP_CONFIG_IMUXEN_BIT           PORTC5

#define APP_CONFIG_IMUX_PORT_INIT       DDRC
#define APP_CONFIG_IMUX_PORT            PORTC
#define APP_CONFIG_IMUX_BIT             PORTC4

#else
#error "Missing OS_CONFIG_BOARD_* board definition"
#endif

// ----------------------------------------------------------------------------

#endif /* APP_DEFINES_H_ */
