/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK_H_
#define TASKBLINK_H_

#include "portable/kernel/include/OS.h"
//#include "AppConfig.h"

class TaskBlink : public OSTask
  {
public:
  // task constructor
  TaskBlink(const char *pName, schedTicks_t rate = 1);

  // actual task main code
  virtual void taskMain(void);

  void ledInit(void);
  void ledOn(void);
  void ledOff(void);
  void ledToggle(void);

private:
  // members
  OSStack_t m_stack[(OSTask::STACK_MINIMAL_SIZE + 150)/sizeof(OSStack_t)];
  schedTicks_t m_rate;

  Timer m_timerSeconds;
  };

#include "App_TaskBlink_Inlines.h"

#endif /*TASKBLINK_H_*/
