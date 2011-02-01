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

#define TSKBLKNEST_TIMER                        (AVR32_TC0)
#define TSKBLKNEST_CHANNEL                      (1)
#define TSKBLKNEST_CFGINT_TIMER_IRQ_ID          (AVR32_TC0_IRQ1)
#define TSKBLKNEST_CFGINT_TIMER_IRQ_LEVEL       (1)
#define TSKBLKNEST_CFGINT_TIMER_CLOCK_SELECT    (3)
#define TSKBLKNEST_CFGINT_TIMER_PRESCALLER      (4)
#define TSKBLKNEST_CFGINT_TICK_RATE_HZ          (3000)

class TaskBlinkNested : public OSTask
{
public:
  // task constructor
  TaskBlinkNested(const char *pName, unsigned char iLed, schedTicks_t rate);

  // actual task main code
  virtual void
  taskMain(void);

private:
  // members
  OSStack_t m_stack[(OSTask::STACK_MINIMAL_SIZE + 200) / sizeof(OSStack_t)];

  LedVar m_oLed;
  schedTicks_t m_rate;
};

#endif /*TASKBLINKBUSYWAIT_H_*/
