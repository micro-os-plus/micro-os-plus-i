/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKPITPALAC_H_
#define TASKPITPALAC_H_

#include "portable/kernel/include/OS.h"

#if !defined(OS_CFGINT_TASKPITPALAC_STACK_SIZE)
#define OS_CFGINT_TASKPITPALAC_STACK_SIZE       150
#endif

class TaskPitpalac: public OSTask
  {
public:
  // task constructor
  TaskPitpalac(const char *pName, schedTicks_t rate = 1);

  // actual task main code
  virtual void taskMain(void);

  void ledInit(void);
  void ledOn(void);
  void ledOff(void);

private:
  void waitAndToggle(schedTicks_t t1, schedTicks_t t2);

  // members
  OSStack_t m_stack[(OSTask::STACK_MINIMAL_SIZE + 150) / sizeof(OSStack_t)];
  schedTicks_t m_rate;

  };

#if defined(OS_CONFIG_BOARD_AVI_A07527) || defined(OS_CONFIG_BOARD_AVI_A07527)
#include "portable/tasks/include/TaskPitpalac_Inlines.h"
#else
#include "App_TaskPitpalac_Inlines.h"
#endif

#endif /*TASKPITPALAC_H_*/
