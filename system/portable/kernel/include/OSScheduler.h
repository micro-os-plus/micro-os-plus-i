/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSSCHEDULER_H_
#define OSSCHEDULER_H_

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSTask.h"

typedef unsigned short schedTicks_t;

class OSActiveTasks;

#if defined(OS_INCLUDE_OSTIMERSECONDS)
class OSTimerSeconds;
#endif /* defined(OS_INCLUDE_OSTIMERSECONDS) */

class OSTimerTicks;
class OSTask;

// ============================================================================

class OSSchedulerLock
{
public:
#if defined(DEBUG)
  OSSchedulerLock();
#endif

  // Pair of functions, using a stack to save/restore value
  static void
  enter(void) __attribute__((always_inline));
  static void
  exit(void) __attribute__((always_inline));

  // Lock the scheduler if the flag is true, clear otherwise.
  static bool
  set(bool flag = true);

  static bool
  clear(void);

  // Check if the scheduler was locked
  static bool
  isSet(void) __attribute__((always_inline));

private:
  static unsigned char volatile ms_nestingLevel;

  char m_dummy;
};

// ============================================================================

class OSCriticalSection
{
public:
#if defined(DEBUG)
  OSCriticalSection();
#endif

  // Pair of functions to enable disable interrupts

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

  // Inlines using a stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

  // Regular functions using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

  static OSStack_t volatile ms_nestingLevel;

private:
  char m_dummy;
};

// ============================================================================

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

  // Return true if the scheduler runs in preemptive mode.
  inline static bool
  isPreemptive(void);

  // Set the scheduler to run in cooperative mode if flag is false,
  // set to preemptive mode if flag is true (default).
  static void
  setPreemptive(bool flag);

  // Return a pointer to the task currently running on the CPU.
  static OSTask*
  getTaskCurrent(void);

  // returns the idle task
  static OSTask*
  getTaskIdle(void);

  // Return the number of tasks registered to the scheduler.
  static int
  getTasksCount(void);

  // Return the i-th task registered to the scheduler,
  // or NULL for illegal index or missing task.
  static OSTask*
  getTask(int i);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Return true if the deep sleep flag was set to true
  static bool
  isAllowDeepSleep(void);
  // Set the value of the deep sleep flag
  static void
  setAllowDeepSleep(bool flag);
#endif

  // sleep until event occurs
  static OSEventWaitReturn_t
  eventWait(OSEvent_t event);

  static bool
  eventWaitPrepare(OSEvent_t event) __attribute__((always_inline));

  // Perform the actual wait
  static OSEventWaitReturn_t
  eventWaitPerform(void) __attribute__((always_inline));

  static OSEventWaitReturn_t
  getEventWaitReturn(void) __attribute__((always_inline));

  static void
  setEventWaitReturn(OSEventWaitReturn_t ret) __attribute__((always_inline));

  static void
  eventWaitClear(void);

  // wakeup all tasks waiting for event
  static int
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t ret = OSEventWaitReturn::OS_VOID);

  // Suspend the current task and reschedule the CPU to the next task.
  static void
  yield(void);

  // TODO: do we need this?
  inline static void
  ledActiveInit(void) __attribute__((always_inline));
  inline static void
  ISR_ledActiveOn(void) __attribute__((always_inline));
  inline static void
  ledActiveOn(void) __attribute__((always_inline));
  inline static void
  ISR_ledActiveOff(void) __attribute__((always_inline));
  inline static void
  ledActiveOff(void) __attribute__((always_inline));

  // register a task to the scheduler
  static unsigned char
  taskRegister(OSTask* pTask);

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

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // called in order to save the context of the current task
  // must be called in conjunction with interruptExit
  inline static void
  interruptEnter(void) __attribute__((always_inline));
  // called in order to perform the context switch
  // must be called in conjunction with interruptEnter
  inline static void
  interruptExit(void) __attribute__((always_inline));

#endif

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSAVE_RESTORE)

  inline static void
  contextSave(void) __attribute__((always_inline));
  inline static void
  contextRestore(void) __attribute__((always_inline));

#endif

  // flag to store lock/unlock
  static OSSchedulerLock lock;

  // critical section support
  static OSCriticalSection critical;

  // timer used by scheduler to schedule the next task
  static OSTimerTicks timerTicks;

