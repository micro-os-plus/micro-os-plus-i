/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSSCHEDULER_H_
#define OSSCHEDULER_H_

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSThread.h"

typedef uint16_t schedTicks_t;

class OSActiveThreads;

#if defined(OS_INCLUDE_OSTIMERSECONDS)
class OSTimerSeconds;
#endif /* defined(OS_INCLUDE_OSTIMERSECONDS) */

class OSTimerTicks;
class OSThread;

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
  start(void) __attribute__((noreturn));

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

  // Return a pointer to the thread currently running on the CPU.
  static OSThread*
  getThreadCurrent(void);

  // returns the idle thread
  static OSThread*
  getThreadIdle(void);

  // Return the number of threads registered to the scheduler.
  static int
  getThreadsCount(void);

  // Return the i-th thread registered to the scheduler,
  // or NULL for illegal index or missing thread.
  static OSThread*
  getThread(int i);

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
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

  // wakeup all threads waiting for event
  static int
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t ret = OSEventWaitReturn::OS_VOID);

  // Suspend the current thread and reschedule the CPU to the next thread.
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

  // register a thread to the scheduler
  static unsigned char
  threadRegister(OSThread* pThread);

  // TODO: do we need this?
  static void
  interruptTick(void);

  // quick check if a context switch is necessary
  static bool
  isContextSwitchRequired(void);

  // make the context switch to the first thread
  // from the ReadyList
  static void
  performContextSwitch(void);

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // called in order to save the context of the current thread
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

  // timer used by scheduler to schedule the next thread
  static OSTimerTicks timerTicks;

#if defined(OS_INCLUDE_OSTIMERSECONDS)
  // timer expressed in seconds
  static OSTimerSeconds timerSeconds;
#endif

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)
  // Notifies a thread which is waiting for time events
  // (something like kill command in linux - SIGKILL)
  static void ISRcancelThread(OSThread* pThread);
#endif

  // pointer to the active thread (running thread)
  static OSThread* volatile ms_pThreadRunning;

  static volatile OSStack_t** volatile ms_ppCurrentStack;

  friend class OSTimerTicks;

private:

  // TODO: remove if no longer needed
  //  static void
  //  timerSetup(void);

  // set the idle thread
  static void
  setThreadIdle(OSThread*);

  // TODO: remove if no longer needed
  //static void timerISR(void) __attribute__((naked));
  //  static void
  //  timerISR(void);

  // members

  // true if the scheduler is preemptive
  static bool ms_isPreemptive;

  // true if the scheduler was started
  static bool ms_isRunning;
  // true if the scheduler is locked, i.e. yield will return to same thread
  //static bool ms_isLocked;

  // list of threads registered to the scheduler
  static OSThread* ms_threads[OS_CFGINT_THREADS_ARRAY_SIZE + 1];
  // the number of threads registered to the scheduler
  static unsigned char ms_threadsCount;
  //static unsigned char threadsIdx;

  #if defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY)
  static unsigned char ms_notifyIndex;
#endif /* defined(OS_INCLUDE_OSSCHEDULER_ROUND_ROBIN_NOTIFY) */

  // the Idle thread
  static OSThread* ms_pThreadIdle;

  // the ready list used for managing the runnable threads
  static OSActiveThreads ms_activeThreads;

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
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

  // starts the first thread from the ready list
  static void
  start(void) __attribute__((noreturn));

  // yield function
  inline static void
  yield(void) __attribute__((always_inline));

  // initialise the stack for a thread
  static OSStack_t*
  stackInitialise(OSStack_t* pStackTop, void
  (*entryPoint)(void*), void* pParams, unsigned char id, void* pNesting);

#if defined(OS_INCLUDE_OSSCHEDULER_CONTEXTSWITCHREQUEST)
  static void
  ISRcontextSwitchRequest(void);
#endif

  // TODO: remove if no longer needed
  //static void dumpContextInfo(void* pContext);

  // dump context info to the device debug
  static void
  dumpContextInfo(OSThread*);

  // restore the context for the next thread to be scheduled
  inline static void
  FirstThread_contextRestore(void) __attribute__((always_inline));

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

  // save the SP for the active thread
  inline static void
  stackPointerSave(void) __attribute__((always_inline));

  // restore the SP for the active thread
  inline static void
  stackPointerRestore(void) __attribute__((always_inline));

  // save the registers for the active thread
  inline static void
  registersSave(void) __attribute__((always_inline));

  // restore the registers for the active thread
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

class OSActiveThreads
{
public:
  OSActiveThreads();

  // insert thread in ready list
  static void
  insert(OSThread* pThread);
  // delete thread from ready list
  static void
  remove(OSThread* pThread);

  // returns the first thread from ready list (thread with higher priority)
  inline static OSThread*
  getTop(void);
  // returns the number of the threads from ready list
  inline static unsigned char
  getCount(void);

#if defined(DEBUG)

  static void
  dump(void);

#endif

private:
  // find if pThread is in the ready threads array
  static int
  find(OSThread* pThread);

  // members

  // array of ready threads
  static OSThread* ms_array[];
  // number of ready threads (ready to run) in ms_array
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
#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) && false
  ms_isLocked = OSCPUImpl::stackPop();
#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */
  --ms_nestingLevel;
#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */
}

// ============================================================================

inline OSThread* 
OSActiveThreads::getTop(void)
{
  return ms_array[0];
}

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

inline OSThread* 
OSScheduler::getThreadCurrent(void)
{
  return ms_pThreadRunning;
}

inline OSThread* 
OSScheduler::getThreadIdle(void)
{
  return ms_pThreadIdle;
}

inline void
OSScheduler::setThreadIdle(OSThread* pt)
{
  ms_pThreadIdle = pt;
}

inline int
OSScheduler::getThreadsCount(void)
{
  return ms_threadsCount;
}

// Runs in a critical section
inline bool
OSScheduler::eventWaitPrepare(OSEvent_t event)
{
  return ms_pThreadRunning->eventWaitPrepare(event);
}

inline OSEventWaitReturn_t
OSScheduler::eventWaitPerform(void)
{
  return ms_pThreadRunning->eventWaitPerform();
}

inline void
OSScheduler::eventWaitClear(void)
{
  ms_pThreadRunning->eventWaitClear();
}

inline OSEventWaitReturn_t
OSScheduler::getEventWaitReturn(void)
{
  return ms_pThreadRunning->getEventWaitReturn();
}

inline void
OSScheduler::setEventWaitReturn(OSEventWaitReturn_t ret)
{
  ms_pThreadRunning->setEventWaitReturn(ret);
}

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)

inline bool OSScheduler::isAllowDeepSleep(void)
  {
    return ms_allowDeepSleep;
  }

inline void OSScheduler::setAllowDeepSleep(bool flag)
  {
    ms_allowDeepSleep = flag;
  }

#endif /* OS_INCLUDE_OSTHREAD_SLEEP */

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
#if defined(OS_CONFIG_ACTIVE_LED_PORT_WRITE) && defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
  // Turn off led, i.e. HIGH if active low
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
#else
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
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
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
#else
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
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
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
#else
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
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
OSActiveThreads::getCount(void)
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
