/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSREALTIME)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSRealTime.h"

// ----- OSRealTime static variables ------------------------------------------

OSTask* volatile OSRealTime::ms_pTask;
bool volatile OSRealTime::ms_requestNofication;
OSEvent_t volatile OSRealTime::ms_event;
OSEventWaitReturn_t volatile OSRealTime::ms_ret;

// ----------------------------------------------------------------------------

OSRealTime::OSRealTime()
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("OSRealTime()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pTask = 0;

  ms_requestNofication = false;
  ms_event = 0;
  ms_ret = OSEventWaitReturn::OS_VOID;
}

void
OSRealTime::registerTask(OSTask* pTask)
{
  ms_pTask = pTask;
}

// Wake up this task if it waits for the given event.
int
OSRealTime::eventNotify(OSEvent_t event, OSEventWaitReturn_t ret)
{
  // Check first if there is a pending event.
  if (ms_requestNofication)
    return 0;

  // Store event details
  ms_event = event;
  ms_ret = ret;

  // Request a notification
  ms_requestNofication = true;

  return 1;
}

// This is called from the system timer interruptTick()
void
OSRealTime::interruptTick(void)
{
  if (ms_requestNofication && ms_pTask != 0)
    {
      if (ms_pTask->eventNotify(ms_event, ms_ret) == 1)
        {
          ms_requestNofication = false;
        }
    }
}

#endif /* defined(OS_INCLUDE_OSREALTIME) */
