/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

#include <avr32/io.h>

// _VALUE == 1 output
// _VALUE == 0 input
#define OS_GPIO_PORT_CONFIG(_PORT_CONFIG, _BIT, _VALUE) \
	{ \
	(AVR32_GPIO.port[(_BIT)>>5].gpers = (1<< ((_BIT) & 0x1F))); \
	if( _VALUE > 0) \
	(AVR32_GPIO.port[(_BIT)>>5].oders = (1<< ((_BIT) & 0x1F))); \
        else \
	(AVR32_GPIO.port[(_BIT)>>5].oderc = (1<< ((_BIT) & 0x1F))); \
	}

#define OS_GPIO_PORT_HIGH(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrs = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PORT_LOW(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrc = (1<< ((_BIT) & 0x1F)))
#define OS_GPIO_PORT_TOGGLE(_PORT, _BIT) (AVR32_GPIO.port[(_BIT)>>5].ovrt = (1<< ((_BIT) & 0x1F)))

#define OS_GPIO_PORT_ISHIGH(_PORT, _BIT)	((AVR32_GPIO.port[(_BIT)>>5].pvr & (1<< ((_BIT) & 0x1F)))!=0 ? true : false)
#define OS_GPIO_PORT_ISLOW(_PORT, _BIT)		((AVR32_GPIO.port[(_BIT)>>5].pvr & (1<< ((_BIT) & 0x1F)))==0 ? true : false)

#endif /* HAL_FAMILY_OS_DEFS_H_ */
