/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTHREAD_H_
#define OSTHREAD_H_

#include "portable/kernel/include/OS.h"

class OSScheduler;

typedef unsigned char OSThreadPriority_t;

typedef void
(*OSThreadMainPtr_t)(void *);

class OSThread
{
public:

  // Thread range priorities.
  static const OSThreadPriority_t IDLE_PRIORITY = 0x00;
  static const OSThreadPriority_t MIN_PRIORITY = 0x01;
  static const OSThreadPriority_t MAX_PRIORITY = 0xFF;
  static const OSThreadPriority_t DEFAULT_PRIORITY = (MAX_PRIORITY / 2 + 1);

  // Value for the minimal size for a thread.
  static const unsigned short STACK_MINIMAL_SIZE = OS_CFGINT_STACK_MINIMAL_SIZE;
  // Pattern for initialising the stack.
  static const OSStack_t STACK_FILL_BYTE = 0x5A;

  // Constructor; run static threadMain (C++ style).
  OSThread(const char *pName, const OSStack_t *pStack, unsigned short stackSize,
      OSThreadPriority_t priority = OSThread::DEFAULT_PRIORITY);

  // Constructor; run given entryPoint function (C style).
  OSThread(const char *pName, OSThreadMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStack, unsigned short stackSize,
      OSThreadPriority_t priority = OSThread::DEFAULT_PRIORITY);

  // Thread main() function.
  // Overridden by actual implementation.
  virtual void
  threadMain(void);

  // Suspend the thread and remove it from the ready list.
  void
  suspend(void);
  // Resume the thread, previously suspended by inserting it into the ready list.
  void
  resume(void);
  // Check if the thread is suspended.
  bool
  isSuspended(void);
  // Check if the thread is waiting on an event.
  bool
  isWaiting(void);

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  // Return TRUE if the thread can go to sleep, FALSE otherwise.
  bool isDeepSleepAllowed();
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  // Set the virtual watchdog expire interval (in seconds).
  void virtualWatchdogSet(unsigned short seconds);
  // If the virtual watchdog interval expires, the MCU is reset using
  // the hardware module watchdog.
  void virtualWatchdogCheck(void);
#endif

  // Return the thread name.
  char const *
  getName(void);

  // Return the address of the stack bottom.
  // Stack grows from high address to low address,
  // so this is the maximum address the stack can grow.
  unsigned char *
  getStackBottom(void);

  // Return the current stack pointer of the thread.
  // This value is stored only during context switch,
  // so the running thread will not get the actual value.
  OSStack_t *
  getStack(void);

  // Return the stack size given at thread creation.
  unsigned short
  getStackSize(void); /* bytes */

#if defined(OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER)
  OSProgramPtr_t getProgramCounter(void);
#endif

  // Return the thread ID.
  int
  getID(void);

  // Return the thread's context.
  void *
  getContext(void);

  // Return thread priority.
  // Higher means better priority.
  OSThreadPriority_t
  getPriority(void);

  // Set thread priority.
  void
  setPriority(OSThreadPriority_t priority);

  // Return the event the thread is waiting for.
  // Cancelling a waiting thread can be done by notifying
  // this event with a return value of OS_EVENT_WAIT_RETURN_CANCELED.
  OSEvent_t
  getEvent(void);

  // Set the event the thread is waiting for.
  void
  setEvent(OSEvent_t event);

  // Return the maximum usage of the stack, in bytes.
  unsigned short
  getStackUsed(void);

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  // Allow thread to be put to sleep.
  void setAllowSleep(bool status);
#endif

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)
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

  // Prepare the thread to enter the wait state
  bool
  eventWaitPrepare(OSEvent_t event);

  // Perform the actual wait
  OSEventWaitReturn_t
  eventWaitPerform(void);

  void
  eventWaitClear(void);

  // Chain the prepare and perform actions
  OSEventWaitReturn_t
  eventWait(OSEvent_t event);

  // Wake up this thread if it waits for the given event.
  int
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t ret = OSEventWaitReturn::OS_VOID);

  // Retrieve the value returned by the last eventWait()
  OSEventWaitReturn_t
  getEventWaitReturn(void);

  // Set the value to be returned by getEventWaitReturn()
  void
  setEventWaitReturn(OSEventWaitReturn_t ret);

