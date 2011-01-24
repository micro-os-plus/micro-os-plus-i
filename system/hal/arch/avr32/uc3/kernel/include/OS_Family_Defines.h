/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

#include <avr32/io.h>

/*
 * Usage:
 *      _PORT = GPIOX (currently ignored)
 *      _BIT = AVR32_PIN_PXnn
 */
#define OS_GPIO_PIN_CONFIG_ENABLE(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].gpers = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PIN_CONFIG_OUTPUT(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].oders = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PIN_CONFIG_INPUT(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].oderc = (1<< ((_BIT) & 0x1F)))

#define OS_GPIO_PIN_HIGH(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrs = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PIN_LOW(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrc = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PIN_TOGGLE(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrt = (1<< ((_BIT) & 0x1F)))

#define OS_GPIO_PIN_ISHIGH(_PORT, _BIT)	((AVR32_GPIO.port[(_BIT)>>5].pvr & (1<< ((_BIT) & 0x1F)))!=0 ? true : false)
#define OS_GPIO_PIN_ISLOW(_PORT, _BIT)		((AVR32_GPIO.port[(_BIT)>>5].pvr & (1<< ((_BIT) & 0x1F)))==0 ? true : false)

#endif /* HAL_FAMILY_OS_DEFS_H_ */
