/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSSCHEDULER_H_
#define OSSCHEDULER_H_

#include "portable/kernel/include/OS.h"

typedef unsigned short schedTicks_t;

class OSReadyList;
#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
class OSTimerSeconds;
#endif
class OSTimerTicks;
class OSTask;

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

  // user values should be relative to this one
  static const OSEventWaitReturn_t OS_CUSTOM = 10;
};

// ----------------------------------------------------------------------------

class OSScheduler
{
public:
#if defined(DEBUG)
  OSScheduler();
#endif

  static void
  earlyInit(void);

  // publicly available methods
  static void
  start(void) __attribute__( ( noreturn ) );
  static void
  startImpl(void) __attribute__( ( noreturn ) );

  static bool
  isRunning(void);

  static void
  lock(void);
  static void
  unlock(void);

  static bool
  isLocked(void);
  static void
  setLock(bool flag);

  inline static bool
  isPreemptive(void);
  static void
  setPreemptive(bool flag);

  static OSTask *
  getTaskCurrent(void);
  static OSTask *
  getTaskIdle(void);

  static int
  getTasksCount(void);
  static OSTask *
  getTask(int i);

  static bool
  isAllowDeepSleep(void);
  static void
  setAllowDeepSleep(bool flag);

  inline static void
  criticalEnter(void) __attribute__( ( always_inline ) );
  inline static void
  criticalExit(void) __attribute__( ( always_inline ) );

  // event/synchronization methods
  static OSEventWaitReturn_t
  eventWait(OSEvent_t event);

  static int
  eventNotify(OSEvent_t event, OSEventWaitReturn_t ret =
      OSEventWaitReturn::OS_VOID);

  static void
  yield(void);
  inline static void
  yieldImpl(void) __attribute__( ( always_inline ) );

  static OSStack_t *
  stackInitialize(OSStack_t * pStackTop, void
  (*entryPoint)(void *), void *pParams, unsigned char id);

  static void
  stackSetReturnedValue(OSStack_t * pStack, OSEventWaitReturn_t ret);

  inline static void
  ledActiveInit(void) __attribute__( ( always_inline ) );
  inline static void
  ISRledActiveOn(void) __attribute__( ( always_inline ) );
  inline static void
  ledActiveOn(void) __attribute__( ( always_inline ) );
  inline static void
  ISRledActiveOff(void) __attribute__( ( always_inline ) );
  inline static void
  ledActiveOff(void) __attribute__( ( always_inline ) );

  static unsigned char
  taskRegister(OSTask * pTask);

  static void
  interruptTick(void);

  static bool
  requireContextSwitch(void);
  static void
  contextSwitch(void);

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSWITCHREQUEST)
  static void
  ISRcontextSwitchRequest(void);
#endif

#if defined(OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT) || defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  inline static void
  interruptEnter(void) __attribute__( ( always_inline ) );
  inline static void
  interruptExit(void) __attribute__( ( always_inline ) );

#endif

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSAVE_RESTORE)
  inline static void
  contextSave(void) __attribute__( ( always_inline ) );
  inline static void
  contextRestore(void) __attribute__( ( always_inline ) );
#endif

  static OSTimerTicks timerTicks;

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
  static OSTimerSeconds timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  static void ISRcancelTask(OSTask *pTask);
#endif

  static OSTask *ms_pTaskRunning;

private:
  static void
  timerSetup(void);

  static void
  setTaskIdle(OSTask *);

  //static void timerISR(void) __attribute__( ( naked ) );
  static void
  timerISR(void);

  //static void dumpContextInfo(void *pContext);
  static void
  dumpContextInfo(OSTask *);

  // members
  static bool ms_isPreemptive;

  static bool ms_isRunning;
  static bool ms_isLocked;

  static OSTask *ms_tasks[OS_CFGINT_TASKS_TABLE_SIZE + 1];
  static unsigned char ms_tasksCount;
  //static unsigned char tasksIdx;

  static OSTask *ms_pTaskIdle;

  static OSReadyList ms_readyTasksList;

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  static bool ms_allowDeepSleep;
#endif

};

// ----------------------------------------------------------------------------

class OSSchedulerImpl
{
public:

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  inline static void
  implStackPointerSave(void) __attribute__( ( always_inline ) );
  inline static void
  implStackPointerRestore(void) __attribute__( ( always_inline ) );

  inline static void
  implRegistersSave(void) __attribute__( ( always_inline ) );
  inline static void
  implRegistersRestore(void) __attribute__( ( always_inline ) );

  inline static bool
  implIsAllowedToSwitch(void) __attribute__( ( always_inline ) );

#endif

};

//-----------------------------------------------------------------------------

class OSReadyList
{
public:
  OSReadyList();

