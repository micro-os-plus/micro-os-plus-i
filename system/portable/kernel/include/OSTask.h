/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTASK_H_
#define OSTASK_H_

#include "portable/kernel/include/OS.h"

typedef unsigned char OSTaskPriority_t;

typedef void (*OSTaskMainPtr_t)(void *);

class OSTask
  {
public:

  // Task range priorities.
  static const OSTaskPriority_t IDLE_PRIORITY = 0x00;
  static const OSTaskPriority_t MIN_PRIORITY = 0x01;
  static const OSTaskPriority_t MAX_PRIORITY = 0xFF;
  static const OSTaskPriority_t DEFAULT_PRIORITY = (MAX_PRIORITY/2+1);

  // TODO: remove it if no longer used
//  Static const int NONE = -1;

  // Value for the minimal size for a task.
  static const unsigned short STACK_MINIMAL_SIZE = OS_CFGINT_STACK_MINIMAL_SIZE;
  // Pattern for initializing the stack.
  static const OSStack_t STACK_FILL_BYTE = 0x5A;


  //#define OS_TASK_NONE                    ((OSTask *)-1)

  // Constructors.
  OSTask(const char *pName, const OSStack_t *pStack,
      unsigned short stackSize, OSTaskPriority_t priority =
      OSTask::DEFAULT_PRIORITY);

  OSTask(const char *pName, OSTaskMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStack, unsigned short stackSize,
      OSTaskPriority_t priority =
      OSTask::DEFAULT_PRIORITY);

#if false
  virtual ~OSTask();
#endif

  // Main function task,
  // overriden by actual implementation.
  virtual void taskMain(void);

  // Suspend the task and remove it from the ready list.
  void suspend(void);
  // Resume a task, previously suspended and insert into the ready list
  void resume(void);
  // Check if a task is suspended.
  bool isSuspended(void);
  // Check if a task is waiting on a specific event.
  bool isWaiting(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Return TRUE if the task can go to sleep,
  // FALSE otherwise.
  bool isAllowDeepSleep();
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  // TODO: if these function are no longer used, should be removed.
  void virtualWatchdogSet(unsigned short seconds);
  void virtualWatchdogCheck(void);
#endif

  // Return the task name.
  char const *getName(void);
  // Return the address of the stack bottom.
  // Stack grows from high address to low address,
  // so this is the maximum address the stack can grow.
  unsigned char *getStackBottom(void);
  // Return the current stack pointer of the task.
  // This value is stored only during context switch,
  // so the running task will not get the actual value.
  OSStack_t *getStack(void);
  // Return the stack size given at task creation.
  unsigned short getStackSize(void);    /* bytes */
#if defined(OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER)
  // TODO: Remove it if is no longer necessary.
  OSProgramPtr_t getProgramCounter(void);
#endif
  // Return the task ID.
  int getID(void);
  // Returns the task's context.
  void *getContext(void);

  // Return task priority.
  // Higher means better priority.
  OSTaskPriority_t getPriority(void);
  void setPriority(OSTaskPriority_t priority);

  // Return the event the task is waiting for.
  // Cancelling a waiting task can be done by notifying
  // this event with a return value of OS_EVENT_WAIT_RETURN_CANCELED.
  OSEvent_t getEvent(void);
  // Set the event the task is waiting for.
  void setEvent(OSEvent_t event);
  // Returns the maximum usage of the stack.
  unsigned short getStackUsed(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Allow task to be put to sleep.
  void setAllowSleep(bool status);
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  // TODO: Remove the next 4 functions if are no longer used.
//  bool isInterrupted(void);
//  void setInterruption(bool flag);
//  void requestInterruption(void);
//  void ackInterruption(void);
#endif

#if defined(DEBUG)
  // TODO: this is the only place where
  // this function appears, should be removed?
//  void dumpPC(void);
#endif

  // public members
  // Saved SP during context switches.
  OSStack_t *m_pStack;

  // Wake up this task if it waits for the event received as parameter.
  int
  eventNotify(OSEvent_t event, OSEventWaitReturn_t ret =
      OSEventWaitReturn::OS_VOID);
private:
  friend class OSScheduler;
  friend class OSReadyList;

  // Initialize task's environment.
  void init(const char *pName, OSTaskMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStackBottom, unsigned short stackSize,
      OSTaskPriority_t priority);

  // Redirect to virtual function (taskMain).
  static void staticMain(OSTask * pt);

#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK)
  // Warning: no longer run in critical section
  // TODO: remove if no longer needed.
//  virtual void schedulerTick( void );
#endif

  // members
#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  // TODO: remove if no longer needed.
  //bool m_isInterrupted;
#endif

  //bool m_isPreempted;
  bool m_isSuspended;
  bool m_isWaiting;
  //bool m_hasReturnValue;

  // Events which the task is waiting for.
  OSEvent_t m_event;
  OSEventWaitReturn_t m_eventWaitReturn;

  // Task's priority.
  OSTaskPriority_t m_staticPriority;

  // Task's ID.
  unsigned char m_id;
  // Task's name.
  const char *m_pName;

  // Task's entry point (i.e. the function executed by this task).
  OSTaskMainPtr_t m_entryPoint;
  // Data passed to this task.
  void *m_pParameters;

  // The bottom of the task's stack.
  unsigned char *m_pStackBottom;
  // The size of the task's stack.
  unsigned short m_stackSize;   /* bytes */

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // True if the task can pe put to sleep.
  bool m_allowSleep;
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  // TODO: Remove it if is no longer necessary.
  unsigned short m_WDseconds;
#endif
  };

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)

inline void OSTask::ackInterruption(void)
  {
    setInterruption(false);
  }

#endif

#endif /*OSTASK_H_ */
