/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

// you may override the board definitions, if needed

#define OS_CFGINT_TASKS_TABLE_SIZE                      (4)

#define OS_INCLUDE_OSTIMERSECONDS                       1

#define OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP              1

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#define OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0        (1)
#endif
#define OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1        (1)

#define OS_INCLUDE_OSTASK_GETSTACKUSED                  1
//#define OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER             1

#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           1

#if true
//#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1
#define OS_INCLUDE_CLOG                                 1
//#define OS_INCLUDE_OSTREAM                              1
//#define OS_INCLUDE_ISTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTASK                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1
//#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF          1
#endif

#define OS_INCLUDE_SIMPLECLI                            1


#define OS_INCLUDE_TASKBLINK                            1

//#define OS_INCLUDE_USB_CDC_DUAL_INTERFACE               1
//#define OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION           1


#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE (57600)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_RX_PIN  (5)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_TX_PIN  (6)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION (0)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_IRQ_PRIORITY    (1)

#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_TXBUF_SIZE (200)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_RXBUF_SIZE (200)


#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_RATE (19200)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART0_RX_PIN  (51)
#define OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART0_TX_PIN  (52)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_GPIO_FUNCTION (1)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_IRQ_PRIORITY    (0)

#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_TXBUF_SIZE (32)
#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_RXBUF_SIZE (32)
// ----------------------------------------------------------------------------

#if defined(DEBUG)

// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_DEBUG_CONSTRUCTORS                           1
#define OS_DEBUG_INITS                                  1

//#define OS_DEBUG_OSDEVICECHARACTER_OPEN                 1
//#define OS_DEBUG_OSDEVICECHARACTER_READBYTE             1
//#define OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE            1

#define OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG            1

//#define OS_DEBUG_OSDEVICECHARACTERBUFFERED_ISR         (1)// for test the tx/rx isr

#endif /* defined(DEBUG) */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
