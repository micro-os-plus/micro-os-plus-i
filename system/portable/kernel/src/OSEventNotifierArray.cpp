/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSEVENTNOTIFIERARRAY)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSEventNotifierArray.h"

OSEventNotifierArray::OSEventNotifierArray(OSThread** ppThreadsArray,
    std::size_t threadsArraySize)
{
  OSDeviceDebug::putConstructor_P(PSTR("OSEventNotifierArray"), this);

  m_ppThreadsArray = ppThreadsArray;
  m_size = threadsArraySize;
  m_count = 0;
}

uint_t
OSEventNotifierArray::eventNotify(OSEvent_t event,
    OSEventWaitReturn_t eventReturn)
{
  uint_t count;
  count = 0;

  OSThread** ppThread;
  ppThread = m_ppThreadsArray;

  for (std::size_t i = 0; i < m_count; ++i)
    {
      count += (*ppThread)->eventNotify(event, eventReturn);
      ++ppThread;
    }

  return count;
}

OSReturn_t
OSEventNotifierArray::registerThread(OSThread* pThread)
{
  OSReturn_t ret;

  if (m_count < m_size)
    {
      m_ppThreadsArray[m_count++] = pThread;
      ret = OSReturn::OS_OK;
    }
  else
    {
      OSDeviceDebug::putString_P(
          PSTR("OSEventNotifierArray::registerThread() size exceeded"));
      OSDeviceDebug::putNewLine();

      ret = OSReturn::OS_SIZE_EXCEEDED;
    }

  return ret;
}

#endif /* defined(OS_INCLUDE_OSEVENTNOTIFIERARRAY) */
