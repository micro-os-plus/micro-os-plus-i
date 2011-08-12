/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEDEBUGI2CIMPL_H_
#define DEVICEDEBUGI2CIMPL_H_

#include "portable/kernel/include/OS_Defines.h"

class DeviceDebugI2cImpl
{
public:
  static void
  implEarlyInit(void);

  // Ouput one byte on the debug interface.
  static void
  implPutByte(unsigned char c);

  // Ouput the given buffer on the debug interface.
  static int
  implPutBytes(const char* s, unsigned int n);

  // Return true if the debug interface can output now.
  static bool
  implCanTransmit(void);

  // Return true if the debug interface is connected with the MCU.
  static bool
  implDevicePresent(void);

  // Watchdog reset.
  static void
  implWDReset(void);

private:

  const static int I2C_WRITE = 0x0;
  const static int I2C_READ = 0x1;

  // Implement I2C master by bitbanging.
  static void
  masterInit(void);

  static void
  masterSclInit(void);
  static void
  masterSclHigh(void);
  static void
  masterSclLow(void);
  static bool
  masterSclIsLow(void);

  static void
  masterSdaInit(void);
  static void
  masterSdaHigh(void);
  static void
  masterSdaLow(void);
  static bool
  masterSdaIsLow(void);

  static void
  masterSclSynchronize(void);
  static void
  masterSdaSynchronize(void);

  static void
  masterStart(void);
  static void
  masterDelay(int n) __attribute__((noinline));

  static void
  masterPutAddress(unsigned char addr, unsigned char mode);
  static void
  masterPutByte(unsigned char c);
  //static unsigned char masterGetByte(void);
  static void
  masterPutAck(bool b);
  static bool
  masterGetAck(void);
  static void
  masterStop(void);
  static void
  masterCleanup(void);
};

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

#elif defined(OS_CONFIG_BOARD_CUSTOM)
#include "custom/include/DeviceDebugI2C_Board_Defines.h"

#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#if !defined(OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT)
#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT (0)
#endif

//#if !defined(OS_EXCLUDE_OSDEVICEDEBUG_IMPLEMENTATION)

// ----- Public methods -------------------------------------------------------

inline bool
DeviceDebugI2cImpl::implCanTransmit(void)
{
  return true;
}

inline bool
DeviceDebugI2cImpl::implDevicePresent(void)
{
  return !masterSdaIsLow();
}

inline void
DeviceDebugI2cImpl::implWDReset(void)
{
#if false
  IWDG_ReloadCounter();
#endif
}

inline void
DeviceDebugI2cImpl::implEarlyInit(void)
{
#if true
    {
      unsigned int _i, _j;
      for (_i = 0; _i < OS_CFGINT_DEBUG_I2C_EMU_INIT_DELAY_COUNT; ++_i)
        for (_j = 0; _j < OS_CFGINT_BUSYWAIT_CALIBRATION; ++_j)
          implWDReset();
    }
#endif

  masterInit();

  masterSclInit();
  masterSdaInit();
}

inline void
DeviceDebugI2cImpl::implPutByte(unsigned char c)
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

inline int
DeviceDebugI2cImpl::implPutBytes(const char* s, unsigned int n)
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

// ---------------------------------------------------------------------------
// Implementation of the bitbanged i2c master.

inline void
DeviceDebugI2cImpl::masterInit(void)
{
  ;
}

inline void
DeviceDebugI2cImpl::masterSclInit(void)
{
  // Set the pin value to 0; when the pin direction will be configured
  // to output, this will set the level to low.
  OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_SCL_PORT_WRITE, OS_CONFIG_DEBUG_SCL_BIT);

  // Set the pin direction to input, i.e. allow the
  // pull-ups to raise the level to high.
  OS_GPIO_PIN_CONFIG_INPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline void
DeviceDebugI2cImpl::masterSclHigh(void)
{
  // The pull-up will do the job.
  OS_GPIO_PIN_CONFIG_INPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline void
DeviceDebugI2cImpl::masterSclLow(void)
{
  // Configure direction to output, the value was already set to 0.
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline bool
DeviceDebugI2cImpl::masterSclIsLow(void)
{
  return OS_GPIO_PIN_ISLOW(OS_CONFIG_DEBUG_SCL_PORT_READ,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline void
DeviceDebugI2cImpl::masterSdaInit(void)
{
  // Set the pin value to 0; when the pin direction will be configured
  // to output, this will set the level to low.
  OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_SDA_PORT_WRITE, OS_CONFIG_DEBUG_SDA_BIT);

  // Set the pin direction to input, i.e. allow the
  // pull-ups to raise the level to high.
  OS_GPIO_PIN_CONFIG_INPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline void
DeviceDebugI2cImpl::masterSdaHigh(void)
{
  // The pull-up will do the job.
  OS_GPIO_PIN_CONFIG_INPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline void
DeviceDebugI2cImpl::masterSdaLow(void)
{
  // Configure direction to output, the value was already set to 0.
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline bool
DeviceDebugI2cImpl::masterSdaIsLow(void)
{
  return OS_GPIO_PIN_ISLOW(OS_CONFIG_DEBUG_SDA_PORT_READ,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline void
DeviceDebugI2cImpl::masterSclSynchronize(void)
{
  while (masterSclIsLow())
    {
      implWDReset();
    }
}

inline void
DeviceDebugI2cImpl::masterSdaSynchronize(void)
{
  while (masterSdaIsLow())
    {
      implWDReset();
    }
}

//#endif /* OS_EXCLUDE_OSDEVICEDEBUG_IMPLEMENTATION */

#if false
#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/devices/debug/include/DeviceDebugI2CEmuMaster_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)
//#if defined(OS_CONFIG_FAMILY_AVR32UC3)
//#include "hal/arch/avr32/uc3/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
//#else
//#error "Missing OS_CONFIG_FAMILY_* definition"
//#endif

#elif defined(OS_CONFIG_ARCH_TEMPLATE)
#if defined(OS_CONFIG_FAMILY_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/TEMPLATE_FAMILY/devices/debug/include/DeviceDebugI2CEmuMaster_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif
#endif /* false */

#endif /* DEVICEDEBUGI2CIMPL_H_ */
