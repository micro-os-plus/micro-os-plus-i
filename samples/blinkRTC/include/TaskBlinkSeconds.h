/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKSECONDS_H_
#define TASKBLINKSECONDS_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

class TaskBlinkSeconds : public OSTask
  {
public:
  // task constructor
  TaskBlinkSeconds(const char *pName, unsigned char iLed, unsigned int rate);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];

  LedVar m_oLed;
  unsigned int m_rate;
  };

#endif /*TASKBLINKSECONDS_H_*/
