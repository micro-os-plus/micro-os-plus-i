/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKBUSYWAIT_H_
#define TASKBLINKBUSYWAIT_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlinkBusyWait : public OSTask
  {
public:
  // task constructor
  TaskBlinkBusyWait(const char *pName, unsigned int iLed, unsigned int rate);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  unsigned int m_rate;
  };

#endif /* TASKBLINKBUSYWAIT_H_ */
