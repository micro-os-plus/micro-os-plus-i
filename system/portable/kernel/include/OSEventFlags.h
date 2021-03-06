/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSEVENTFLAGS_H_
#define OSEVENTFLAGS_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSEVENTFLAGS_WORD)
typedef uint16_t OSEventFlagsBits_t;
#else
typedef unsigned char OSEventFlagsBits_t;
#endif

// Inter-thread synchronisation mechanism allowing a thread to wait for multiple events,
// defined as separate flags in a structure.
class OSEventFlags
{
public:

  const static OSEventFlagsBits_t ALL = ~0;
  const static OSEventFlagsBits_t ONE = 1;

public:

  // initialise internal structures
  OSEventFlags();

  // Set bits and send an internally identified event.
  // Return the new flags.
  OSEventFlagsBits_t
  notify(OSEventFlagsBits_t bits);

  // Return the current flags value
  OSEventFlagsBits_t
  get(void) const;

  // Clear the given flags.
  // Return the new flags.
  OSEventFlagsBits_t
  clear(OSEventFlagsBits_t bits);

  // Loop until one the requested bits are set.
  // If isStrict is true one of the requested bits must be set, in order to return.
  // If isStrict is false the call may return due to an error.
  // If any flag is already set, return immediately.
  // For timeouts please set timers to send events; this function will return
  // the timer event.
  OSReturn_t
  wait(OSEventFlagsBits_t bits, bool isStrict = true);

  // Return the event used for notification
  OSEvent_t
  getEvent(void) const;

  void
  registerThread(OSThread* pThread);

public:
  // Contain the current value of the flags
  volatile OSEventFlagsBits_t m_flags;

private:

  OSThread* m_pThread;

};

inline OSEvent_t
OSEventFlags::getEvent(void) const
{
  return (OSEvent_t) this;
}

inline OSEventFlagsBits_t
OSEventFlags::get(void) const
{
  return m_flags;
}

inline void
OSEventFlags::registerThread(OSThread* pThread)
{
  m_pThread = pThread;
}

#endif /* OSEVENTFLAGS_H_ */
