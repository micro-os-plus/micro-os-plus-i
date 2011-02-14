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

  // publicly available methods

  // early initialisations
  // must be called before all constructors
  static void
  earlyInit(void);

  // start the scheduler.
  // the scheduler cannot be stopped if it was started.
  static void
  start(void) __attribute__( ( noreturn ) );

  // return true if the the scheduler was started
  static bool
  isRunning(void);

  // disable preemption.
  // yield() will immediately return to the same task,
  // and eventWait() will immediately return with OS_EVENT_WAIT_RETURN_LOCKED.
  static void
  lock(void);

  // enable preemption
  static void
  unlock(void);

  // check if the scheduler was locked
  static bool
  isLocked(void);

  // Lock the scheduler if the flag is true, unlock otherwise.
  static void
  setLock(bool flag);

  // Return true if the scheduler runs in preemptive mode.
  inline static bool
  isPreemptive(void);

  // Set the scheduler to run in cooperative mode if flag is false,
  // set to preemptive mode if flag is true (default).
  static void
  setPreemptive(bool flag);

  // Return a pointer to the task currently running on the CPU.
  static OSTask *
  getTaskCurrent(void);

  // returns the idle task
  static OSTask *
  getTaskIdle(void);

  // Return the number of tasks registered to the scheduler.
  static int
  getTasksCount(void);

  // Return the i-th task registered to the scheduler,
  // or OS_TASK_NONE for illegal index.
  static OSTask *
  getTask(int i);

  static bool
  isAllowDeepSleep(void);
  static void
  setAllowDeepSleep(bool flag);

  // enter a critical section, i.e. a section where interrupts are disabled.
  inline static void
  criticalEnter(void) __attribute__( ( always_inline ) );
  // exit from critical section.
  inline static void
  criticalExit(void) __attribute__( ( always_inline ) );

  // sleep until event occurs
  static OSEventWaitReturn_t
  eventWait(OSEvent_t event);

  // wakeup all tasks waiting for event
  static int
  eventNotify(OSEvent_t event, OSEventWaitReturn_t ret =
      OSEventWaitReturn::OS_VOID);

  // Suspend the current task and reschedule the CPU to the next task.
  static void
  yield(void);

  inline static void
  ledActiveInit(void) __attribute__( ( always_inline ) );
  inline static void
  ISR_ledActiveOn(void) __attribute__( ( always_inline ) );
  inline static void
  ledActiveOn(void) __attribute__( ( always_inline ) );
  inline static void
  ISR_ledActiveOff(void) __attribute__( ( always_inline ) );
  inline static void
  ledActiveOff(void) __attribute__( ( always_inline ) );

  // register a task to the scheduler
  static unsigned char
  taskRegister(OSTask * pTask);

  // TODO: do we need this?
  static void
  interruptTick(void);

  // quick check if a context switch is necessary
  static bool
  isContextSwitchRequired(void);

  // make the context switch to the first task
  // from the ReadyList
  static void
  performContextSwitch(void);

#if defined(OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT) || defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // called in order to save the context of the current task
  // must be called in conjunction with interruptExit
  inline static void
  interruptEnter(void) __attribute__( ( always_inline ) );
  // called in order to perform the context switch
  // must be called in conjunction with interruptEnter
  inline static void
  interruptExit(void) __attribute__( ( always_inline ) );

#endif

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSAVE_RESTORE)

  inline static void
  contextSave(void) __attribute__( ( always_inline ) );
  inline static void
  contextRestore(void) __attribute__( ( always_inline ) );

#endif

  // timer used by scheduler to chedule the next task
  static OSTimerTicks timerTicks;

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
  static OSTimerSeconds timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  static void ISRcancelTask(OSTask *pTask);
#endif

  // pointer to the active task (running task)
  static OSTask *ms_pTaskRunning;

  friend class OSTimerTicks;

private:

  // TODO: remove if no longer needed
//  static void
//  timerSetup(void);

  // set the idle task
  static void
  setTaskIdle(OSTask *);

  // TODO: remove if no longer needed
  //static void timerISR(void) __attribute__( ( naked ) );
//  static void
//  timerISR(void);

  // members

  // true if the scheduler is preemptive
  static bool ms_isPreemptive;

  // true if the scheduler was started
  static bool ms_isRunning;
  // true if the scheduler is locked, i.e. preemption is disabled
  static bool ms_isLocked;

  // list of tasks registered to the scheduler
  static OSTask *ms_tasks[OS_CFGINT_TASKS_TABLE_SIZE + 1];
  // the number of tasks registered to the scheduler
  static unsigned char ms_tasksCount;
  //static unsigned char tasksIdx;

  // the Idle task
  static OSTask *ms_pTaskIdle;

  // the ready list used for managing the runnable tasks
  static OSReadyList ms_readyTasksList;

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  static bool ms_allowDeepSleep;
#endif

};

// ----------------------------------------------------------------------------

class OSSchedulerImpl
{
public:

  // starts the first task from the ready list
  static void
  start(void) __attribute__( ( noreturn ) );

  // yield function
  inline static void
  yield(void) __attribute__( ( always_inline ) );

  // used to initialize the stack for a task
  static OSStack_t *
  stackInitialize(OSStack_t * pStackTop, void
  (*entryPoint)(void *), void *pParams, unsigned char id);

