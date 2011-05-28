/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OS_ACTIVE_STACKS_REPORTER_H_
#define OS_ACTIVE_STACKS_REPORTER_H_

#include "portable/kernel/include/uOS.h"

#if !defined(OS_CFGINT_ACTIVE_STACKSREPORTER_EXTRA_STACK_SIZE)
#define OS_CFGINT_ACTIVE_STACKSREPORTER_EXTRA_STACK_SIZE OSThread::STACK_MINIMAL_SIZE
#endif /* !defined(OS_CFGINT_ACTIVE_STACKSREPORTER_EXTRA_STACK_SIZE) */

class StacksReporter : public OSThread
{
public:
  // thread constructor
  StacksReporter(const char* pName, schedTicks_t rateSeconds,
      schedTicks_t maxSeconds, uint8_t increaseRate,
      OSThreadPriority_t priority = OSThread::DEFAULT_PRIORITY);

  // actual thread main code
  virtual void
  threadMain(void);

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE
      + OS_CFGINT_ACTIVE_STACKSREPORTER_EXTRA_STACK_SIZE) / sizeof(OSStack_t)];

  schedTicks_t m_rateSeconds;
  schedTicks_t m_maxSeconds;
  uint8_t m_increaseRate;
};

#endif /* OS_ACTIVE_STACKS_REPORTER_H_ */
