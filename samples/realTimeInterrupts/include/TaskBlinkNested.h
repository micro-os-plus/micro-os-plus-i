/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKNESTED_H_
#define TASKBLINKNESTED_H_

#include "portable/kernel/include/uOS.h"

#include "LedVar.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

extern bool g_flagNotify;

#define TSKBLKNEST_TIMER                        (AVR32_TC1)
#define TSKBLKNEST_CHANNEL                      (0)
#define TSKBLKNEST_CFGINT_TIMER_IRQ_ID          (AVR32_TC1_IRQ0)
#define TSKBLKNEST_CFGINT_TIMER_IRQ_LEVEL       (3)
#define TSKBLKNEST_CFGINT_TIMER_CLOCK_SELECT    (3)
#define TSKBLKNEST_CFGINT_TIMER_PRESCALLER      (4)
//#define TSKBLKNEST_CFGINT_TICK_RATE_HZ          (9000)

#define EVENT_NESTEED                           0x1234

class TaskBlinkNested : public OSTask
{
public:
  // task constructor
  TaskBlinkNested(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual task main code
  virtual void
  taskMain(void);

  void
  interruptInit(void);

  void
  interruptAcknowledge(void);

  void
  interruptServiceRoutine(void) __attribute__ ((noinline));

private:
  // members
  OSStack_t m_stack[(OSTask::STACK_MINIMAL_SIZE + 400) / sizeof(OSStack_t)];

  LedVar m_oLed;
  schedTicks_t m_rate;

  volatile schedTicks_t m_count;
};

#endif /*TASKBLINKBUSYWAIT_H_*/