  static void
  insert(OSTask * pTask);
  static void
  remove(OSTask * pTask);

  inline static OSTask *
  getTop(void);
  inline static unsigned char
  getCount(void);

#if defined(DEBUG)

  static void
  dump(void);
#endif

private:
  static int
  find(OSTask * pTask);

  // members
  static OSTask *ms_array[];
  static unsigned char ms_count;
};

//-----------------------------------------------------------------------------

inline bool
OSScheduler::isRunning(void)
{
  return ms_isRunning;
}

inline void
OSScheduler::lock(void)
{
  ms_isLocked = true;
}

inline void
OSScheduler::unlock(void)
{
  ms_isLocked = false;
}

inline bool
OSScheduler::isLocked(void)
{
  return ms_isLocked;
}

inline void
OSScheduler::setLock(bool flag)
{
  ms_isLocked = flag;
}

inline void
OSScheduler::setPreemptive(bool flag)
{
  ms_isPreemptive = flag;
}

inline bool
OSScheduler::isPreemptive(void)
{
  return ms_isPreemptive;
}

inline OSTask *
OSScheduler::getTaskCurrent(void)
{
  return ms_pTaskRunning;
}

inline OSTask *
OSScheduler::getTaskIdle(void)
{
  return ms_pTaskIdle;
}

inline void
OSScheduler::setTaskIdle(OSTask * pt)
{
  ms_pTaskIdle = pt;
}

inline int
OSScheduler::getTasksCount(void)
{
  return ms_tasksCount;
}

#if defined(OS_INCLUDE_OSTASK_SLEEP)

inline bool OSScheduler::isAllowDeepSleep(void)
  {
    return ms_allowDeepSleep;
  }

inline void OSScheduler::setAllowDeepSleep(bool flag)
  {
    ms_allowDeepSleep = flag;
  }

#endif

#if defined(OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT)

inline void OSScheduler::interruptEnter(void)
  {
#if !defined(OS_EXCLUDE_PREEMPTION)
    OSScheduler::contextSave(); // interrupts disabled in here
#endif
    OSScheduler::ledActiveOn();
  }

inline void OSScheduler::interruptExit(void)
  {
    if (OSScheduler::requireContextSwitch() )
      {
        OSScheduler::contextSwitch();
      }

#if !defined(OS_EXCLUDE_PREEMPTION)
    OSScheduler::contextRestore();
    OSImpl::returnFromInterrupt();
#endif
    // interrupts enabled after this point
  }

#endif

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

inline void
OSScheduler::interruptEnter(void)
  {
#if !defined(OS_EXCLUDE_PREEMPTION)
  OSSchedulerImpl::implRegistersSave();

    if (OSSchedulerImpl::implIsAllowedToSwitch())
      {
        OSSchedulerImpl::implStackPointerSave();
      }
#endif
    OSScheduler::ledActiveOn();
  }

inline void
OSScheduler::interruptExit(void)
  {
#if !defined(OS_EXCLUDE_PREEMPTION)

    if (OSSchedulerImpl::implIsAllowedToSwitch())
      {
        if (OSScheduler::requireContextSwitch())
          {
            OSScheduler::contextSwitch();
          }
        OSSchedulerImpl::implStackPointerRestore();
      }
    OSSchedulerImpl::implRegistersRestore();

    OSImpl::returnFromInterrupt();

#endif
    // interrupts re-enabled after this point
  }

#endif

//-----------------------------------------------------------------------------

inline OSTask *
OSReadyList::getTop(void)
{
  return ms_array[0];
}

inline unsigned char
OSReadyList::getCount(void)
{
  return ms_count;
}

//-----------------------------------------------------------------------------

#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/include/OSScheduler_Arch_Inlines.h"

#elif defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)
#include "hal/arch/arm_cortex_m3/kernel/include/OSScheduler_Arch_Inlines.h"

#if defined(OS_CONFIG_FAMILY_STM32F10X)
#include "hal/arch/arm_cortex_m3/stm32f10x/kernel/include/OSScheduler_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_AVR32)
#include "hal/arch/avr32/kernel/include/OSScheduler_Arch_Inlines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
#include "hal/arch/avr32/uc3/kernel/include/OSScheduler_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#elif defined(OS_CONFIG_ARCH_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/kernel/include/OSScheduler_Arch_Inlines.h"

#if defined(OS_CONFIG_FAMILY_TEMPLATE)
#include "hal/arch/TEMPLATE_ARCH/TEMPLATE_FAMILY/kernel/include/OSScheduler_Family_Inlines.h"
#else
#error "Missing OS_CONFIG_FAMILY_* definition"
#endif

#else
#error "Missing OS_CONFIG_ARCH_* definition"
#endif

#endif /*OSSCHEDULER_H_ */
