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
#define OS_CFGLONG_OSCILLATOR_HZ				(12000000UL)
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

// OS_CFGLONG_CPU_FREQUENCY_HZ, HSB, PBA & PBB computed in OS_Family_Defines.h

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

#define OS_CFG_RTC_TOP                                          (32768)

#if !defined(OS_CFGINT_TICK_RATE_HZ)
#define OS_CFGINT_TICK_RATE_HZ             		        (1000)
#endif

// Led definitions
#define OS_CFGREG_BOARD_LED0_PORT_CONFIG                        GPIOX
#define OS_CFGREG_BOARD_LED0_PORT_WRITE                         GPIOX
#define OS_CFGREG_BOARD_LED0_PORT_READ                          GPIOX
#define OS_CFGPIN_BOARD_LED0_PIN                                AVR32_PIN_PX16
#define OS_CFGBOOL_BOARD_LED0_ISACTIVE_LOW                      (true)

#define OS_CFGREG_BOARD_LED1_PORT_CONFIG                        GPIOX
#define OS_CFGREG_BOARD_LED1_PORT_WRITE                         GPIOX
#define OS_CFGREG_BOARD_LED1_PORT_READ                          GPIOX
#define OS_CFGPIN_BOARD_LED1_PIN                                AVR32_PIN_PX50
#define OS_CFGBOOL_BOARD_LED1_ISACTIVE_LOW                      (true)

#define OS_CFGREG_BOARD_LED2_PORT_CONFIG                        GPIOX
#define OS_CFGREG_BOARD_LED2_PORT_WRITE                         GPIOX
#define OS_CFGREG_BOARD_LED2_PORT_READ                          GPIOX
#define OS_CFGPIN_BOARD_LED2_PIN                                AVR32_PIN_PX54
#define OS_CFGBOOL_BOARD_LED2_ISACTIVE_LOW                      (true)

#define OS_CFGREG_BOARD_LED3_PORT_CONFIG                        GPIOX
#define OS_CFGREG_BOARD_LED3_PORT_WRITE                         GPIOX
#define OS_CFGREG_BOARD_LED3_PORT_READ                          GPIOX
#define OS_CFGPIN_BOARD_LED3_PIN                                AVR32_PIN_PX51
#define OS_CFGBOOL_BOARD_LED3_ISACTIVE_LOW                      (true)

// Default processor activity led is on Led0
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
#define OS_CONFIG_ACTIVE_LED_PORT_CONFIG		        OS_CFGREG_BOARD_LED0_PORT_CONFIG
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_PORT_WRITE)
#define OS_CONFIG_ACTIVE_LED_PORT_WRITE                         OS_CFGREG_BOARD_LED0_PORT_WRITE
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_BIT)
#define OS_CONFIG_ACTIVE_LED_BIT			        OS_CFGPIN_BOARD_LED0_PIN
#endif
#if !defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
#define OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW                       OS_CFGBOOL_BOARD_LED0_ISACTIVE_LOW
#endif

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
#if !defined(OS_CONFIG_OSTIMERTICKS_LED_PORT_WRITE)
#define OS_CONFIG_OSTIMERTICKS_LED_PORT                         GPIOX
#endif
#if !defined(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG)
#define OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG                  OS_CONFIG_ACTIVE_LED_PORT
#endif
#if !defined(OS_CONFIG_OSTIMERTICKS_LED_BIT)
#define OS_CONFIG_OSTIMERTICKS_LED_BIT                          AVR32_PIN_PX50
#endif
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

// Default USB activity led on LED3
#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_CONFIG)
#define OS_CFGREG_OSUSBLED_LED_PORT_CONFIG			        OS_CFGREG_BOARD_LED3_PORT_CONFIG
#endif
#if !defined(OS_CFGREG_OSUSBLED_LED_PORT_WRITE)
#define OS_CFGREG_OSUSBLED_LED_PORT_WRITE				OS_CFGREG_BOARD_LED3_PORT_WRITE
#endif
#if !defined(OS_CFGPIN_OSUSBLED_LED_PIN)
#define OS_CFGPIN_OSUSBLED_LED_PIN			        OS_CFGPIN_BOARD_LED3_PIN
#endif
#if !defined(OS_CFGBOOL_OSUSBLED_LED_ISACTIVE_LOW)
#define OS_CFGBOOL_OSUSBLED_LED_ISACTIVE_LOW                    OS_CFGBOOL_BOARD_LED3_ISACTIVE_LOW
#endif

// Default blink led on LED1
#if !defined(OS_CONFIG_BLINK_LED_PORT_CONFIG)
#define OS_CONFIG_BLINK_LED_PORT_CONFIG				OS_CFGREG_BOARD_LED1_PORT_CONFIG
#endif
#if !defined(OS_CONFIG_BLINK_LED_PORT_WRITE)
#define OS_CONFIG_BLINK_LED_PORT_WRITE				OS_CFGREG_BOARD_LED1_PORT_WRITE
#endif
#if !defined(OS_CONFIG_BLINK_LED_BIT)
#define OS_CONFIG_BLINK_LED_BIT					OS_CFGPIN_BOARD_LED1_PIN
#endif

// Default debug led 1 on LED1
#if !defined(OS_CONFIG_DEBUG_LED1_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED1_PORT_CONFIG                        OS_CFGREG_BOARD_LED1_PORT_CONFIG
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT_WRITE)
#define OS_CONFIG_DEBUG_LED1_PORT_WRITE                         OS_CFGREG_BOARD_LED1_PORT_WRITE
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT                                OS_CFGPIN_BOARD_LED1_PIN
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_ISACTIVE_LOW)
#define OS_CONFIG_DEBUG_LED1_ISACTIVE_LOW                      OS_CFGBOOL_BOARD_LED1_ISACTIVE_LOW
#endif

// Default debug led 2 on LED1
#if !defined(OS_CONFIG_DEBUG_LED2_PORT_CONFIG)
#define OS_CONFIG_DEBUG_LED2_PORT_CONFIG                        OS_CFGREG_BOARD_LED2_PORT_CONFIG
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT_WRITE)
#define OS_CONFIG_DEBUG_LED2_PORT_WRITE                         OS_CFGREG_BOARD_LED2_PORT_WRITE
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT                                OS_CFGPIN_BOARD_LED2_PIN
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_ISACTIVE_LOW)
#define OS_CONFIG_DEBUG_LED2_ISACTIVE_LOW                      OS_CFGBOOL_BOARD_LED2_ISACTIVE_LOW
#endif

#if defined(DEBUG) && !defined(OS_CONFIG_DEBUG_DEVICE_I2C)
#define OS_CONFIG_DEBUG_DEVICE_I2C                      (1)
#endif

#endif /* HAL_OS_BOARD_DEFINES_H_ */
