/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_CONFIG_H_
#define HAL_BOARD_CONFIG_H_

#define APP_CONFIG_PWR_ENABLE_PORT_INIT         DDRD
#define APP_CONFIG_PWR_ENABLE_PORT              PORTD
#define APP_CONFIG_PWR_ENABLE_BIT               PD7

#define APP_CONFIG_DIGIPOT_PORT_INIT            DDRB
#define APP_CONFIG_DIGIPOT_PORT                 PORTB
#define APP_CONFIG_DIGIPOT_BIT                  PB2
#define APP_CONFIG_DIGIPOT_MASK                 0x3

#endif /* HAL_BOARD_CONFIG_H_ */
