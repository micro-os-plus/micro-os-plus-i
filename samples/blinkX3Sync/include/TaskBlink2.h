/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK2_H_
#define TASKBLINK2_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlink2 : public OSThread
  {
public:
  // active object constructor
  TaskBlink2(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual thread main code
  virtual void threadMain(void);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  schedTicks_t m_rate;
  bool m_bToggle;
  };

#endif /*TASKBLINK2_H_*/