  // TODO: remove if no longer needed
//  static void
//  stackSetReturnedValue(OSStack_t * pStack, OSEventWaitReturn_t ret);

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSWITCHREQUEST)
  static void
  ISRcontextSwitchRequest(void);
#endif

  // TODO: remove if no longer needed
  //static void dumpContextInfo(void *pContext);

  // dump context info to the device debug
  static void
  dumpContextInfo(OSTask *);

  // restore the context for the next task to be scheduled
  inline static void
  FirstTask_contextRestore(void) __attribute__( ( always_inline ) );

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // save the SP for the active task
  inline static void
  stackPointerSave(void) __attribute__( ( always_inline ) );

  // restore the SP for the active task
  inline static void
  stackPointerRestore(void) __attribute__( ( always_inline ) );

  // save the registers for the active task
  inline static void
  registersSave(void) __attribute__( ( always_inline ) );

  // restore the registers for the active task
  inline static void
  registersRestore(void) __attribute__( ( always_inline ) );

  // check if the context switch can be done. The context
  // switch cannot be done if the current context is a nested interrupt.
  // In order to check this, the SR register saved on stack is verified
  // (to see what the previous context was).
  // See the OSScheduler::interruptEnter or OSScheduler::interruptExit
  // in order to see how this is used.
  inline static bool
  isContextSwitchAllowed(void) __attribute__( ( always_inline ) );

#endif

};

//-----------------------------------------------------------------------------

class OSReadyList
{
public:
  OSReadyList();

  // insert task in ready list
  static void
  insert(OSTask * pTask);
  // delete task from ready list
  static void
  remove(OSTask * pTask);

  // returns the first task from ready list (task with higher priority)
  inline static OSTask *
  getTop(void);
  // returns the number of the tasks from ready list
  inline static unsigned char
  getCount(void);

#if defined(DEBUG)

  static void
  dump(void);

#endif

private:
  // find if pTask is in the ready tasks array
  static int
  find(OSTask * pTask);

  // members

  // array of ready tasks
  static OSTask *ms_array[];
  // number of ready tasks (ready to run) in ms_array
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
#if !defined(OS_EXCLUDE_PREEMPTION)
  ms_isPreemptive = flag;
#else
  flag = flag;
#endif
}

inline bool
OSScheduler::isPreemptive(void)
{
#if !defined(OS_EXCLUDE_PREEMPTION)
  return ms_isPreemptive;
#else
  return false;
#endif
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

#endif /* OS_INCLUDE_OSTASK_SLEEP */

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
    if (OSScheduler::isContextSwitchRequired() )
      {
        OSScheduler::performContextSwitch();
      }

#if !defined(OS_EXCLUDE_PREEMPTION)
    OSScheduler::contextRestore();
    OSImpl::returnFromInterrupt();
#endif
    // interrupts enabled after this point
  }

#endif /* OS_INCLUDE_OSSCHEDULER_INTERRUPTENTER_EXIT */

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

inline void
OSScheduler::interruptEnter(void)
{

  OSSchedulerImpl::registersSave();
  OSScheduler::ISR_ledActiveOn();

#if !defined(OS_EXCLUDE_PREEMPTION)

  if (OSSchedulerImpl::isContextSwitchAllowed())
    {
      OSSchedulerImpl::stackPointerSave();
    }

#endif
}

inline void
OSScheduler::interruptExit(void)
{
#if !defined(OS_EXCLUDE_PREEMPTION)

  if (OSSchedulerImpl::isContextSwitchAllowed())
    {
      if (OSScheduler::isContextSwitchRequired())
        {
          OSScheduler::performContextSwitch();
        }
      OSSchedulerImpl::stackPointerRestore();
    }

#endif

  OSSchedulerImpl::registersRestore();
  OSImpl::returnFromInterrupt();

  // interrupts re-enabled after this point
}

#endif /* OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING */

#if !defined(OS_EXCLUDE_OSSCHEDULER_LED_ACTIVE)
/*
 * Manage the activity LED
 */
inline void OSScheduler::ledActiveInit(void)
  {
#if defined(OS_CONFIG_ACTIVE_LED_PORT) && defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
        OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
        // Turn off led, i.e. HIGH if active low
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
        OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
        OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
        OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
#else
#error "OS_CONFIG_ACTIVE_LED_* missing"
#endif
  }

/* Turn LED on (on interrupts) */
inline void OSScheduler::ISR_ledActiveOn(void)
  {
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
    OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
    OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
   }

// TODO: if GPIO actions are atomic, we need no critical sections
inline void OSScheduler::ledActiveOn(void)
  {
    OSScheduler::criticalEnter();
      {
        ISR_ledActiveOn();
      }
    OSScheduler::criticalExit();
  }

/* Turn LED off (on interrupts) */
inline void OSScheduler::ISR_ledActiveOff(void)
  {
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
    OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
    OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
  }

/* Turn LED off (at sleep) */
inline void OSScheduler::ledActiveOff(void)
  {
    OSScheduler::criticalEnter();
      {
        ISR_ledActiveOff();
      }
    OSScheduler::criticalExit();
    }

#endif /* ! OS_EXCLUDE_OSSCHEDULER_LED_ACTIVE */

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
