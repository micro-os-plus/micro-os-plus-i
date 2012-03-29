/*
 * OS_Board_Defines.h
 *
 *  Created on: 18 Jan 2011
 *      Author: Liviu Ionescu
 */

#ifndef HAL_OS_BOARD_DEFINES_H_
#define HAL_OS_BOARD_DEFINES_H_

#define OS_CONFIG_ARCH_TEMPLATE					1
#define OS_CONFIG_FAMILY_TEMPLATE				1
#define OS_CONFIG_VARIANT_TEMPLATE				1
#if !defined(OS_CONFIG_BOARD_TEMPLATE)
#define OS_CONFIG_BOARD_TEMPLATE				1
#endif

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ				(16000000UL)
#endif
#if !defined(OS_CFGINT_CLOCK_PRESCALER)
#define OS_CFGINT_CLOCK_PRESCALER				(1)
#endif

#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ             		        (1000)
#endif

#if !defined(OS_CONFIG_ACTIVE_LED_PORT)
#define OS_CONFIG_ACTIVE_LED_PORT						GPIOC
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG				OS_CONFIG_ACTIVE_LED_PORT->CRH
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT						12
#endif

#if false
#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG)
#define OS_CFGREG_OSUSBLED_LED_PORT_CONFIG						DDRD
#endif
#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE)
#define OS_CFGREG_OSUSBLED_LED_PORT_WRITE							PORTD
#endif
#if !defined(OS_CFGPIN_OSUSBLED_LED_PIN)
#define OS_CFGPIN_OSUSBLED_LED_PIN							PORTD7
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_INIT)
#define OS_CONFIG_BLINK_LED_PORT_INIT					DDRD
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT)
#define OS_CONFIG_BLINK_LED_PORT						PORTD
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT							5
#endif
#endif

#if false
#if !defined(OS_CONFIG_DEBUG_LED1_PORT)
#define OS_CONFIG_DEBUG_LED1_PORT                       GPIOC
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED1_PORT_CONFIG                OS_CONFIG_DEBUG_LED1_PORT->CRH
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT                        10
#endif

#if !defined(OS_CONFIG_DEBUG_LED2_PORT)
#define OS_CONFIG_DEBUG_LED2_PORT                       GPIOC
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED2_PORT_CONFIG                OS_CONFIG_DEBUG_LED2_PORT->CRH
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT                        9
#endif
#endif

#endif /* HAL_OS_BOARD_DEFINES_H_ */
