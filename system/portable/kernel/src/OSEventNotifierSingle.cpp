/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSEVENTNOTIFIERSINGLE)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSEventNotifierSingle.h"

OSEventNotifierSingle::OSEventNotifierSingle()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSEventNotifierSingle"), this);
}

uint_t
OSEventNotifierSingle::eventNotify(OSEvent_t event,
    OSEventWaitReturn_t eventReturn)
{
  uint_t count;
  count = (*m_pThread)->eventNotify(event, eventReturn);

  return count;
}

OSReturn_t
OSEventNotifierArray::registerThread(OSThread* pThread)
{
  OSReturn_t ret;

  m_pThread = pThread;
  ret = OSReturn::OS_OK;

  return ret;
}

#endif /* defined(OS_INCLUDE_OSEVENTNOTIFIERSINGLE) */
