/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTASK_H_
#define OSTASK_H_

#include "portable/kernel/include/OS.h"

class OSScheduler;

typedef unsigned char OSTaskPriority_t;

typedef void
(*OSTaskMainPtr_t)(void *);

class OSTask
{
public:

  // Task range priorities.
  static const OSTaskPriority_t IDLE_PRIORITY = 0x00;
  static const OSTaskPriority_t MIN_PRIORITY = 0x01;
  static const OSTaskPriority_t MAX_PRIORITY = 0xFF;
  static const OSTaskPriority_t DEFAULT_PRIORITY = (MAX_PRIORITY / 2 + 1);

  // Value for the minimal size for a task.
  static const unsigned short STACK_MINIMAL_SIZE = OS_CFGINT_STACK_MINIMAL_SIZE;
  // Pattern for initialising the stack.
  static const OSStack_t STACK_FILL_BYTE = 0x5A;

  // Constructor; run static taskMain (C++ style).
  OSTask(const char *pName, const OSStack_t *pStack, unsigned short stackSize,
      OSTaskPriority_t priority = OSTask::DEFAULT_PRIORITY);

  // Constructor; run given entryPoint function (C style).
  OSTask(const char *pName, OSTaskMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStack, unsigned short stackSize,
      OSTaskPriority_t priority = OSTask::DEFAULT_PRIORITY);

#if false
  virtual ~OSTask();
#endif

  // Main function task.
  // Overridden by actual implementation.
  virtual void
  taskMain(void);

  // Suspend the task and remove it from the ready list.
  void
  suspend(void);
  // Resume the task, previously suspended by inserting it into the ready list.
  void
  resume(void);
  // Check if the task is suspended.
  bool
  isSuspended(void);
  // Check if the task is waiting on an event.
  bool
  isWaiting(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Return TRUE if the task can go to sleep, FALSE otherwise.
  bool isDeepSleepAllowed();
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  // Set the virtual watchdog expire interval (in seconds).
  void virtualWatchdogSet(unsigned short seconds);
  // If the virtual watchdog interval expires, the MCU is reset using
  // the hardware module watchdog.
  void virtualWatchdogCheck(void);
#endif

  // Return the task name.
  char const *
  getName(void);

  // Return the address of the stack bottom.
  // Stack grows from high address to low address,
  // so this is the maximum address the stack can grow.
  unsigned char *
  getStackBottom(void);

  // Return the current stack pointer of the task.
  // This value is stored only during context switch,
  // so the running task will not get the actual value.
  OSStack_t *
  getStack(void);

  // Return the stack size given at task creation.
  unsigned short
  getStackSize(void); /* bytes */

#if defined(OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER)
  OSProgramPtr_t getProgramCounter(void);
#endif

  // Return the task ID.
  int
  getID(void);

  // Return the task's context.
  void *
  getContext(void);

  // Return task priority.
  // Higher means better priority.
  OSTaskPriority_t
  getPriority(void);

  // Set task priority.
  void
  setPriority(OSTaskPriority_t priority);

  // Return the event the task is waiting for.
  // Cancelling a waiting task can be done by notifying
  // this event with a return value of OS_EVENT_WAIT_RETURN_CANCELED.
  OSEvent_t
  getEvent(void);

  // Set the event the task is waiting for.
  void
  setEvent(OSEvent_t event);

  // Return the maximum usage of the stack, in bytes.
  unsigned short
  getStackUsed(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Allow task to be put to sleep.
  void setAllowSleep(bool status);
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  bool isInterrupted(void);
  void setInterruption(bool flag);
  void requestInterruption(void);
  void ackInterruption(void);
#endif

#if defined(DEBUG)
  void
  dumpPC(void);
#endif

  // The SP is saved in this variable at contextSave 
  // and restored from here at contextRestore.
  OSStack_t *m_pStack;

  // Prepare the task to enter the wait state
  bool
  eventWaitPrepare(OSEvent_t event);

  // Perform the actual wait
  OSEventWaitReturn_t
  eventWaitPerform(void);

  // Chain the prepare and perform actions
  OSEventWaitReturn_t
  eventWait(OSEvent_t event);

  // Wake up this task if it waits for the given event.
  int
  eventNotify(OSEvent_t event, OSEventWaitReturn_t ret =
      OSEventWaitReturn::OS_VOID);

  // Retrieve the value returned by the last eventWait()
  OSEventWaitReturn_t
  getEventWaitReturn(void);

  // Set the value to be returned by getEventWaitReturn()
  void
  setEventWaitReturn(OSEventWaitReturn_t ret);

private:
  friend class OSScheduler; // TODO: explain why they are here
  friend class OSActiveTasks;

  // Initialise task's environment.
  void
  init(const char *pName, OSTaskMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStackBottom, unsigned short stackSize,
      OSTaskPriority_t priority);

  // Redirect to virtual function (taskMain).
  static void
  staticMain(OSTask * pt);

  static void
  yield(void);

#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK)
  // Warning: no longer run in critical section!
  virtual void schedulerTick( void );
#endif

  // members
#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  // True if the task is interrupted.
  bool m_isInterrupted;
#endif

  // True if the task is suspended.
  bool m_isSuspended;

  // True if task is waiting for an event. The event value is in m_event.
  bool m_isWaiting;

  // The event which the task is waiting for, or OSEvent::OS_NONE
  OSEvent_t m_event;

  // The value to be returned by eventWait(), or OSEventReturn::OS_NONE
  OSEventWaitReturn_t m_eventWaitReturn;

  // Task's priority.
  OSTaskPriority_t m_staticPriority;

  // Task's numerical ID, the index in the scheduler array.
  unsigned char m_id;

  // Task's name.
  const char *m_pName;

  // Task's entry point (i.e. the function executed by this task).
  OSTaskMainPtr_t m_entryPoint;

  // Data passed to this task.
  void *m_pParameters;

  // The bottom of the task's stack (lowest address).
  unsigned char *m_pStackBottom;

  // The size of the task's stack, in bytes.
  unsigned short m_stackSize;

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // True if the task can be put to sleep.
  bool m_allowSleep;
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  unsigned short m_WDseconds;
#endif
};

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)

inline void OSTask::ackInterruption(void)
  {
    setInterruption(false);
  }

#endif

inline OSEventWaitReturn_t
OSTask::eventWait(OSEvent_t event)
{
  if (eventWaitPrepare(event))
    eventWaitPerform();
  return m_eventWaitReturn;
}

inline OSEventWaitReturn_t
OSTask::eventWaitPerform(void)
{
  yield();
  return m_eventWaitReturn;
}

inline OSEventWaitReturn_t
OSTask::getEventWaitReturn(void)
{
  return m_eventWaitReturn;
}

inline void
OSTask::setEventWaitReturn(OSEventWaitReturn_t ret)
{
  m_eventWaitReturn = ret;
}

#endif /* OSTASK_H_ */
