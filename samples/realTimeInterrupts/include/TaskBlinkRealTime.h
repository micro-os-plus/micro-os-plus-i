/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKREALTIME_H_
#define TASKBLINKREALTIME_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

extern bool g_flagNotify;

class TaskBlinkRealTime : public OSThread
{
public:
  // task constructor
  TaskBlinkRealTime(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual task main code
  virtual void
  threadMain(void);

  void
  interruptInit(void);

  void
  interruptAcknowledge(void);

  static void
  contextHandler(void) __attribute__((interrupt));

  void
  interruptServiceRoutine(void) __attribute__((noinline));

private:
  // members
  OSStack_t m_stack[(OSThread::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

  LedVar m_oLed;
  schedTicks_t m_rate;

  volatile schedTicks_t m_count;
};

#endif /* TASKBLINKREALTIME_H_ */
