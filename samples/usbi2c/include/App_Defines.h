/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

// do not use parenthesis, will also be stringified
#define APP_CFGINT_MAJOR                                2
#define APP_CFGINT_MINOR                                2
#define APP_CFGINT_REVISION                             541

// Strings
#define APP_CONFIG_MAJOR                                xstr(APP_CFGINT_MAJOR)
#define APP_CONFIG_MINOR                                xstr(APP_CFGINT_MINOR)
#define APP_CONFIG_REVISION                             xstr(APP_CFGINT_REVISION)

#define APP_CONFIG_IMUXEN_PORT_INIT     DDRC
#define APP_CONFIG_IMUXEN_PORT          PORTC
#define APP_CONFIG_IMUXEN_BIT           PORTC5

#define APP_CONFIG_IMUX_PORT_INIT       DDRC
#define APP_CONFIG_IMUX_PORT            PORTC
#define APP_CONFIG_IMUX_BIT             PORTC4

#endif /*APP_DEFINES_H_ */
