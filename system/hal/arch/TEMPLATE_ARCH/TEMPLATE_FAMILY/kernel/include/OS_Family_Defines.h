/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

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

#if !defined(OS_CFGINT_BUSYWAIT_CALIBRATION)
#if defined(DEBUG)
#define OS_CFGINT_BUSYWAIT_CALIBRATION                  (7950)
#else
#define OS_CFGINT_BUSYWAIT_CALIBRATION                  (7074)
#endif
#endif

#if !defined(OS_CFGINT_OSCRITICALSECTION_MASK)
#define OS_CFGINT_OSCRITICALSECTION_MASK                (0xF << (16+1))
#endif /* !defined(OS_CFGINT_OSCRITICALSECTION_MASK) */

/*
 * Usage:
 *      _PORT =
 *      _BIT =
 */
#define OS_GPIO_PIN_CONFIG_ENABLE(_PORT, _BIT)
#define OS_GPIO_PIN_CONFIG_OUTPUT(_PORT, _BIT)
#define OS_GPIO_PIN_CONFIG_INPUT(_PORT, _BIT)

#define OS_GPIO_PIN_HIGH(_PORT, _BIT)
#define OS_GPIO_PIN_LOW(_PORT, _BIT)
#define OS_GPIO_PIN_TOGGLE(_PORT, _BIT)

#define OS_GPIO_PIN_ISHIGH(_PORT, _BIT) ((1) ? true : false)
#define OS_GPIO_PIN_ISLOW(_PORT, _BIT) ((0) ? true : false)

#endif /* HAL_FAMILY_OS_DEFS_H_ */
