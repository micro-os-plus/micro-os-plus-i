/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

#define OS_CONFIG_APP_LED_PORT_INIT     DDRD
#define OS_CONFIG_APP_LED_PORT          PORTD
#define OS_CONFIG_APP_LED_BIT           PORTD5

#define MMC_BUFSIZE                     (96+4+4)
#define MMC_CID_OFFSET                  20

#endif /*APP_DEFINES_H_ */
