/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSEVENTFLAGS_H_
#define OSEVENTFLAGS_H_

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSEVENTFLAGS_WORD)
typedef unsigned short OSEventFlagsBits_t;
#else
typedef unsigned char OSEventFlagsBits_t;
#endif

class OSEventFlags
  {
public:
  OSEventFlags();

  OSEventFlagsBits_t notify(OSEventFlagsBits_t bits);
  OSEventFlagsBits_t get(void);
  OSEventFlagsBits_t clear(OSEventFlagsBits_t bits);

  OSEventFlagsBits_t wait(OSEventFlagsBits_t bits, bool isStrict = true);

  OSEvent_t getEvent(void);

private:
  volatile OSEventFlagsBits_t m_flags;
  };

inline OSEvent_t OSEventFlags::getEvent(void)
  {
    return (OSEvent_t)this;
  }

inline OSEventFlagsBits_t OSEventFlags::get(void)
  {
    return m_flags;
  }

#endif /*OSEVENTFLAGS_H_*/