private:
  friend class OSScheduler; // TODO: explain why they are here
  friend class OSActiveThreads;

  // Initialise thread's environment.
  void
  init(const char *pName, OSThreadMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStackBottom, unsigned short stackSize,
      OSThreadPriority_t priority);

  // Redirect to virtual function (threadMain).
  static void
  staticMain(OSThread * pt);

  static void
  yield(void);

#if defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK)
  // Warning: no longer run in critical section!
  virtual void schedulerTick( void );
#endif

  // members
#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)
  // True if the thread is interrupted.
  bool m_isInterrupted;
#endif

  // True if the thread is suspended.
  bool m_isSuspended;

  // True if thread is waiting for an event. The event value is in m_event.
  bool m_isWaiting;

  // The event which the thread is waiting for, or OSEvent::OS_NONE
  OSEvent_t m_event;

  // The value to be returned by eventWait(), or OSEventReturn::OS_NONE
  OSEventWaitReturn_t m_eventWaitReturn;

  // Thread's priority.
  OSThreadPriority_t m_staticPriority;

  // Thread's numerical ID, the index in the scheduler array.
  unsigned char m_id;

  // Thread's name.
  const char *m_pName;

  // Thread's entry point (i.e. the function executed by this thread).
  OSThreadMainPtr_t m_entryPoint;

  // Data passed to this thread.
  void *m_pParameters;

  // The bottom of the thread's stack (lowest address).
  unsigned char *m_pStackBottom;

  // The size of the thread's stack, in bytes.
  unsigned short m_stackSize;

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  // True if the thread can be put to sleep.
  bool m_allowSleep;
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  unsigned short m_WDseconds;
#endif
};

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

inline void OSThread::ackInterruption(void)
  {
    setInterruption(false);
  }

#endif

inline OSEventWaitReturn_t
OSThread::eventWait(OSEvent_t event)
{
  if (eventWaitPrepare(event))
    eventWaitPerform();
  return m_eventWaitReturn;
}

inline OSEventWaitReturn_t
OSThread::eventWaitPerform(void)
{
  yield();
  return m_eventWaitReturn;
}

inline OSEventWaitReturn_t
OSThread::getEventWaitReturn(void)
{
  return m_eventWaitReturn;
}

inline void
OSThread::setEventWaitReturn(OSEventWaitReturn_t ret)
{
  m_eventWaitReturn = ret;
}

inline void
OSThread::eventWaitClear(void)
{
  m_isWaiting = false;
  m_event = OSEvent::OS_NONE; // no longer wait for it
}

// ---

inline char const *
OSThread::getName(void)
{
  return m_pName;
}

inline unsigned char *
OSThread::getStackBottom(void)
{
  return m_pStackBottom;
}

inline OSStack_t *
OSThread::getStack(void)
{
  return m_pStack;
}

inline unsigned short
OSThread::getStackSize(void)
{
  return m_stackSize;
}

// since all registers are saved on the thread stack,
// the 'context' in simply a pointer to the stack (the address below
// the deepest register pushed; for offsets to registers please see
// OSScheduler::stackInitialise()).
inline void *
OSThread::getContext(void)
{
  return (void *) &m_pStack;
}

inline void
OSThread::setPriority(OSThreadPriority_t priority)
{
  m_staticPriority = priority;
}

inline void
OSThread::setEvent(OSEvent_t event)
{
  m_event = event;
}

inline int
OSThread::getID(void)
{
  return m_id;
}

inline OSEvent_t
OSThread::getEvent(void)
{
  return m_event;
}

inline OSThreadPriority_t
OSThread::getPriority(void)
{
  return m_staticPriority;
}

inline bool
OSThread::isSuspended(void)
{
  return m_isSuspended;
}

inline bool
OSThread::isWaiting(void)
{
  return m_isWaiting;
}

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)

inline bool
OSThread::isDeepSleepAllowed(void)
  {
    return m_allowSleep;
  }

inline void
OSThread::setAllowSleep(bool b)
  {
    m_allowSleep = b;
  }

#endif /* defined(OS_INCLUDE_OSTHREAD_SLEEP) */

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

inline bool
OSThread::isInterrupted(void)
  {
    return m_isInterrupted;
  }

inline void
OSThread::setInterruption(bool flag)
  {
    m_isInterrupted = flag;
  }

#endif /* defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) */

#endif /* OSTHREAD_H_ */
