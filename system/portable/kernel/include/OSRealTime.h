/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSREALTIME_H_
#define OSREALTIME_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

class OSRealTime
{
public:
  OSRealTime();

  // critical section support
  static OSRealTimeCriticalSection critical;

  // Register the task that will receive notifications
  static void
  registerTask(OSTask* pTask);

  // Wake up this task if it waits for the given event.
  static int
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t ret = OSEventWaitReturn::OS_VOID);

  static void
  interruptTick(void);

private:
  static OSTask* volatile ms_pTask;
  static bool volatile ms_requestNofication;
  static OSEvent_t volatile ms_event;
  static OSEventWaitReturn_t volatile ms_ret;

private:
  char m_dummy;
};

// ----------------------------------------------------------------------------

#endif /* OSREALTIME_H_ */
