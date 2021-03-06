/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK4_H_
#define TASKBLINK4_H_

#include "portable/kernel/include/uOS.h"

#include "TaskBlink1.h"

class TaskBlink4 : public OSThread
  {
public:
  // active object constructor
  TaskBlink4(const char *pName, schedTicks_t rate, TaskBlink1& thread);

  // actual thread main code
  virtual void threadMain(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  schedTicks_t m_rate;
  bool m_bToggleBusyWait;
  bool m_bTogglePreemptive;
  TaskBlink1& m_thread;
  };

#endif /*TASKBLINK4_H_*/
