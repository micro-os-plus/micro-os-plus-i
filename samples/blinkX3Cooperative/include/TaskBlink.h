/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK_H_
#define TASKBLINK_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlink : public OSTask
  {
public:
  // task constructor
  TaskBlink(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  schedTicks_t m_rate;
  };

#endif /* TASKBLINK_H_ */
