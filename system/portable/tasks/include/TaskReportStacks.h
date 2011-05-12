/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKREPORTSTACKS_H_
#define TASKREPORTSTACKS_H_

#include "portable/kernel/include/uOS.h"

class TaskReportStacks : public OSThread
{
public:
  // thread constructor
  TaskReportStacks(const char *pName, schedTicks_t rateSeconds,
      schedTicks_t maxSeconds, unsigned char increaseRate);

  // actual thread main code
  virtual void
  threadMain(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

  schedTicks_t m_rateSeconds;
  schedTicks_t m_maxSeconds;
  unsigned char m_increaseRate;
};

#endif /* TASKREPORTSTACKS_H_ */
