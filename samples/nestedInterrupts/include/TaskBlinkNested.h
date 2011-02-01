/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKNESTED_H_
#define TASKBLINKNESTED_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlinkNested : public OSTask
  {
public:
  // task constructor
  TaskBlinkNested(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  schedTicks_t m_rate;
  };

#endif /*TASKBLINKBUSYWAIT_H_*/
