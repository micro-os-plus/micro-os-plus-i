/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_
#define HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_

#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT             (0)
#define OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT	(8)

#define OS_CONFIG_DEBUG_SCL_PORT_CONFIG                 DDRC
#define OS_CONFIG_DEBUG_SCL_PORT_WRITE                  PORTC
#define OS_CONFIG_DEBUG_SCL_PORT_READ                   PINC
#define OS_CONFIG_DEBUG_SCL_BIT                         PORTC0

#define OS_CONFIG_DEBUG_SDA_PORT_CONFIG                 DDRC
#define OS_CONFIG_DEBUG_SDA_PORT_WRITE                  PORTC
#define OS_CONFIG_DEBUG_SDA_PORT_READ                   PINC
#define OS_CONFIG_DEBUG_SDA_BIT                         PORTC1

// Include the AVI file, for the USB-I2C address
#include "hal/boards/AVI/include/DeviceDebugI2CAddr_Defines.h"

#endif /* HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_ */
