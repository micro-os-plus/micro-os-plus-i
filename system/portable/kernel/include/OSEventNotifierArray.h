/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSEVENTNOTIFIERARRAY_H_
#define OSEVENTNOTIFIERARRAY_H_

#include "portable/kernel/include/OS.h"

class OSEventNotifierArray
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSEventNotifierArray(OSThread** ppThreadArray, std::size_t threadArraySize);
  virtual ~OSEventNotifierArray();

  // ----- Public methods -----------------------------------------------------

  virtual uint_t
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t eventReturn = OSEventWaitReturn::OS_VOID);

  virtual OSReturn_t
  registerThread(OSThread* pThread);

  // ----- Private members ----------------------------------------------------

private:

  OSThread** m_ppThreadsArray;
  std::size_t m_size;
  std::size_t m_count;

  // --------------------------------------------------------------------------
};

#endif /* OSEVENTNOTIFIERARRAY_H_ */
