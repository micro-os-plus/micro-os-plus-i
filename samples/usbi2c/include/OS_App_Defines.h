/*
 *	Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_CONFIG_H_
#define OS_APP_CONFIG_H_

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (3)

#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS             1

#define OS_INCLUDE_CIRCULARBYTEBUFFER                   1

//#define OS_CFGINT_DEVICECHARACTERMULTIUSART1_BAUD_RATE  (19200)

#define OS_INCLUDE_OSDEVICECHARACTER                    1
//#define OS_INCLUDE_DEVICECHARACTERMULTIUSART1         1
#define OS_INCLUDE_DEVICECHARACTERUSB                 1
#define OS_INCLUDE_DEVICECHARACTERUSB_MULTIBYTE         1
#define OS_EXCLUDE_USBSERIALNUMBER                      (1)

#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              1

#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF          1

#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                  1
#define OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER             1

#define OS_INCLUDE_SIMPLECLI_PARSER                     1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           1

#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_SIMPLECLI                            1

#define OS_INCLUDE_OSTREAM                              1
#define OS_INCLUDE_ISTREAM                              1
#define OS_INCLUDE_OSTREAM_OSTHREAD                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1

#define OS_INCLUDE_TASKPITPALAC                         1

#define OS_CFGINT_DEVICECHARACTERI2C_TXBUF_SIZE         32
#define OS_CFGINT_DEVICECHARACTERMI2C_RXBUF_SIZE        1024

#define OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK     1

#if defined(DEBUG)

#define OS_DEBUG_CONSTRUCTORS                           1
//#define OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE           1

#define OS_DEBUG_DEVICECHARACTERUSB_WRITE               1
#define OS_DEBUG_DEVICECHARACTERUSB_READ                1

#endif /* DEBUG */

#endif /* OS_APP_CONFIG_H_ */
