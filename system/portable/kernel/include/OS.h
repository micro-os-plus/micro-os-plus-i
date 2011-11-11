/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_H_
#define OS_H_

// ----------------------------------------------------------------------------

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OS_StdTypes.h"

// ----------------------------------------------------------------------------

typedef uint16_t OSStackSize_t;

// ----------------------------------------------------------------------------

#include "portable/kernel/include/OSReturn.h"

// ----------------------------------------------------------------------------

typedef unsigned short OSTimerTicks_t;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS)

class OSTimeout
  {
  public:
    const static OSTimerTicks_t OS_IMMEDIATELY = 0;
    const static OSTimerTicks_t OS_NEVER = (0 - 1);
  };
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_TIMEOUTS) */

// ----------------------------------------------------------------------------

// must allow to cast a pointer
typedef unsigned int OSEvent_t;

class OSEvent
{
public:
  const static OSEvent_t OS_NONE = (0-1);
  const static OSEvent_t OS_ALL = 0;
  const static OSEvent_t OS_TIMEOUT = 1;
  const static OSEvent_t OS_CUSTOM_TIMER = 2;
};

// ----------------------------------------------------------------------------

// must allow to cast a pointer
typedef unsigned int OSEventWaitReturn_t;

class OSEventWaitReturn
{
public:
  const static OSEventWaitReturn_t OS_NONE = (0-1);
  const static OSEventWaitReturn_t OS_VOID = 0;
  const static OSEventWaitReturn_t OS_LOCKED = 1;
  const static OSEventWaitReturn_t OS_TIMEOUT = 2;
  const static OSEventWaitReturn_t OS_CANCELLED = 3;
  const static OSEventWaitReturn_t OS_ALL = 4;
  const static OSEventWaitReturn_t OS_IMMEDIATELY = 5;
  const static OSEventWaitReturn_t OS_FAILED = 6;
  const static OSEventWaitReturn_t OS_WOULD_BLOCK = 7;

  // user values should be relative to this one
  const static OSEventWaitReturn_t OS_CUSTOM = 10;
};

#include "portable/kernel/include/OSCPUImpl.h"

// ----------------------------------------------------------------------------

class OSImpl
{
public:
  static void
  familyEarlyInit(void);
};

// ----------------------------------------------------------------------------

class OSApplicationImpl
{
public:

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_LOGEXCEPTIONDETAILS)
  static void logExceptionDetails(unsigned int n);
#endif /* OS_INCLUDE_OSSAPPLICATIONIMPL_LOGEXCEPTIONDETAILS */

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)
  static void
  interruptTick(void);
#endif /* OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK */

#if defined(OS_INCLUDE_OSAPPLICATIONIMPL_NONMASKABLEINTERRUPTHANDLER)
  static void
  nonMaskableInterruptHandler(void);
#endif

#if defined(OS_INCLUDE_OSAPPLICATIONIMPL_INITIALISEEXTERNALCLOCK)
  static void
  earlyInitialiseExternalClock(void);
  static void
  initialiseExternalClock(void);
#endif

#if defined(OS_INCLUDE_OSAPPLICATIONIMPL_PREPARESLEEP)
  static void
  prepareSleep(bool isDeepSleepAllowed);
#endif /* defined(OS_INCLUDE_OSAPPLICATIONIMPL_PREPARESLEEP) */

};

// ----------------------------------------------------------------------------

#include "portable/devices/debug/include/OSDeviceDebug.h"

#include "portable/kernel/include/OSCriticalSection.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OSScheduler.h"

#include "portable/kernel/include/OSThread.h"

#include "portable/kernel/include/OSRealTime.h"

#include "portable/kernel/include/OSTimer.h"

#if defined(OS_INCLUDE_OSTIMERSECONDS)
#include "portable/kernel/include/OSTimerSeconds.h"
#endif

#if defined(OS_INCLUDE_OSTIMERRTC)
#include "portable/kernel/include/OSTimerRtc.h"
#endif

#include "portable/kernel/include/OSTimerTicks.h"

#include "portable/kernel/include/Timer.h"

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

// ----------------------------------------------------------------------------

class OSCPU : public OSCPUImpl
{
  // inherit all methods from CPU implementation
public:
#if defined(DEBUG)
  // Constructor.
  OSCPU();
#endif
  // Returns the reset bits used to know the reset reason.
  static OSResetBits_t
  getResetBits(void);

  // save to private member the reset bits using implementation dependent method
  static void
  saveResetBits(void);

private:
  // internal variable to keep reset bits
  static OSResetBits_t ms_resetBits;

  // Added to avoid the ABI warning "contains empty classes..."
  char m_dummy;
};

inline OSResetBits_t
OSCPU::getResetBits(void)
{
  return ms_resetBits;
}

// ----------------------------------------------------------------------------

class OS : public OSImpl
{

public:

#if !defined(OS_EXCLUDE_MULTITASKING)
  // The cpu object to be used by applications
  OSCPU cpu;

  // Scheduler object used by this class.
  OSScheduler sched;

#if defined(OS_INCLUDE_OSREALTIME)

  // Real time support
  OSRealTime rt;

#endif /* defined(OS_INCLUDE_OSREALTIME) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

#if !defined(OS_EXCLUDE_MULTITASKING)
#if defined(DEBUG)
  // Constructor.
  OS();
#endif
#else
  // Used when multitasking is disabled
  static void main(void) __attribute__((noreturn));
#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

#if defined(OS_INCLUDE_NAKED_INIT)
  // Naked version for the earlyInit function.
  static void nakedEarlyInit(void) __attribute__((naked, section(".init6")));
#endif /* defined(OS_INCLUDE_NAKED_INIT) */

  // Performs early initialisations.
  // Must be called before all constructors.
  static void
  earlyInit(void);

  // isDebug function return true if debug is active, false otherwise.
#if defined(DEBUG)

  //const static bool isDebug = true;
  inline static bool
  isDebug()
  {
    return true;
  }
  ;
#else

  //const static bool isDebug = false;
  inline static bool isDebug()
    {
      return false;
    }
  ;
#endif

#if defined(OS_INCLUDE_OS_BUSYWAITMILLIS)
  // Busy waiting for n milliseconds.
  static void
  busyWaitMillis(uint_t n);
#endif /* defined(OS_INCLUDE_OS_BUSYWAITMILLIS) */

#if defined(OS_INCLUDE_OS_BUSYWAITMICROS)
  // Busy waiting for n microseconds.
  static void
  busyWaitMicros(uint_t n);
#endif /* defined(OS_INCLUDE_OS_BUSYWAITMICROS) */

  static void
  resetHandler(void) __attribute__((naked, noreturn));

private:
  static void
  dataInit(void);
  static void
  bssInit(void);
  static void
  staticConstructorsInit(void);
};

// ----------------------------------------------------------------------------

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/include/OS_Arch_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#include "hal/arch/arm_cortex_m3/kernel/include/OS_Arch_Inlines.h"
#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/kernel/include/OS_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)
#include "hal/arch/avr32/kernel/include/OS_Arch_Inlines.h"
#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/kernel/include/OS_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/kernel/include/OS_Arch_Inlines.h"
#if defined(OS_CONFIG_FAMILY_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/TEMPLATE_FAMILY/kernel/include/OS_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

// ----------------------------------------------------------------------------

#endif /* OS_H_ */
