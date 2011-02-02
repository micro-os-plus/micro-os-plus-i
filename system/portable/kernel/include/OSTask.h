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
  static const OSTaskPriority_t IDLE_PRIORITY = 0x00;
  static const OSTaskPriority_t MIN_PRIORITY = 0x01;
  static const OSTaskPriority_t MAX_PRIORITY = 0xFF;
  static const OSTaskPriority_t DEFAULT_PRIORITY = (MAX_PRIORITY/2+1);

  static const int NONE = -1;

  static const unsigned short STACK_MINIMAL_SIZE = OS_CFGINT_STACK_MINIMAL_SIZE;
  static const OSStack_t STACK_FILL_BYTE = 0x5A;


  //#define OS_TASK_NONE                    ((OSTask *)-1)

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

  virtual void taskMain(void);

  void suspend(void);
  void resume(void);
  bool isSuspended(void);
  bool isWaiting(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  bool isAllowDeepSleep();
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
  void virtualWatchdogSet(unsigned short seconds);
  void virtualWatchdogCheck(void);
#endif

  char const *getName(void);
  unsigned char *getStackBottom(void);
  OSStack_t *getStack(void);
  unsigned short getStackSize(void);    /* bytes */
#if defined(OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER)
  OSProgramPtr_t getProgramCounter(void);
#endif

  int getID(void);

  void *getContext(void);

  // higher means better priority
  OSTaskPriority_t getPriority(void);
  void setPriority(OSTaskPriority_t priority);

  OSEvent_t getEvent(void);
  void setEvent(OSEvent_t event);

  unsigned short getStackUsed(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  void setAllowSleep(bool status);
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  bool isInterrupted(void);
  void setInterruption(bool flag);
  void requestInterruption(void);
  void ackInterruption(void);
#endif

#if defined(DEBUG)
  void dumpPC(void);
#endif

  // public members
  OSStack_t *m_pStack; // saved SP during context switches

private:
  friend class OSScheduler;
  friend class OSReadyList;

  void init(const char *pName, OSTaskMainPtr_t entryPoint, void *pParameters,
      const OSStack_t *pStackBottom, unsigned short stackSize,
      OSTaskPriority_t priority);

  static void staticMain(OSTask * pt);

#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK)
  // Warning: no longer run in critical section
  virtual void schedulerTick( void );
#endif

  // members
#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
  bool m_isInterrupted;
#endif

  //bool m_isPreempted;
  bool m_isSuspended;
  bool m_isWaiting;
  //bool m_hasReturnValue;

  OSEvent_t m_event;
  OSEventWaitReturn_t m_eventWaitReturn;

  OSTaskPriority_t m_staticPriority;

  unsigned char m_id;
  const char *m_pName;

  OSTaskMainPtr_t m_entryPoint;
  void *m_pParameters;

  unsigned char *m_pStackBottom;
  unsigned short m_stackSize;   /* bytes */

#if defined(OS_INCLUDE_OSTASK_SLEEP)
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

#endif /*OSTASK_H_ */
