/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTASKIDLE_H_
#define OSTASKIDLE_H_

#include "portable/kernel/include/OSTask.h"

#ifndef OS_CFGINT_IDLE_TASK_STACK_SIZE
#define OS_CFGINT_IDLE_TASK_STACK_SIZE  (OSTask::STACK_MINIMAL_SIZE+50)
#endif

class OSTaskIdle : public OSTask
  {
public:
  OSTaskIdle();

  virtual void taskMain(void);
#if defined(OS_INCLUDE_OSTASK_SLEEP)
  virtual bool enterSleep(void);
#endif

private:
  // members
  OSStack_t m_stack[OS_CFGINT_IDLE_TASK_STACK_SIZE / sizeof(OSStack_t)];
  };

#endif /*OSTASKIDLE_H_*/
