/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_APP_DEFINES_H_
#define OS_APP_DEFINES_H_

// ----------------------------------------------------------------------------

//#define OS_CFGINT_TICK_RATE_HZ                          (1000)
#define OS_CFGINT_THREADS_ARRAY_SIZE                    (3)

//#define OS_EXCLUDE_MULTITASKING                         (1)

#define OS_INCLUDE_CLOG                                 (1)
#define OS_INCLUDE_SIMPLECLI                            (1)
#define OS_INCLUDE_SIMPLECLI_PARSER                     (1)

#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_SHORT            (1)
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_LONG             (1)
#define OS_INCLUDE_SIMPLECLI_PARSE_HEX_NIBBLE           (1)
#define OS_INCLUDE_SIMPLECLI_PARSE_UNSIGNED_LONG        (1)

#define OS_INCLUDE_OSDEVICECHARACTER                    (1)
#define OS_INCLUDE_DEVICECHARACTERUSB                   (1)
#define OS_INCLUDE_OSUSBDEVICE                          (1)

#define OS_INCLUDE_OSTHREAD_GETSTACKUSED                (1)

#define OS_INCLUDE_ISTREAM                              (1)
#define OS_INCLUDE_OSTREAM                              (1)
#define OS_INCLUDE_OSTREAM_OSTHREAD                     (1)

//#define OS_INCLUDE_TASKBLINK                            (1)

#define OS_INCLUDE_OS_BUSYWAITMICROS                    (1)

#define OS_INCLUDE_AVR32_UC3_MCI                        (1)
#define OS_INCLUDE_AVR32_UC3_GPIO                       (1)
#define OS_INCLUDE_AVR32_UC3_PM                         (1)

#define OS_INCLUDE_AVR32_UC3_SPI                        (1)

#define OS_INCLUDE_CHIPSELECTACTIVELOW                  (1)

#define OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI              (1)
//#define OS_INCLUDE_AVR32_UC3_MEMORYCARDSPI              (1)
#define OS_INCLUDE_OSDEVICEMEMORYCARD                   (1)

// ----------------------------------------------------------------------------

#if defined(DEBUG)

#define OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS          (1)

//#define OS_DEBUG_OSSCHEDULER_STACKINITIALIZE            (1)
#define OS_DEBUG_CONSTRUCTORS                           (1)
#define OS_DEBUG_OSSCHEDULER_START                      (1)
//#define OS_DEBUG_OSSCHEDULER_CONTEXTSWITCH              (1)
//#define OS_DEBUG_OSSCHEDULER_THREADEGISTER              (1)
//#define OS_DEBUG_OSREADYLIST_INSERT                     (1)
//#define OS_DEBUG_OSSCHEDULER_DUMPSTACK                  (1)
//#define OS_DEBUG_CONSTRUCTORS_INIT                      (1)

#if true
#define OS_DEBUG_AVR32_UC3_MCI_READDATA                 (1)
#define OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SENDCOMMAND (1)
#define OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_READRESPONSE (1)
#define OS_DEBUG_AVR32_MCI_MEMORYCARDMCI_IMPLEMENTATION_SELECTCARD (1)
#define OS_DEBUG_AVR32_UC3_MCI_CONFIGURECLOCKFREQUENCYHZ (1)
#define OS_DEBUG_AVR32_UC3_MCI_SENDCOMMAND              (1)
#endif

#endif /* DEBUG */

// ----------------------------------------------------------------------------

#endif /* OS_APP_DEFINES_H_ */
