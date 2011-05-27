/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSCRITICALSECTION_H_
#define OSCRITICALSECTION_H_

#include "portable/kernel/include/OS.h"

class OSCriticalSection
{
public:
#if defined(DEBUG)
  OSCriticalSection();
#endif

  // Pair of functions to enable disable interrupts

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

  // Inlines using the stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

  // Regular functions, using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

  static OSStack_t volatile ms_nestingLevel;

private:
  char m_dummy;
};

// ----------------------------------------------------------------------------

class OSRealTimeCriticalSection
{
public:
#if defined(DEBUG)
  OSRealTimeCriticalSection();
#endif
  // Pair of functions to enable disable interrupts

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

  // Inlines using the stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

  // Regular functions, using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

private:
  char m_dummy;
};

// ============================================================================

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

// When we can use the stack, the enter()/exit() calls are inlined

inline void
OSCriticalSection::enter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();
  OSCPUImpl::stackPush(tmp);

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
  tmp |= (OS_CFGINT_OSCRITICALSECTION_MASK);
  OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

inline void
OSCriticalSection::exit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

  register OSStack_t tmp;

  tmp = OSCPUImpl::stackPop();
  OSCPUImpl::setInterruptsMask(tmp);

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

// ----------------------------------------------------------------------------

// When we can use the stack, the enter()/exit() calls are inlined

inline void
OSRealTimeCriticalSection::enter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();
  OSCPUImpl::stackPush(tmp);

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
  tmp |= (OS_CFGINT_OSCRITICALSECTION_MASKRT);
  OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

inline void
OSRealTimeCriticalSection::exit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

  register OSStack_t tmp;

  tmp = OSCPUImpl::stackPop();
  OSCPUImpl::setInterruptsMask(tmp);

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

// ----------------------------------------------------------------------------

#endif /* OSCRITICALSECTION_H_ */

