/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

// you may override the board definitions, if needed

#define OS_CFGINT_THREADS_ARRAY_SIZE                      (4)

#define OS_INCLUDE_TIMER                                1
#define OS_INCLUDE_OSTIMERSECONDS                       1

//#define OS_INCLUDE_CIRCULARBYTEBUFFER                   1
//#define OS_INCLUDE_CIRCULARBYTEBUFFER_DUMP              1

#define OS_INCLUDE_OSDEVICECHARACTER                    1
//#define OS_INCLUDE_OSDEVICECHARACTERBUFFERED            1

#define OS_INCLUDE_DEVICECHARACTERUSB                   1

#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                  1
//#define OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER             1

//#define OS_INCLUDE_SIMPLECLI                            1

#define OS_INCLUDE_SIMPLECLI_PARSER                     1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             1
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           1

#if true
#define OS_INCLUDE_CLOG                                 1
#define OS_INCLUDE_OSTREAM_OSTHREAD                       1
#define OS_INCLUDE_OSTREAM_PROGRAMPTR                   1
#endif


#define OS_INCLUDE_TASKBLINK                            1

//#define OS_INCLUDE_USB_CDC_DUAL_INTERFACE               1
//#define OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION           1


//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE (9600)
//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED (true)

//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_TXBUF_SIZE (32)
//#define OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_RXBUF_SIZE (32)

// ----- Board Definitions ----------------------------------------------------

#if defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

//#define OS_EXCLUDE_PREEMPTION                           (1)
//#define OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK          (1)
//#define OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS    (1)
//#define OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS (1)
//#define OS_INCLUDE_OSSCHEDULER_CRITICALENTER_WITH_MASK  (1)
//#define OS_CFGINT_OSCRITICALSECTION_MASK                (0xF << (16+1))
//#define OS_CFGINT_CPU_STATUS_REGISTER_MODE              ((OSCPUImpl::getInterruptsMask() >> 22) & 0x7)
//#define OS_CFGINT_CPU_STATUS_REGISTER_INTERRUPT_MASK    (1 << ((OS_CFGINT_CPU_STATUS_REGISTER_MODE - 2) & 3))
//#define OS_CFGINT_OSCRITICALSECTION_MASK                ((0xF & ~OS_CFGINT_CPU_STATUS_REGISTER_INTERRUPT_MASK) << (16+1))


#elif defined(OS_CONFIG_BOARD_ATMEL_STK525)

#endif /* OS_CONFIG_BOARD_* */

// ----- Debug Definitions ----------------------------------------------------

#if defined(DEBUG)

// One of the following must be defined in Makefile or project
//#define OS_CONFIG_DEBUG_DEVICE_I2C                    1
//#define OS_CONFIG_DEBUG_DEVICE_USART			1

#define OS_DEBUG_CONSTRUCTORS                           1
#define OS_DEBUG_INITS                                  1

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS       (1)

//#define OS_DEBUG_OSDEVICECHARACTER_OPEN                 1
//#define OS_DEBUG_OSDEVICECHARACTER_READBYTE             1
//#define OS_DEBUG_OSDEVICECHARACTER_WRITEBYTE            1

#define OS_DEBUG_OSUSBDEVICE_REQUEST                    1
#define OS_DEBUG_DEVICECHARACTERUSB_WRITE               1
#define OS_DEBUG_DEVICECHARACTERUSB_READ                1
#define OS_CFGINT_OSUSBDEVICE_IRQ_PRIORITY    (0)

#endif /* defined(DEBUG) */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