#if defined(OS_INCLUDE_OSTIMERSECONDS)
  // timer expressed in seconds
  static OSTimerSeconds timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  // Notifies a task which is waiting for time events
  // (something like kill command in linux - SIGKILL)
  static void ISRcancelTask(OSTask *pTask);
#endif

  // pointer to the active task (running task)
  static OSTask* volatile ms_pTaskRunning;

  static volatile OSStack_t** volatile ms_ppCurrentStack;

  friend class OSTimerTicks;

private:

  // TODO: remove if no longer needed
  //  static void
  //  timerSetup(void);

  // set the idle task
  static void
  setTaskIdle(OSTask*);

  // TODO: remove if no longer needed
  //static void timerISR(void) __attribute__( ( naked ) );
  //  static void
  //  timerISR(void);

  // members

  // true if the scheduler is preemptive
  static bool ms_isPreemptive;

  // true if the scheduler was started
  static bool ms_isRunning;
  // true if the scheduler is locked, i.e. yield will return to same task
  //static bool ms_isLocked;

  // list of tasks registered to the scheduler
  static OSTask* ms_tasks[OS_CFGINT_TASKS_TABLE_SIZE + 1];
  // the number of tasks registered to the scheduler
  static unsigned char ms_tasksCount;
  //static unsigned char tasksIdx;

#if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  static unsigned char ms_notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  // the Idle task
  static OSTask* ms_pTaskIdle;

  // the ready list used for managing the runnable tasks
  static OSActiveTasks ms_activeTasks;

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // records the deep sleep flag
  static bool ms_allowDeepSleep;
#endif

  // Added to avoid the ABI warning "contains empty classes..."
  char m_dummy;
};

// ============================================================================

class OSSchedulerImpl
{
public:

  // starts the first task from the ready list
  static void
  start(void) __attribute__((noreturn));

  // yield function
  inline static void
  yield(void) __attribute__((always_inline));

  // initialise the stack for a task
  static OSStack_t*
  stackInitialise(OSStack_t* pStackTop, void
  (*entryPoint)(void*), void* pParams, unsigned char id);

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSWITCHREQUEST)
  static void
  ISRcontextSwitchRequest(void);
#endif

  // TODO: remove if no longer needed
  //static void dumpContextInfo(void *pContext);

  // dump context info to the device debug
  static void
  dumpContextInfo(OSTask*);

  // restore the context for the next task to be scheduled
  inline static void
  FirstTask_contextRestore(void) __attribute__((always_inline));

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // save the SP for the active task
  inline static void
  stackPointerSave(void) __attribute__((always_inline));

  // restore the SP for the active task
  inline static void
  stackPointerRestore(void) __attribute__((always_inline));

  // save the registers for the active task
  inline static void
  registersSave(void) __attribute__((always_inline));

  // restore the registers for the active task
  inline static void
  registersRestore(void) __attribute__((always_inline));

  // save the critical section nesting counter
  inline static void
  criticalSectionNestingSave(void) __attribute__((always_inline));

  // restore the critical section nesting counter
  inline static void
  criticalSectionNestingRestore(void) __attribute__((always_inline));

  // check if the context switch can be done. The context
  // switch cannot be done if the current context is a nested interrupt.
  // In order to check this, the SR register saved on stack is verified
  // (to see what the previous context was).
  // See the OSScheduler::interruptEnter or OSScheduler::interruptExit
  // in order to see how this is used.
  inline static bool
  isContextSwitchAllowed(void) __attribute__((always_inline));

#endif

};

// ============================================================================

class OSActiveTasks
{
public:
  OSActiveTasks();

  // insert task in ready list
  static void
  insert(OSTask* pTask);
  // delete task from ready list
  static void
  remove(OSTask* pTask);

  // returns the first task from ready list (task with higher priority)
  inline static OSTask*
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
  find(OSTask* pTask);

  // members

  // array of ready tasks
  static OSTask* ms_array[];
  // number of ready tasks (ready to run) in ms_array
  static unsigned char ms_count;
};

//-----------------------------------------------------------------------------

inline bool
OSSchedulerLock::set(bool flag)
{
  bool b;
  b = isSet();
  ms_nestingLevel = (flag ? 1 : 0);

#if defined(DEBUG) && false
  OSDeviceDebug::putString("lock set(");
  OSDeviceDebug::putString(flag? "T": "F");
  OSDeviceDebug::putString(") ");
#endif

  return b;
}

