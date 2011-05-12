/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKBUSYWAIT_H_
#define TASKBLINKBUSYWAIT_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlinkBusyWait : public OSThread
  {
public:
  // active object constructor
  TaskBlinkBusyWait(const char *pName, unsigned int iLed, unsigned int rate);

  // actual thread main code
  virtual void threadMain(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  unsigned int m_rate;
  };

#endif /* TASKBLINKBUSYWAIT_H_ */
