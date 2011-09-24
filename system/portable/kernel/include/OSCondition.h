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
  OSReturn_t
  wait(OSEvent_t event, bool doNotBlock = false);

  // ----- Virtual methods ----------------------------------------------------

  // Called in a loop to test when the condition is satisfied.
  // Returns:
  //    OSReturn::OS_OK when the condition is satisfied
  //    OSReturn::OS_SHOULD_WAIT when the code should loop to wait for the condition
  //    any error code

  // It is abstract and it should be implemented by the derived class.
  virtual OSReturn_t
  checkSynchronisedCondition(void) = 0;

  // Should return true when the wait needs to be cancelled.
  // If not redefined, return false.
  virtual bool
  isCancelled(void);

  OSEventWaitReturn_t
  getEventWaitReturn(void);

private:

  OSEventWaitReturn_t m_eventWaitReturn;

  // --------------------------------------------------------------------------
};

inline OSEventWaitReturn_t
OSCondition::getEventWaitReturn(void)
{
  return m_eventWaitReturn;
}

// ----------------------------------------------------------------------------

#endif /* OSCONDITION_H_ */
