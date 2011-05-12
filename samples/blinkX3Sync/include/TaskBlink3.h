/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK3_H_
#define TASKBLINK3_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlink3 : public OSThread
  {
public:
  // active object constructor
  TaskBlink3(const char *pName, unsigned char iLed);

  // actual thread main code
  virtual void threadMain(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  };

#endif /*TASKBLINK3_H_*/
