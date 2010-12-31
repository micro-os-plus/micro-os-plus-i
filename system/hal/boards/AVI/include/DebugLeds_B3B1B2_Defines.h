/*
 *	Copyright (C) 2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_BOARD_DEBUGLEDS_B3B1B2_DEFINES_H_
#define HAL_BOARD_DEBUGLEDS_B3B1B2_DEFINES_H_

/*
 * Debug leds are intended for temporary visual clues while debugging.
 * To avoid interference with other signal, they are assigned to the 
 * programming pins and in my setup are physically installed on the 
 * programming adapter from the standard large 6 pin programming 
 * connector to the custom small 8 pin header Lix designed for the
 * new Metrilog SDI12 adapters.
 * 
 * In case you are curious about the order of signals, it was assigned
 * by Lix when he designed the adapter, based on PCB routing convenience
 * and here it is even today.
 * 
 * The colours of the debug leds are:
 * 
 * - LED1 = Red
 * - LED2 = Yellow
 * - LED3 = Green
 * 
 * Usage of these leds is entirely at developper discretion. For embedded boards 
 * that do not provide an activity led, the Green led is probably a good choice.
 * 
 */

#if !defined(OS_CONFIG_DEBUG_LED1_PORT_INIT)
#define OS_CONFIG_DEBUG_LED1_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_PORT)
#define OS_CONFIG_DEBUG_LED1_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED1_BIT)
#define OS_CONFIG_DEBUG_LED1_BIT                        3
#endif

#if !defined(OS_CONFIG_DEBUG_LED2_PORT_INIT)
#define OS_CONFIG_DEBUG_LED2_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_PORT)
#define OS_CONFIG_DEBUG_LED2_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED2_BIT)
#define OS_CONFIG_DEBUG_LED2_BIT                        1
#endif

#if !defined(OS_CONFIG_DEBUG_LED3_PORT_INIT)
#define OS_CONFIG_DEBUG_LED3_PORT_INIT                  DDRB
#endif
#if !defined(OS_CONFIG_DEBUG_LED3_PORT)
#define OS_CONFIG_DEBUG_LED3_PORT                       PORTB
#endif
#if !defined(OS_CONFIG_DEBUG_LED3_BIT)
#define OS_CONFIG_DEBUG_LED3_BIT                        2
#endif

#endif /* HAL_BOARD_DEBUGLEDS_B3B1B2_DEFINES_H_ */
