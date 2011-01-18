/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef DEVICEDEBUGI2C_INLINES_H_
#define DEVICEDEBUGI2C_INLINES_H_

#if defined(OS_CONFIG_BOARD_ATMEL_STK525)
#include "hal/boards/Atmel/stk525/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_ATMEL_EVK1104)
#include "hal/boards/Atmel/evk1104/include/DeviceDebugI2C_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_METRILOG_M512)
#include "hal/boards/Metrilog/m512/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_METRILOG_M514)
#include "hal/boards/Metrilog/m514/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A0739)
#include "hal/boards/AVI/a0739/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07523)
#include "hal/boards/AVI/a07523/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07527)
#include "hal/boards/AVI/a07527/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07524)
#include "hal/boards/AVI/a07524/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_AVI_A07521)
#include "hal/boards/AVI/a07521/include/DeviceDebugI2C_Defines.h"

#elif defined(OS_CONFIG_BOARD_OLIMEX_STM32_H103)
#include "hal/boards/Olimex/stm32_h103/include/DeviceDebugI2C_Board_Defines.h"

#elif defined(OS_CONFIG_BOARD_TEMPLATE)
#include "hal/boards/TEMPLATE_MANUFACTURER/TEMPLATE_BOARD/include/DeviceDebugI2C_Board_Defines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

static void masterInit(void);

extern void masterSclInit(void);
extern void masterSclHigh(void);
extern void masterSclLow(void);
extern bool masterSclIsLow(void);

extern void masterSdaInit(void);
extern void masterSdaHigh(void);
extern void masterSdaLow(void);
extern bool masterSdaIsLow(void);

extern void masterSclSynchronize(void);
extern void masterSdaSynchronize(void);

extern void masterStart(void);
extern void masterDelay(int n) __attribute__( ( noinline ) );

extern void masterPutAddress(unsigned char addr, unsigned char mode);
extern void masterPutByte(unsigned char c);
//static unsigned char masterGetByte(void);
extern void masterPutAck(bool b);
extern bool masterGetAck(void);
extern void masterStop(void);
extern void masterCleanup(void);

static const int I2C_WRITE = 0x0;
static const int I2C_READ = 0x1;

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/devices/debug/include/DeviceDebugI2CEmuMaster_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)
#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_TEMPLATE)
#if defined(OS_CONFIG_FAMILY_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/TEMPLATE_FAMILY/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

// ----- public methods ------------------------------------------------------

inline void OSDeviceDebug::implEarlyInit(void)
  {
      {
        unsigned int _i, _j;
        for (_i = 0; _i < OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT; ++_i)
          for (_j = 0; _j < 8000; ++_j)
            implWDReset();
      }

    masterInit();

    masterSclInit();
    masterSdaInit();
  }

inline void OSDeviceDebug::implPutByte(unsigned char c)
  {
    for (;;) // loop until we succeed to transmit

      {
        masterStart();
        masterPutAddress(OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR, I2C_WRITE);
        if (!masterGetAck())
          {
            masterCleanup();
            goto retry;
          }

        masterPutByte(c);
        if (!masterGetAck())
          {
            masterCleanup();
            goto retry;
          }

        masterStop();
        break;

        retry: masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT * 10);
      }
  }

inline int OSDeviceDebug::implPutBytes(const char *s, unsigned int n)
  {
    unsigned int i;
#if defined(OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE)
    for (i = 0; i < n; ++i)
    implPutByte(s[i]);
#else
    for (i = 0; i < n;)
      { // loop until we succeed to transmit
        masterStart();
        masterPutAddress(OS_CFGINT8_DEBUG_I2C_DEBUGGER_ADDR, I2C_WRITE);
        if (!masterGetAck())
          {
            masterCleanup();
            goto retry;
          }

        for (; i < n; ++i)
          {
            masterPutByte(s[i]);
            if (!masterGetAck())
              {
                masterCleanup();
                goto retry;
              }
          }
        masterStop();
        break;

        retry: masterDelay(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT * 10);
      }
#endif
    return n;
  }

#endif /*DEVICEDEBUGI2C_INLINES_H_*/
