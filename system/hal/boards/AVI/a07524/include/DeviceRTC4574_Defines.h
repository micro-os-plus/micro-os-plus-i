/*
 *      Copyright (C) 2008-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICERTC4574_DEFINES_H_
#define DEVICERTC4574_DEFINES_H_

#define OS_CONFIG_RTC4574_CLK_PORT_CONFIG       DDRC
#define OS_CONFIG_RTC4574_CLK_PORT_WRITE        PORTC
#define OS_CONFIG_RTC4574_CLK_BIT               6

#define OS_CONFIG_RTC4574_DATA_PORT_CONFIG      DDRC
#define OS_CONFIG_RTC4574_DATA_PORT_WRITE       PORTC
#define OS_CONFIG_RTC4574_DATA_PORT_READ        PINC
#define OS_CONFIG_RTC4574_DATA_BIT              5

#define OS_CONFIG_RTC4574_FCON_PORT_CONFIG      DDRC
#define OS_CONFIG_RTC4574_FCON_PORT_WRITE       PORTC
#define OS_CONFIG_RTC4574_FCON_BIT              4

#endif /* DEVICERTC4574_DEFINES_H_ */
