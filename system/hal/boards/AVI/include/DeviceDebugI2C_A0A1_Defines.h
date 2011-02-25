/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_DEVICEDEBUGI2C_A0A1_DEFINES_H_
#define HAL_BOARD_DEVICEDEBUGI2C_A0A1_DEFINES_H_

#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT             (0)
#define OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT	(8)

#define OS_CONFIG_DEBUG_SCL_PORT_CONFIG                 DDRA
#define OS_CONFIG_DEBUG_SCL_PORT_WRITE                  PORTA
#define OS_CONFIG_DEBUG_SCL_PORT_READ                   PINA
#define OS_CONFIG_DEBUG_SCL_BIT                         0

#define OS_CONFIG_DEBUG_SDA_PORT_CONFIG                 DDRA
#define OS_CONFIG_DEBUG_SDA_PORT_WRITE                  PORTA
#define OS_CONFIG_DEBUG_SDA_PORT_READ                   PINA
#define OS_CONFIG_DEBUG_SDA_BIT                         1

#endif /* HAL_BOARD_DEVICEDEBUGI2C_A0A1_DEFINES_H_ */
