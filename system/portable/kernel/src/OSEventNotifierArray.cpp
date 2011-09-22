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

OSEventNotifierArray::~OSEventNotifierArray()
{
  OSDeviceDebug::putDestructor_P(PSTR("OSEventNotifierArray"), this);
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

  std::size_t i;
  for (i = 0; i < m_count; ++i)
    {
      if (m_ppThreadsArray[i] == pThread)
        {
          // Already in, avoid registering it for multiple times
          return OSReturn::OS_OK;
        }
    }

  // If not already in, must add

  if (m_count < m_size)
    {
      // If array not full, store pointer to array
      m_ppThreadsArray[m_count++] = pThread;
      ret = OSReturn::OS_OK;
    }
  else
    {
      OSDeviceDebug::putString_P(PSTR(" m_ppThreadsArray SIZE_EXCEEDED "));

      ret = OSReturn::OS_SIZE_EXCEEDED;
    }

  return ret;
}

#endif /* defined(OS_INCLUDE_OSEVENTNOTIFIERARRAY) */
