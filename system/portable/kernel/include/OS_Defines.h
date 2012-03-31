/*
 *      Copyright (C) 2007-2012 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OS_DEFINES_H_
#define OS_DEFINES_H_

// ----------------------------------------------------------------------------
// Notice: since this is also included in assembly source files,
// it should not include anything else than preprocessor defines.

// ----------------------------------------------------------------------------
// Operating System versioning and greeting definitions

// Notice: do not use parenthesis! (the values will be stringified)
#define OS_CFGINT_VERSION_MAJOR         4
#define OS_CFGINT_VERSION_MINOR         3
#define OS_CFGINT_VERSION_REVISION      7628

#define OS_CFGSTR_STRINGIFY(x)          OS_CFGSTR_SHARP(x)
#define OS_CFGSTR_SHARP(x)              #x

// stringify above integers
#define OS_CFGSTR_VERSION_MAJOR         OS_CFGSTR_STRINGIFY(OS_CFGINT_VERSION_MAJOR)
#define OS_CFGSTR_VERSION_MINOR         OS_CFGSTR_STRINGIFY(OS_CFGINT_VERSION_MINOR)
#define OS_CFGSTR_VERSION_REVISION      OS_CFGSTR_STRINGIFY(OS_CFGINT_VERSION_REVISION)

#define OS_CFGSTR_VERSION       OS_CFGSTR_VERSION_MAJOR "." OS_CFGSTR_VERSION_MINOR "." OS_CFGSTR_VERSION_REVISION

#define OS_CFGSTR_GREETING      "uOS++ v" OS_CFGSTR_VERSION ", GCC " __VERSION__

// ----------------------------------------------------------------------------

// include additional OS definitions from the application space
#include "OS_App_Defines.h"

// include board OS definitions
#if defined(OS_CONFIG_BOARD_ATMEL_STK525)
#include "hal/boards/Atmel/stk525/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)
#include "hal/boards/Atmel/evk1104/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_METRILOG_M512)
#include "hal/boards/Metrilog/m512/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_METRILOG_M514)
#include "hal/boards/Metrilog/m514/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A0739)
#include "hal/boards/AVI/a0739/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07523)
#include "hal/boards/AVI/a07523/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_TEMPLATE)
#include "hal/boards/TEMPLATE_MANUFACTURER/TEMPLATE_BOARD/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)
#include "hal/boards/Olimex/stm32_h103/include/OS_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_CUSTOM)
#include "custom/include/OS_Board_Defines.h"

#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

// include architecture OS definitions
// please note the inclusion order: first family and then architecture

#if defined(OS_CONFIG_ARCH_AVR8)
#  include "hal/arch/avr8/kernel/include/OS_Arch_Defines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)

#if defined(OS_CONFIG_FAMILY_STM32F10X)
#  include "hal/arch/arm_cortex_m3/stm32f10x/kernel/include/OS_Family_Defines.h"
#else
#  error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#  include "hal/arch/arm_cortex_m3/kernel/include/OS_Arch_Defines.h"

#elif defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#  include "hal/arch/avr32/uc3/kernel/include/OS_Family_Defines.h"
#else
#  error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#  include "hal/arch/avr32/kernel/include/OS_Arch_Defines.h"

#elif defined(OS_CONFIG_ARCH_TEMPLATE)

#if defined(OS_CONFIG_FAMILY_TEMPLATE)
#  include "hal/arch/TEMPLATE_ARCH/TEMPLATE_FAMILY/kernel/include/OS_Family_Defines.h"
#else
#  error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#  include "hal/arch/TEMPLATE_ARCH/kernel/include/OS_Arch_Defines.h"

#else
#  error "Missing OS_CONFIG_ARCH_* definition"
#endif

#if !defined(DEBUG_ASSERT)
#  if !defined(DEBUG)
#    define DEBUG_ASSERT(e)   ((void)0)
#  else /* DEBUG */
#    define DEBUG_ASSERT(e) ((e) ? (void)0 : OSDeviceDebug::assert(__func__, __FILE__, __LINE__, #e))
#  endif
#endif

// ----- Default definitions for various variables ----------------------------

#if defined(OS_CONFIG_ARCH_AVR8)

#if !defined(OS_CFGINT_CLOCK_PRESCALER)
#define OS_CFGINT_CLOCK_PRESCALER (1)
#endif

