/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSREALTIME_H_
#define OSREALTIME_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSREALTIME)

// ----------------------------------------------------------------------------

class OSRealTime
{
public:
  OSRealTime();

  // critical section support
  static OSRealTimeCriticalSection critical;
  static OSTotalCriticalSection totalCritical;

  // Register the thread that will receive notifications
  static void
  registerThread(OSThread* pThread);

  // Wake up this thread if it waits for the given event.
  static int
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t ret = OSEventWaitReturn::OS_VOID);

  static void
  interruptTick(void);

private:
  static OSThread* volatile ms_pThread;
  static bool volatile ms_requestNofication;
  static OSEvent_t volatile ms_event;
  static OSEventWaitReturn_t volatile ms_ret;

private:
  char m_dummy;
};

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSREALTIME) */

#endif /* OSREALTIME_H_ */
