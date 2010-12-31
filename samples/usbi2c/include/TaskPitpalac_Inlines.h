/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKPITPALACINLINES_H_
#define TASKPITPALACINLINES_H_

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

#endif /*TASKPITPALACINLINES_H_*/