inline bool
OSSchedulerLock::clear(void)
{
  bool b;
  b = isSet();
  ms_nestingLevel = 0;

#if defined(DEBUG) && false
  OSDeviceDebug::putString("lock clear() ");
#endif
  return b;
}

inline bool
OSSchedulerLock::isSet(void)
{
  return (ms_nestingLevel != 0);
}

inline void
OSSchedulerLock::enter(void)
{
  ++ms_nestingLevel;
}

inline void
OSSchedulerLock::exit(void)
{
#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) && false
  ms_isLocked = OSCPUImpl::stackPop();
#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */
  --ms_nestingLevel;
#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */
}

// ============================================================================

inline OSTask *
OSActiveTasks::getTop(void)
{
  return ms_array[0];
}

// ============================================================================

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

// When we can use the stack, the enter()/exit() calls are inlined

inline void
OSCriticalSection::enter(void)
{
  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();
  OSCPUImpl::stackPush(tmp);

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
  tmp |= (OS_CFGINT_OSCRITICALSECTION_MASK);
  OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
}

inline void
OSCriticalSection::exit(void)
{
  register OSStack_t tmp;

  tmp = OSCPUImpl::stackPop();
  OSCPUImpl::setInterruptsMask(tmp);
}

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

// ============================================================================

inline bool
OSScheduler::isRunning(void)
{
  return ms_isRunning;
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

// Runs in a critical section
inline bool
OSScheduler::eventWaitPrepare(OSEvent_t event)
{
  return ms_pTaskRunning->eventWaitPrepare(event);
}

inline OSEventWaitReturn_t
OSScheduler::eventWaitPerform(void)
{
  return ms_pTaskRunning->eventWaitPerform();
}

inline void
OSScheduler::eventWaitClear(void)
{
  ms_pTaskRunning->eventWaitClear();
}

inline OSEventWaitReturn_t
OSScheduler::getEventWaitReturn(void)
{
  return ms_pTaskRunning->getEventWaitReturn();
}

inline void
OSScheduler::setEventWaitReturn(OSEventWaitReturn_t ret)
{
  ms_pTaskRunning->setEventWaitReturn(ret);
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

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSAVE_RESTORE)

inline void
OSScheduler::contextSave(void)
  {
    OSSchedulerImpl::registersSave();
    OSSchedulerImpl::stackPointerSave();
  }

inline void
OSScheduler::contextRestore(void)
  {
    OSSchedulerImpl::stackPointerRestore();
    OSSchedulerImpl::registersRestore();
  }

#endif

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

inline void
OSScheduler::interruptEnter(void)
{
  OSSchedulerImpl::registersSave();
  OSScheduler::ISR_ledActiveOn();
  OSSchedulerImpl::criticalSectionNestingSave();

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

  OSSchedulerImpl::criticalSectionNestingRestore();
  OSSchedulerImpl::registersRestore();
  OSCPUImpl::returnFromInterrupt();

  // interrupts re-enabled after this point
}

#endif /* OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING */

#if !defined(OS_EXCLUDE_OSSCHEDULER_LED_ACTIVE)
/*
 * Manage the activity LED
 */
inline void
OSScheduler::ledActiveInit(void)
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
inline void
OSScheduler::ISR_ledActiveOn(void)
{
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
}

// TODO: if GPIO actions are atomic, we need no critical sections
inline void
OSScheduler::ledActiveOn(void)
{
  OSCriticalSection::enter();
    {
      ISR_ledActiveOn();
    }
  OSCriticalSection::exit();
}

/* Turn LED off (on interrupts) */
inline void
OSScheduler::ISR_ledActiveOff(void)
{
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
}

/* Turn LED off (at sleep) */
inline void
OSScheduler::ledActiveOff(void)
{
  OSCriticalSection::enter();
    {
      ISR_ledActiveOff();
    }
  OSCriticalSection::exit();
}

#endif /* ! OS_EXCLUDE_OSSCHEDULER_LED_ACTIVE */

// ============================================================================

inline unsigned char
OSActiveTasks::getCount(void)
{
  return ms_count;
}

// ============================================================================

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

// ----------------------------------------------------------------------------

#endif /* OSSCHEDULER_H_ */
