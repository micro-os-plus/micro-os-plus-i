/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEDEBUGI2CIMPL_H_
#define DEVICEDEBUGI2CIMPL_H_

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OSCPUImpl.h"

#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

#include "portable/misc/include/CircularByteBuffer.h"

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

//#include <stdint.h>

class DeviceDebugI2cImpl
{
public:
  static void
  implEarlyInit(void);

#if defined(OS_INCLUDE_DEVICEDEBUGI2C_SINGLE_BYTE)
  // Ouput one byte on the debug interface.
  static void
  implPutByte(unsigned char c);
#endif

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

  static void
  implFlush(void);

  static OSStack_t
  criticalEnter(void);

  static void
  criticalExit(OSStack_t mask);

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

#if defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER)

public:

  class CircularBuffer : public CircularByteBuffer
  {
  public:
    void
    initialise(char* pc, unsigned short size);

#if true
    bool
    isEmpty();
    unsigned short
    putBytes(const char* pc, unsigned short size);
    unsigned char
    get(void);

  private:
#endif
  };

protected:

  static char ms_buffer[];
  static CircularBuffer ms_circularBuffer;

public:

  class Lock
  {
  public:
    void
    initialise(void);

    bool
    aquire(void);
    void
    release(void);

  private:
    bool m_isAquired;
  };

protected:

  static Lock ms_lock;

#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_BUFFER) */

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
#define OS_CFGINT_DEBUG_I2C_EMU_DELAY_COUNT (1)
#endif

//#if !defined(OS_EXCLUDE_OSDEVICEDEBUG_IMPLEMENTATION)

// ----- Public methods -------------------------------------------------------

inline OSStack_t
DeviceDebugI2cImpl::criticalEnter(void)
{
  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();
#if defined(OS_INCLUDE_OSDEVICEDEBUG_MASK_INTERRUPTS)
  OSCPUImpl::setInterruptsMask(tmp | OS_CFGINT_OSDEVICEDEBUG_MASK);
#else
  OSCPUImpl::interruptsDisable();
#endif
  return tmp;
}

inline void
DeviceDebugI2cImpl::criticalExit(OSStack_t mask)
{
  OSCPUImpl::setInterruptsMask(mask);
}

// ----------------------------------------------------------------------------

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
#if true
  OSCPUImpl::watchdogReset();
#else
  IWDG_ReloadCounter();
#endif
}

// ---------------------------------------------------------------------------
// Default definitions for _CONFIG, _WRITE, _READ based on the generic port

#if !defined(OS_CONFIG_DEBUG_SCL_PORT_CONFIG) && defined(OS_CONFIG_DEBUG_SCL_PORT)
#define OS_CONFIG_DEBUG_SCL_PORT_CONFIG  OS_CONFIG_DEBUG_SCL_PORT
#endif

#if !defined(OS_CONFIG_DEBUG_SCL_PORT_WRITE) && defined(OS_CONFIG_DEBUG_SCL_PORT)
#define OS_CONFIG_DEBUG_SCL_PORT_WRITE  OS_CONFIG_DEBUG_SCL_PORT
#endif

#if !defined(OS_CONFIG_DEBUG_SCL_PORT_READ) && defined(OS_CONFIG_DEBUG_SCL_PORT)
#define OS_CONFIG_DEBUG_SCL_PORT_READ  OS_CONFIG_DEBUG_SCL_PORT
#endif

#if !defined(OS_CONFIG_DEBUG_SDA_PORT_CONFIG) && defined(OS_CONFIG_DEBUG_SDA_PORT)
#define OS_CONFIG_DEBUG_SDA_PORT_CONFIG  OS_CONFIG_DEBUG_SDA_PORT
#endif

#if !defined(OS_CONFIG_DEBUG_SDA_PORT_WRITE) && defined(OS_CONFIG_DEBUG_SDA_PORT)
#define OS_CONFIG_DEBUG_SDA_PORT_WRITE  OS_CONFIG_DEBUG_SDA_PORT
#endif

#if !defined(OS_CONFIG_DEBUG_SDA_PORT_READ) && defined(OS_CONFIG_DEBUG_SDA_PORT)
#define OS_CONFIG_DEBUG_SDA_PORT_READ  OS_CONFIG_DEBUG_SDA_PORT
#endif

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

  // Enable the pin (required to allow read)
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_DEBUG_SCL_PORT_CONFIG, OS_CONFIG_DEBUG_SCL_BIT);
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

  // Enable the pin (required to allow read)
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_DEBUG_SDA_PORT_CONFIG, OS_CONFIG_DEBUG_SDA_BIT);
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
