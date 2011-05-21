/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

// you may override the board definitions, if needed

#define OS_CFGINT_THREADS_TABLE_SIZE                      (4)

#define OS_INCLUDE_TIMER                                1
#define OS_INCLUDE_OSTIMERSECONDS                       1

//#define OS_INCLUDE_CIRCULARBYTEBUFFER                   1
//#define OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP              1

#define OS_INCLUDE_OSDEVICECHARACTER                    1
//#define OS_INCLUDE_OSDEVICECHARACTERBUFFERED            1

#define OS_INCLUDE_DEVICECHARACTERUSB                   (1)

#define OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0        (1)

#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                  1
//#define OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER             1

#if true
#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF          1
#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1
#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_ISTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTHREAD                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1
#endif

#define OS_INCLUDE_COMMANDLINEINTERFACE                 (1)
#define OS_INCLUDE_PARSER                               (1)

#if false
#define OS_INCLUDE_SIMPLECLI                            (1)
#define OS_INCLUDE_SIMPLECLI_PARSER                     (1)
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             (1)
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           (1)
#endif

#define OS_INCLUDE_OSEVENTFLAGS                         (1)

#define OS_INCLUDE_TASKBLINK                            (1)

#define OS_INCLUDE_GPSPOSITION                          (1)
#define OS_INCLUDE_OSTREAM_GPSPOSITION                  (1)

//#define OS_INCLUDE_USB_CDC_DUAL_INTERFACE               1
//#define OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION           1


//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE (9600)
//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED (true)

//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_TXBUF_SIZE (32)
//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_RXBUF_SIZE (32)

#define OS_CFGINT_OSUSBDEVICE_IRQ_PRIORITY              (0)


// ----------------------------------------------------------------------------

#if defined(DEBUG)

// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_INITS                                  (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)

//#define OS_DEBUG_OSDEVICECHARACTER_OPEN                 1
//#define OS_DEBUG_OSDEVICECHARACTER_READBYTE             1
//#define OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE            1

#define OS_DEBUG_PARSER_PARSETOKEN                      (1)

//#define OS_DEBUG_OSUSBDEVICE_REQUEST                    1
//#define OS_DEBUG_DEVICECHARACTERUSB_WRITE               1
//#define OS_DEBUG_DEVICECHARACTERUSB_READ                1

#endif /* defined(DEBUG) */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
