/*
 *	Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKPITPALAC_INLINES_H_
#define TASKPITPALAC_INLINES_H_

#include "portable/kernel/include/OS.h"

#include "portable/tasks/include/BlinkLed.h"

inline void TaskPitpalac::ledInit(void)
  {
    BlinkLed::init();
  }

inline void TaskPitpalac::ledOn(void)
  {
    BlinkLed::on();
  }

inline void TaskPitpalac::ledOff(void)
  {
    BlinkLed::off();
  }

#endif /*TASKPITPALAC_INLINES_H_*/
