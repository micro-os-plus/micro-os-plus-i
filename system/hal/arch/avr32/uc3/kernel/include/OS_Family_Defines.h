/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

#include <avr32/io.h>

// UC3 family clock default definitions

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ                                (16000000UL)
#endif

#if !defined(OS_CFGINT_HSB_PRESCALLER)
#define OS_CFGINT_HSB_PRESCALLER_SEL                                (0)
#endif

#if !defined(OS_CFGINT_PBA_PRESCALLER)
#define OS_CFGINT_PBA_PRESCALLER_SEL                                (0)
#endif

#if !defined(OS_CFGINT_PBB_PRESCALLER)
#define OS_CFGINT_PBB_PRESCALLER_SEL                                (0)
#endif

#define OS_CFGLONG_CPU_FREQUENCY_HZ    (OS_CFGLONG_OSCILLATOR_HZ/(1 << OS_CFGINT_HSB_PRESCALLER_SEL))
#define OS_CFGLONG_HSB_FREQUENCY_HZ    (OS_CFGLONG_OSCILLATOR_HZ/(1 << OS_CFGINT_HSB_PRESCALLER_SEL))
#define OS_CFGLONG_PBA_FREQUENCY_HZ    (OS_CFGLONG_OSCILLATOR_HZ/(1 << OS_CFGINT_PBA_PRESCALLER_SEL))
#define OS_CFGLONG_PBB_FREQUENCY_HZ    (OS_CFGLONG_OSCILLATOR_HZ/(1 << OS_CFGINT_PBB_PRESCALLER_SEL))


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
