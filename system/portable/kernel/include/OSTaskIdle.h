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

  // Main function task.
  // If this function run, it means that no other task needs the MCU.
  // It will try to put the MCU to sleep. After that (when the MCU is waked up)
  // a yield is made to give the control to other task, if there is another
  // task which in the meantime needs the MCU.
  virtual void taskMain(void);

#if defined(OS_INCLUDE_OSTASK_SLEEP)
  // Check if the MCU can be put at sleep.
  // If the sleep is allowed put the MCU to sleep.
  virtual bool enterSleep(void);
#endif

private:
  // members

  // Stack used by this task.
  OSStack_t m_stack[OS_CFGINT_IDLE_TASK_STACK_SIZE / sizeof(OSStack_t)];
  };

#endif /*OSTASKIDLE_H_*/
