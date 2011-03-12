/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_H_
#define OS_H_

#include "portable/kernel/include/OS_Defines.h"

// ----------------------------------------------------------------------------

typedef int OSReturn_t;

class OSReturn
  {
public:
  static const OSReturn_t OS_OK = 0;
  static const OSReturn_t OS_TIMEOUT = -1;
  static const OSReturn_t OS_DISCONNECTED = -2;
  static const OSReturn_t OS_WOULD_BLOCK = -3;
  static const OSReturn_t OS_NOT_OWNER = -4;
  static const OSReturn_t OS_NOT_ACQUIRED = -5;
  static const OSReturn_t OS_ALREADY_OPENED = -6;
  static const OSReturn_t OS_NOT_OPENED = -7;
  static const OSReturn_t OS_TOO_LONG = -8;
  static const OSReturn_t OS_ILLEGAL_CHARACTER = -9;
  static const OSReturn_t OS_BAD_CHECKSUM = -10;
  static const OSReturn_t OS_NOT_INITIALIZED = -11;
  static const OSReturn_t OS_BAD_PARAMETER = -12;
  static const OSReturn_t OS_BUFFER_FULL = -13;
  static const OSReturn_t OS_IMMEDIATELY = -14;
  static const OSReturn_t OS_ITEM_NOT_FOUND = -15;
  static const OSReturn_t OS_NOT_ENOUGH_SPACE = -16;
  };

// ----------------------------------------------------------------------------

#if false
class OSTimeout
  {
public:
  static const OSTimerTicks_t OS_IMMEDIATELY = 0;
  static const OSTimerTicks_t OS_NEVER = (0 - 1);
  };
#endif

// ----------------------------------------------------------------------------

// must allow to cast a pointer
typedef unsigned int OSEvent_t;

class OSEvent
{
public:
  static const OSEvent_t OS_NONE = -1;
  static const OSEvent_t OS_ALL = 0;
  static const OSEvent_t OS_TIMEOUT = 1;
  static const OSEvent_t OS_CUSTOM_TIMER = 2;
};

// ----------------------------------------------------------------------------

// must allow to cast a pointer
typedef unsigned int OSEventWaitReturn_t;

class OSEventWaitReturn
{
public:
  static const OSEventWaitReturn_t OS_NONE = -1;
  static const OSEventWaitReturn_t OS_VOID = 0;
  static const OSEventWaitReturn_t OS_LOCKED = 1;
  static const OSEventWaitReturn_t OS_TIMEOUT = 2;
  static const OSEventWaitReturn_t OS_CANCELED = 3;
  static const OSEventWaitReturn_t OS_ALL = 4;
  static const OSEventWaitReturn_t OS_IMMEDIATELY = 5;
  static const OSEventWaitReturn_t OS_FAILED = 6;

  // user values should be relative to this one
  static const OSEventWaitReturn_t OS_CUSTOM = 10;
};

// ----------------------------------------------------------------------------

class OSCPUImpl
  {
public:
  // Initialise the SP register in order to point to the stack section.
  inline static void stackInit(void) __attribute__((always_inline));
  // Some CPU initialisations.
  inline static void earlyInit(void) __attribute__((always_inline));
  // Put MCU to the idle sleep mode.
  inline static void idle(void) __attribute__((always_inline));
  // Put MCU to sleep.
  inline static void sleep(void) __attribute__((always_inline));
  // Put MCU to deep sleep.
  inline static void deepSleep(void) __attribute__((always_inline));
  // Returns the reset bits used to know the reset reason.
  inline static OSResetBits_t fetchResetBits(void) __attribute__((always_inline));
  // Reset the WDT.
  inline static void watchdogReset(void) __attribute__((always_inline));
  // No operation performed. Embeds the nop assembly command.
  inline static void nop(void) __attribute__((always_inline));
  // Generate a soft reset.
  inline static void softReset(void) __attribute__((always_inline));
  // Wrapper for the rete instruction.
  inline static void returnFromInterrupt(void) __attribute__((always_inline,noreturn));
  // Copy link register's (LR) value into the PC. This will restart execution
  // of the previously executed routine.
  inline static void returnFromSubroutine(void) __attribute__((always_inline,noreturn));

#if defined(OS_INLINE_INTERRUPTS_ENABLE_DISABLE)
  // Enable interrupts (global interrupt mask bit - GM from SR).
  inline static void interruptsEnable(void) __attribute__((always_inline));
  // Disable all interrupts (global interrupt mask bit - GM from SR).
  inline static void interruptsDisable(void) __attribute__((always_inline));
#else
  // Enable interrupts (global interrupt mask bit - GM from SR).
  inline static void interruptsEnable(void);
  // Disable all interrupts (global interrupt mask bit - GM from SR).
  inline static void interruptsDisable(void);
#endif

  // Used for ARM architecture.
  inline static void interruptsClearMask(void) __attribute__((always_inline));
  inline static void interruptsSetMask(void) __attribute__((always_inline));

private:

  };


// ----------------------------------------------------------------------------

class OSImpl
  {
public:
  static void familyEarlyInit(void);
  };

// ----------------------------------------------------------------------------

class OSApplicationImpl
{
public:

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_LOGEXCEPTIONDETAILS)
  static void logExceptionDetails(unsigned int n);
#endif /* OS_INCLUDE_OSSAPPLICATIONIMPL_LOGEXCEPTIONDETAILS */

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)
  static void interruptTick(void);
#endif /* OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK */

};

// ----------------------------------------------------------------------------

extern "C" OSStack_t **g_ppCurrentStack;

#include "portable/kernel/include/OSScheduler.h"

#include "portable/kernel/include/OSTask.h"

#include "portable/kernel/include/OSTimer.h"

#if defined(OS_INCLUDE_OSTIMERSECONDS)
#include "portable/kernel/include/OSTimerSeconds.h"
#endif

#include "portable/kernel/include/OSTimerTicks.h"

#include "portable/kernel/include/Timer.h"

#include "portable/devices/debug/include/OSDeviceDebug.h"

// ----------------------------------------------------------------------------

class OSCPU: public OSCPUImpl
{
  // inherit all methods from CPU implementation
public:
#if defined(DEBUG)
  // Constructor.
  OSCPU();
#endif
  // Returns the reset bits used to know the reset reason.
  static OSResetBits_t getResetBits(void);

  // save to private member the reset bits using implementation dependent method
  static void saveResetBits(void);

private:
  // internal variable to keep reset bits
  static OSResetBits_t ms_resetBits;

  // Added to avoid the ABI warning "contains empty classes..."
  char m_dummy;
};

inline OSResetBits_t OSCPU::getResetBits(void)
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
#endif

#if !defined(OS_EXCLUDE_MULTITASKING)
#if defined(DEBUG)
  // Constructor.
  OS();
#endif
#else
  // TODO: if is no longer used please remove it.
  static void main(void) __attribute__( ( noreturn ) );
#endif

#if defined(OS_INCLUDE_NAKED_INIT)
  // Naked version for the earlyInit function.
  static void nakedEarlyInit(void) __attribute__( ( naked, section( ".init6" ) ) );
#endif

  // Performs early initialisations.
  // Must be called before all constructors.
  static void earlyInit(void);

  // isDebug function return true if debug is active, false otherwise.
#if defined(DEBUG)

  //const static bool isDebug = true;
  inline static bool isDebug()
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
  static void busyWaitMillis(unsigned int n);
#endif

#if defined(OS_INCLUDE_OS_BUSYWAITMICROS)
  // Busy waiting for n microseconds.
  static void busyWaitMicros(unsigned int n);
#endif

private:
  };

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

#endif /*OS_H_ */
