/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSEVENTNOTIFIERSINGLE_H_
#define OSEVENTNOTIFIERSINGLE_H_

#include "portable/kernel/include/OS.h"

class OSEventNotifierSingle
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSEventNotifierSingle();
  virtual ~OSEventNotifierSingle();

  // ----- Public methods -----------------------------------------------------

  virtual uint_t
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t eventReturn = OSEventWaitReturn::OS_VOID);

  virtual OSReturn_t
  registerThread(OSThread* pThread);

  // ----- Private members ----------------------------------------------------

private:

  OSThread* m_pThread;

  // --------------------------------------------------------------------------
};

#endif /* OSEVENTNOTIFIERSINGLE_H_ */
