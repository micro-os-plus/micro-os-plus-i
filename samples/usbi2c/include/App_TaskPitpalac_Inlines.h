/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef APP_TASKPITPALACINLINES_H_
#define APP_TASKPITPALACINLINES_H_

#include "portable/tasks/include/BlinkLed.h"

inline void
TaskPitpalac::ledInit(void)
{
  BlinkLed::init();
}

inline void
TaskPitpalac::ledOn(void)
{
  BlinkLed::on();
}

inline void
TaskPitpalac::ledOff(void)
{
  BlinkLed::off();
}

#endif /*APP_TASKPITPALACINLINES_H_*/
