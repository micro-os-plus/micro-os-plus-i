/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_
#define HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_

#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT             (1)
#define OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT        (8)

#define OS_CONFIG_DEBUG_SCL_PORT_CONFIG                 DDRD
#define OS_CONFIG_DEBUG_SCL_PORT_WRITE                  PORTD
#define OS_CONFIG_DEBUG_SCL_PORT_READ                   PIND
#define OS_CONFIG_DEBUG_SCL_BIT                         PIND2

#define OS_CONFIG_DEBUG_SDA_PORT_CONFIG                 DDRD
#define OS_CONFIG_DEBUG_SDA_PORT_WRITE                  PORTD
#define OS_CONFIG_DEBUG_SDA_PORT_READ                   PIND
#define OS_CONFIG_DEBUG_SDA_BIT                         PIND3

#include "hal/boards/AVI/include/DeviceDebugI2CAddr_Defines.h"

#endif /* HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_ */
