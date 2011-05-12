/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINK_H_
#define TASKBLINK_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

// simple task definition. will blink a given led with a given rate.

class TaskBlink : public OSThread
  {
public:
  // task constructor
  TaskBlink(const char *pName, unsigned char iLed, schedTicks_t iRate);

  // actual task main code
  virtual void taskMain(void);

private:
  // members

  // task stack reserved here
  OSStack_t m_stack[ (OSThread::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t) ];

  LedVar m_oLed; // led bit
  schedTicks_t m_iRate; // blink rate in scheduler ticks
  };

#endif /*TASKBLINK_H_*/