#if !defined(OS_CFGINT_CLOCK_MULTIPLIER)
#define OS_CFGINT_CLOCK_MULTIPLIER (1)
#endif

#if !defined(OS_CFGLONG_SYSCLOCK_HZ)
#define OS_CFGLONG_SYSCLOCK_HZ                          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER * OS_CFGINT_CLOCK_MULTIPLIER)
#endif

#endif /* defined(OS_CONFIG_ARCH_AVR8) */

// ----- Interdependencies between various definitions ------------------------

#if !defined(OS_INCLUDE_OSEVENTFLAGS) && defined(OS_INCLUDE_SDI12SENSOR)
#define OS_INCLUDE_OSEVENTFLAGS                         (1)
#endif

// ----- Critical sections interdependencies ----------------------------------

#if !defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK) && (defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) || defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS))
#define OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK   (1)
#endif

#if !defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) && defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) && !defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)
#define OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL  (1)
#endif

// ----- OSThreadIdle dependencies --------------------------------------------

#if !defined(OS_INCLUDE_OSTHREAD_SLEEP) && defined(OS_INCLUDE_OSCPUSLEEPCRITICALSECTION)
#define OS_INCLUDE_OSTHREAD_SLEEP                       (1)
#endif

// ----- OSDeviceCharacter dependencies ---------------------------------------

#if !defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) && (defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) || defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1))
#define OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE     (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICECHARACTERBUFFERED) && defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE)
#define OS_INCLUDE_OSDEVICECHARACTERBUFFERED            (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICECHARACTERBUFFERED) && (defined(OS_INCLUDE_DEVICECHARACTERUSART1) || defined(OS_INCLUDE_DEVICECHARACTERUSART0))
#define OS_INCLUDE_OSDEVICECHARACTERBUFFERED            (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICECHARACTER) && defined(OS_INCLUDE_OSDEVICECHARACTERBUFFERED)
#define OS_INCLUDE_OSDEVICECHARACTER                    (1)
#endif

#if false
#if !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART) && (defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART1) || defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART0))
#define OS_INCLUDE_DEVICECHARACTERMULTIUSART            (1)
#endif

#if !defined(OS_INCLUDE_DEVICECHARACTERUSART) && defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
#define OS_INCLUDE_DEVICECHARACTERUSART                 (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICECHARACTER) && defined(OS_INCLUDE_DEVICECHARACTERUSART)
#define OS_INCLUDE_OSDEVICECHARACTER                    (1)
#endif
#endif

#if !defined(OS_INCLUDE_CIRCULARBYTEBUFFER) && defined(OS_INCLUDE_OSDEVICECHARACTERBUFFERED)
#define OS_INCLUDE_CIRCULARBYTEBUFFER                   (1)
#endif

// ----- OSDeviceMemoryCard dependencies -------------------------------------

#if !defined(OS_INCLUDE_OSDEVICEBLOCK) && defined(OS_INCLUDE_AVR32_UC3_MEMORYCARDMCI)
#define OS_INCLUDE_OSDEVICEMEMORYCARD                   (1)
#endif

// ----- OSDeviceAddressable dependencies -------------------------------------

#if !defined(OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED) && defined(OS_INCLUDE_OSDEVICEADDRESSABLEPARTITION)
#define OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED      (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICEADDRESSABLE) && defined(OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED)
#define OS_INCLUDE_OSDEVICEADDRESSABLE                  (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICEADDRESSABLE) && defined(OS_INCLUDE_LARGECIRCULARSTORAGE)
#define OS_INCLUDE_OSDEVICEADDRESSABLE                  (1)
#endif

// ----- OSDeviceBlock dependencies -------------------------------------------

#if !defined(OS_INCLUDE_OSDEVICEBLOCK) && defined(OS_INCLUDE_OSDEVICEMEMORYCARD)
#define OS_INCLUDE_OSDEVICEBLOCK                        (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICEBLOCKEXTENSION) && defined(OS_INCLUDE_OSDEVICEBLOCKPARTITION)
#define OS_INCLUDE_OSDEVICEBLOCKEXTENSION               (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICEBLOCKEXTENSION) && defined(OS_INCLUDE_OSDEVICEBLOCKSYNCHRONISED)
#define OS_INCLUDE_OSDEVICEBLOCKEXTENSION               (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICEBLOCK) && defined(OS_INCLUDE_OSDEVICEBLOCKEXTENSION)
#define OS_INCLUDE_OSDEVICEBLOCK                        (1)
#endif

