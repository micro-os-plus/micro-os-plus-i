/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef TASKBLINKINLINES_H_
#define TASKBLINKINLINES_H_

#include "portable/devices/debug/include/OSDebugLed1.h"

inline void TaskBlink::ledInit(void)
  {
    OSDebugLed1::init();
  }

inline void TaskBlink::ledOn(void)
  {
    OSDebugLed1::on();
  }

inline void TaskBlink::ledOff(void)
  {
    OSDebugLed1::off();
  }

inline void TaskBlink::ledToggle(void)
  {
    OSDebugLed1::toggle();
  }

#endif /*TASKBLINKINLINES_H_*/
