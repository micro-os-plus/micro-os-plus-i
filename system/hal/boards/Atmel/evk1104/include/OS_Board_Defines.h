/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OS_BOARD_DEFINES_H_
#define HAL_OS_BOARD_DEFINES_H_

#define OS_CONFIG_ARCH_AVR32					1
#define OS_CONFIG_FAMILY_AVR32UC3				1
#define OS_CONFIG_VARIANT_AVR32UC3A3				1
#if !defined(OS_CONFIG_BOARD_ATMEL_EVK1104)
#define OS_CONFIG_BOARD_ATMEL_EVK1104				1
#endif

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ				(16000000UL)
#endif

#if !defined(OS_CFGINT_TIMER_PRESCALLER)
#define OS_CFGINT_TIMER_PRESCALLER				(4)
#endif

// TODO: Stringify definitions
#if !defined(OS_CFGINT_TIMER_ID)
#define OS_CFGINT_TIMER_ID                                      (0)
#endif
#if !defined(OS_CFGVAR_TIMER)
#define OS_CFGVAR_TIMER                                         (AVR32_TC0)
#endif
#if !defined(OS_CFGINT_TIMER_CHANNEL)
#define OS_CFGINT_TIMER_CHANNEL                                 (0)
#endif
#if !defined(OS_CFGINT_TIMER_IRQ_ID)
#define OS_CFGINT_TIMER_IRQ_ID                                  (AVR32_TC0_IRQ0)
#endif
#if !defined(OS_CFGINT_TIMER_IRQ_LEVEL)
#define OS_CFGINT_TIMER_IRQ_LEVEL                               (0)
#endif

#if !defined(OS_CFGINT_TIMER_CLOCK_SELECT)
#define OS_CFGINT_TIMER_CLOCK_SELECT                            (3)
#endif



#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ             		        (1000)
#endif

#if !defined(OS_CONFIG_ACTIVE_LED_PORT)
#define OS_CONFIG_ACTIVE_LED_PORT			        GPIOX
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG		        OS_CONFIG_ACTIVE_LED_PORT
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			        AVR32_PIN_PX16
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
#define OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW                       (1)
#endif

#if false
#if !defined(OS_CONFIG_USB_LED_PORT_INIT)
#define OS_CONFIG_USB_LED_PORT_INIT			        DDRD
#endif
#if !defined(OS_CONFIG_USB_LED_PORT)
#define OS_CONFIG_USB_LED_PORT					PORTD
#endif
#if !defined(OS_CONFIG_USB_LED_BIT)
#define OS_CONFIG_USB_LED_BIT					PORTD7
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_INIT)
#define OS_CONFIG_BLINK_LED_PORT_INIT				DDRD
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT)
#define OS_CONFIG_BLINK_LED_PORT				PORTD
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT					5
#endif
#endif

#if true
#if !defined(OS_CONFIG_DEBUG_LED1_PORT)
#define OS_CONFIG_DEBUG_LED1_PORT                               GPIOX
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED1_PORT_CONFIG                        OS_CONFIG_DEBUG_LED1_PORT
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT                                AVR32_PIN_PX50
#endif

#if !defined(OS_CONFIG_DEBUG_LED2_PORT)
#define OS_CONFIG_DEBUG_LED2_PORT                               GPIOX
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED2_PORT_CONFIG                        OS_CONFIG_DEBUG_LED2_PORT
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT                                AVR32_PIN_PX54
#endif
#endif

#endif /* HAL_OS_BOARD_DEFINES_H_ */
