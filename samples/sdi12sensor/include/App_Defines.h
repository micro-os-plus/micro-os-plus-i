/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_DEFINES_H_
#define APP_DEFINES_H_

// ----------------------------------------------------------------------------

// Application versioning and greeting definitions
// More definitions will be computer in uOS.h

#define APP_CFGSTR_APPLICATION_NAME                     "sdi12sensor"
#define APP_CFGSTR_APPLICATION_MANUFACTURER             "AVI"

// Notice: do not use parenthesis! (the values will be stringified)
#define APP_CFGINT_VERSION_MAJOR                        1
#define APP_CFGINT_VERSION_MINOR                        2
#define APP_CFGINT_VERSION_REVISION                     1241

// ----------------------------------------------------------------------------

#define APP_CFGINT_SDI12SENSOR_PROTO_STACK_SIZE         (OSTask::STACK_MINIMAL_SIZE + 120)
#define APP_CFGINT_SDI12SENSOR_DACQ_STACK_SIZE          (OSTask::STACK_MINIMAL_SIZE + 80)
#define APP_CFGINT_SDI12SENSOR_PERIODIC_STACK_SIZE      (OSTask::STACK_MINIMAL_SIZE + 80)

#define OS_CFGINT_SDI12SENSOR_BUF_SIZE                  (75+5)

//#define OS_CFGBOOL_SDI12SENSOR_SUPPORT_WILD_CARD_ADDRESS (true)
//#define OS_CFGINT_SDI12SENSOR_MAX_DCMD_VALUES           (3)

#define APP_CFGINT_SDI12SENSOR_M0_DURATION              (2)
#define APP_CFGINT_SDI12SENSOR_M1_DURATION              (3)
#define APP_CFGINT_SDI12SENSOR_M2_DURATION              (4)

#define APP_CFGINT_SDI12SENSOR_M0_COUNT                 (3)
#define APP_CFGINT_SDI12SENSOR_M1_COUNT                 (6)
#define APP_CFGINT_SDI12SENSOR_M2_COUNT                 (9)

#define APP_CFGINT_SDI12SENSOR_PERIODICITY              (10)
#define APP_CFGINT_SDI12SENSOR_PERIODIC_DURATION        (3)

#if defined(OS_CONFIG_BOARD_ATMEL_STK525)

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)

#elif defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)

#else
#error "Missing OS_CONFIG_BOARD_* board definition"
#endif

#endif /* APP_DEFINES_H_ */
