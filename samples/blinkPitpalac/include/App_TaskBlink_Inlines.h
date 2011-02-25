/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKINLINES_H_
#define TASKBLINKINLINES_H_

#include "portable/devices/debug/include/OSDebugLed2.h"

inline void TaskBlink::ledInit(void)
  {
    OSDebugLed2::init();
  }

inline void TaskBlink::ledOn(void)
  {
    OSDebugLed2::on();
  }

inline void TaskBlink::ledOff(void)
  {
    OSDebugLed2::off();
  }

inline void TaskBlink::ledToggle(void)
  {
    OSDebugLed2::toggle();
  }

#endif /*TASKBLINKINLINES_H_*/
