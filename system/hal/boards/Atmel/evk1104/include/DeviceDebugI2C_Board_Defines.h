/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_
#define HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_

#include <avr32/io.h>

#define OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT        (100)
#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT             (2)

#define OS_CONFIG_DEBUG_SCL_PORT                        GPIO
#define OS_CONFIG_DEBUG_SCL_PORT_CONFIG                 OS_CONFIG_DEBUG_SCL_PORT
#define OS_CONFIG_DEBUG_SCL_BIT                         AVR32_PIN_PA03

#define OS_CONFIG_DEBUG_SDA_PORT                        GPIO
#define OS_CONFIG_DEBUG_SDA_PORT_CONFIG                 OS_CONFIG_DEBUG_SDA_PORT
#define OS_CONFIG_DEBUG_SDA_BIT                         AVR32_PIN_PA04


#include "hal/boards/AVI/include/DeviceDebugI2CAddr_Defines.h"

#endif /*HAL_BOARD_DEVICEDEBUGI2C_DEFINES_H_*/
