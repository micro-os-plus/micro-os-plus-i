/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSEVENTNOTIFIER_H_
#define OSEVENTNOTIFIER_H_

#include "portable/kernel/include/OS.h"

class OSEventNotifier
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSEventNotifier();
  virtual ~OSEventNotifier() = 0;

  virtual uint_t
  eventNotify(OSEvent_t event,
      OSEventWaitReturn_t eventReturn = OSEventWaitReturn::OS_VOID) = 0;

  virtual void
  registerThread(OSThread* pThread);

};

#endif /* OSEVENTNOTIFIER_H_ */
