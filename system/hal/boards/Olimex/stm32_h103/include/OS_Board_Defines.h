/*
 *	Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_OS_DEFINES_H_
#define HAL_BOARD_OS_DEFINES_H_

#define OS_CONFIG_ARCH_ARM_CORTEX_M3                    1
#define OS_CONFIG_FAMILY_STM32F10X                      1
#define OS_CONFIG_VARIANT_STM32F103RBT6                 1
#if !defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)
#define OS_CONFIG_BOARD_OLIMEX_STM32_H103               1
#endif

#if !defined(OS_CFGLONG_OSCILLATOR_HZ)
#define OS_CFGLONG_OSCILLATOR_HZ			(8000000UL)
#endif

//#if !defined(OS_CFGINT_CLOCK_PRESCALER)
//#define OS_CFGINT_CLOCK_PRESCALER			(1)
//#endif


#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ                          (1000)
#endif

// The density definitions are needed in startup code
// #define STM32F10X_LD   /*!< STM32 Low density devices */
#define STM32F10X_MD   /*!< STM32 Medium density devices */
//#define STM32F10X_HD   /*!< STM32 High density devices */

// The SYSCLK definitions are used in lib/src/system_stm32f10x.cpp
//#define SYSCLK_FREQ_HSE    HSE_Value
//#define SYSCLK_FREQ_20MHz  20000000
//#define SYSCLK_FREQ_36MHz  36000000
//#define SYSCLK_FREQ_48MHz  48000000
//#define SYSCLK_FREQ_56MHz  56000000
#define SYSCLK_FREQ_72MHz  72000000

// keep this in sync with above
#if !defined(OS_CFGINT_CLOCK_MULTIPLIER)
#define OS_CFGINT_CLOCK_MULTIPLIER                      (9)
#endif

#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191 /* equivalent to 0xa0, or priority 5. */

#if !defined(OS_CONFIG_ACTIVE_LED_PORT)
#define OS_CONFIG_ACTIVE_LED_PORT			GPIOC
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG                OS_CONFIG_ACTIVE_LED_PORT->CRH
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			12
#endif

#if false
#if !defined(OS_CONFIG_USB_LED_PORT_INIT)
#define OS_CONFIG_USB_LED_PORT_INIT                     DDRD
#endif
#if !defined(OS_CONFIG_USB_LED_PORT)
#define OS_CONFIG_USB_LED_PORT                          PORTD
#endif
#if !defined(OS_CONFIG_USB_LED_BIT)
#define OS_CONFIG_USB_LED_BIT                           PORTD7
#endif

#if !defined(OS_CONFIG_BLINK_LED_PORT_INIT)
#define OS_CONFIG_BLINK_LED_PORT_INIT                   DDRD
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT)
#define OS_CONFIG_BLINK_LED_PORT                        PORTD
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT                         5
#endif
#endif

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

#endif /*HAL_BOARD_OS_DEFINES_H_*/
