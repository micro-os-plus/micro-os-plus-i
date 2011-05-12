/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK1_H_
#define TASKBLINK1_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlink1 : public OSThread
  {
public:
  // task constructor
  TaskBlink1(const char *pName, unsigned char iLed, schedTicks_t rate);
  virtual void taskMain(void);

  // actual task main code
  void setBusyWait(bool bFlag);

private:
  // members
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  schedTicks_t m_rate;
  bool m_bBusyWait;
  };

#endif /*TASKBLINK1_H_*/