// ----- OSEventNotifier dependencies -------------------------------------------

#if !defined(OS_INCLUDE_OSEVENTNOTIFIERARRAY) && defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE)
#define OS_INCLUDE_OSEVENTNOTIFIERARRAY                 (1)
#endif

#if !defined(OS_INCLUDE_OSEVENTNOTIFIER) && defined(OS_INCLUDE_OSEVENTNOTIFIERARRAY)
#define OS_INCLUDE_OSEVENTNOTIFIER                      (1)
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_CFGINT_OSTHREAD_CRITICALSECTIONNESTINGSTACK_ARRAY_SIZE)
#define OS_CFGINT_OSTHREAD_CRITICALSECTIONNESTINGSTACK_ARRAY_SIZE       (10)
#endif /* !defined(OS_CFGINT_OSTHREAD_CRITICALSECTIONNESTINGSTACK_ARRAY_SIZE) */

#if !defined(OS_INCLUDE_OSUSBDEVICE) && defined(OS_INCLUDE_DEVICECHARACTERUSB)
#define OS_INCLUDE_OSUSBDEVICE
#endif

#if !defined(OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS) && defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
#define OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS         (3)
#endif

#if defined(OS_EXCLUDE_MULTITASKING)
#undef OS_CFGINT_THREADS_ARRAY_SIZE
#define OS_EXCLUDE_OSTIMER                              (1)
#endif /* defined(OS_EXCLUDE_MULTITASKING) */

#if !defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) && defined(OS_INCLUDE_SDI12SENSOR)
#define OS_INCLUDE_OSTHREAD_INTERRUPTION                  (1)
#endif

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS) && !defined(OS_INCLUDE_OSTIMERSECONDS)
#define OS_INCLUDE_OSTIMERSECONDS                       (1)
#endif

#if defined(OS_INCLUDE_OSTIMERSECONDS) && (!defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT) && !defined(OS_INCLUDE_32KHZ_TIMER) && !defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_EXTERNAL))
#define OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT        (1)
#endif

#if !defined(OS_INCLUDE_TIMER) && defined(OS_INCLUDE_OSTIMERSECONDS)
#define OS_INCLUDE_TIMER                                (1)
#endif

// ----- streambuf ------------------------------------------------------------

#if !defined(OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF) && defined(OS_INCLUDE_CLOG)
#define OS_INCLUDE_OSDEVICEDEBUG_STREAMBUF              (1)
#endif

#if !defined(OS_INCLUDE_OSTREAM) && defined(OS_INCLUDE_CLOG)
#define OS_INCLUDE_OSTREAM                              (1)
#endif

#if !defined(OS_INCLUDE_OSTREAM) && (defined(OS_INCLUDE_SIMPLECLI) || defined(OS_INCLUDE_COMMANDLINEINTERFACE))
#define OS_INCLUDE_OSTREAM                              (1)
#endif

#if !defined(OS_INCLUDE_ISTREAM) && (defined(OS_INCLUDE_SIMPLECLI) || defined(OS_INCLUDE_COMMANDLINEINTERFACE))
#define OS_INCLUDE_ISTREAM                              (1)
#endif

#if !defined(OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF) && (defined(OS_INCLUDE_SIMPLECLI) || defined(OS_INCLUDE_COMMANDLINEINTERFACE))
#define OS_INCLUDE_OSDEVICECHARACTER_STREAMBUF          (1)
#endif

#if !defined(OS_INCLUDE_OSTREAM_LONG) && (__SIZEOF_INT__ == 4)
#define OS_INCLUDE_OSTREAM_LONG                         (1)
#endif

// ----- PROGMEM --------------------------------------------------------------

#if !defined(OS_INCLUDE_SEPARATE_PROGMEM)
#define PROGMEM
#define PSTR(_S_) ((const char*)_S_)
#define putString_P putString
#endif

// ----- LOG definitions ------------------------------------------------------

#define OS_CFGINT_OSLOGGER_ARRAY_SIZE 16

#endif /* OS_DEFINES_H_ */
