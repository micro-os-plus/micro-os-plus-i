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

  // Called to prepare to check condition and do a pre-check outside any
  // critical regions. Can use any system calls.
  //
  // Returns:
  //    OSReturn::OS_OK if the condition is satisfied, make wait return
  //    OSReturn::OS_SHOULD_WAIT if we should proceed to wait for the condition
  //    any error code, make the wait() return
  virtual OSReturn_t
  prepareCheckCondition(void);

  // Called to test if the condition is satisfied. Runs within a
  // critical region and cannot use any system calls.
  // Being abstract, must be implemented by the derived class.
  //
  // Returns:
  //    OSReturn::OS_OK if the condition is satisfied, make wait return
  //    OSReturn::OS_SHOULD_WAIT if we should proceed to wait for the condition
  //    OSReturn::OS_SHOULD_RETRY if we should return to prepare
  //    any error code, make the wait() return
  virtual OSReturn_t
  checkSynchronisedCondition(void) = 0;

  // Should return true when the wait() needs to be cancelled.
  // If not redefined, return false.
  virtual bool
  isCancelled(void);

  OSEventWaitReturn_t
  getEventWaitReturn(void);

  virtual void
  criticalSectionEnter(void);

  virtual void
  criticalSectionExit(void);

private:

  OSEventWaitReturn_t m_eventWaitReturn;

  // --------------------------------------------------------------------------
};

inline OSEventWaitReturn_t
OSCondition::getEventWaitReturn(void)
{
  return m_eventWaitReturn;
}

// ============================================================================

class OSRealTimeCondition : public OSCondition
{
public:

  // ----- Constructors & destructors -----------------------------------------

  OSRealTimeCondition();
  virtual
  ~OSRealTimeCondition();

  // ----- Public methods -----------------------------------------------------

  // ----- Virtual methods ----------------------------------------------------

  virtual OSReturn_t
  checkSynchronisedCondition(void) = 0;

  virtual void
  criticalSectionEnter(void);

  virtual void
  criticalSectionExit(void);

private:

  // --------------------------------------------------------------------------
};

// ----------------------------------------------------------------------------

#endif /* OSCONDITION_H_ */
