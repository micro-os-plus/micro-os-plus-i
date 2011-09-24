/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSCONDITION_H_
#define OSCONDITION_H_

#include "portable/kernel/include/OS.h"

class OSCondition
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSCondition();
  virtual
  ~OSCondition();

  // ----- Public methods -----------------------------------------------------

  // It is recommended to check OS_CANCELLED, OS_TIMEOUT.
  OSEventWaitReturn_t
  wait(OSEvent_t event, bool doNotBlock = false);

  // ----- Virtual methods ----------------------------------------------------

  // Called in a loop to test when the condition is satisfied.
  // Should be implemented by the derived class.
  virtual bool
  isConditionSatisfied(void) = 0;

  // Should return true when the wait needs to be cancelled.
  // If not redefined, return false.
  virtual bool
  isCancelled(void);

  // --------------------------------------------------------------------------
};

#endif /* OSCONDITION_H_ */
