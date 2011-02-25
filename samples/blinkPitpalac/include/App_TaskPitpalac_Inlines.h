/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKPITPALACINLINES_H_
#define TASKPITPALACINLINES_H_

#include "portable/devices/debug/include/OSDebugLed1.h"

inline void TaskPitpalac::ledInit(void)
  {
    OSDebugLed1::init();
  }

inline void TaskPitpalac::ledOn(void)
  {
    OSDebugLed1::on();
  }

inline void TaskPitpalac::ledOff(void)
  {
    OSDebugLed1::off();
  }

#endif /*TASKPITPALACINLINES_H_*/
