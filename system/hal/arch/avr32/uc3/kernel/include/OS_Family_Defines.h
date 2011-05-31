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

#if !defined(OS_CFGINT_BUSYWAIT_CALIBRATION)
#if defined(DEBUG)
#define OS_CFGINT_BUSYWAIT_CALIBRATION                  (7950)
#else
#define OS_CFGINT_BUSYWAIT_CALIBRATION                  (7074)
#endif
#endif

//#define OS_EXCLUDE_OSCRITICALSECTION_USE_STACK          (1)
//#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)
//#define OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS (1)
//#define OS_INCLUDE_OSSCHEDULER_CRITICALENTER_WITH_MASK  (1)

// Below are default masks used to disable/enable interrupts.
// The order of bits is: 3210

// Mask to disable ALL (0-3) interrupts, including real-time ones
#define OS_CFGINT_OSCRITICALSECTION_MASKALL             (0xF << (16+1))

// Mask to disable regular (0,1) and real-time (2) interrupts
#if !defined(OS_CFGINT_OSCRITICALSECTION_MASKRT)
#define OS_CFGINT_OSCRITICALSECTION_MASKRT              (0x7 << (16+1))
#endif /* !defined(OS_CFGINT_OSCRITICALSECTION_MASKRT) */

// Mask to disable regular (0, 1) interrupts, except real-time ones
#if !defined(OS_CFGINT_OSCRITICALSECTION_MASK)
#define OS_CFGINT_OSCRITICALSECTION_MASK                (0x3 << (16+1))
#endif /* !defined(OS_CFGINT_OSCRITICALSECTION_MASK) */

// Mask to disable regular (0,1) and real-time (2) interrupts
// Used in yield() context switch code
#if !defined(OS_CFGINT_OSCRITICALSECTION_MASKYIELD)
#define OS_CFGINT_OSCRITICALSECTION_MASKYIELD           (0x7 << (16+1))
#endif /* !defined(OS_CFGINT_OSCRITICALSECTION_MASKYIELD) */

// Hack to adjust the mask when executed in an interrupt context
#define OSCRITICALSECTION_FAMILY_MASK(_MASK_)           (_MASK_ & ~(1 << (((((OSCPUImpl::getInterruptsMask() >> 22) & 0x7) - 2) & 3)+(16+1))))

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
