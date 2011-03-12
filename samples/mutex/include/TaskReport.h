/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKREPORT_H_
#define TASKREPORT_H_

#include "portable/kernel/include/uOS.h"

class TaskReport : public OSTask
  {
public:
  // task constructor
  TaskReport(const char *pName, schedTicks_t seconds);

  // actual task main code
  virtual void taskMain(void);

private:
  // members
  OSStack_t m_stack[ (OSTask::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t) ];

  schedTicks_t m_rate;

  };

#endif /* TASKREPORT_H_ */
