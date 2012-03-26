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

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

  // Inlines using the stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

  // Regular functions, using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  static OSStack_t volatile ms_nestingLevel;

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  static OSStack_t* volatile ms_nestingStackPointer;

  static void
  pushInterruptMask(OSStack_t mask);
  static OSStack_t
  popInterruptMask(void);

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK) */

private:
  char m_dummy;
};

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSREALTIME)

class OSRealTimeCriticalSection
{
public:
#if defined(DEBUG)
  OSRealTimeCriticalSection();
#endif
  // Pair of functions to enable disable interrupts

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

  // Inlines using the stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

  // Regular functions, using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

private:
  char m_dummy;
};

class OSTotalCriticalSection
{
public:
#if defined(DEBUG)
  OSTotalCriticalSection();
#endif
  // Pair of functions to enable disable interrupts

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

  // Inlines using the stack to save/restore the interrupt status.

  static void
  enter(void) __attribute__((always_inline));

  static void
  exit(void) __attribute__((always_inline));

#else /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

  // Regular functions, using a nesting counter.

  static void
  enter(void);

  static void
  exit(void);

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

private:
  char m_dummy;
};

#endif /* defined(OS_INCLUDE_OSREALTIME) */

// ============================================================================

#if !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

// When we can use the stack, the enter()/exit() calls are inlined

inline void
OSCriticalSection::enter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
    OSCPUImpl::setInterruptsMask(tmp | OS_CFGINT_OSCRITICALSECTION_MASK);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
    OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

    OSCPUImpl::stackPush(tmp);

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

#if defined(OS_INCLUDE_OSREALTIME)

// When we can use the stack, the enter()/exit() calls are inlined

inline void
OSRealTimeCriticalSection::enter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
    OSCPUImpl::setInterruptsMask(tmp | OS_CFGINT_OSCRITICALSECTION_MASKRT);
#else /* !defined(OS_INCLUDE_OSSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
    OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

    OSCPUImpl::stackPush(tmp);

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

#endif /* defined(OS_INCLUDE_OSREALTIME) */

#endif /* !defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

inline void
OSCriticalSection::pushInterruptMask(OSStack_t mask)
{
  *ms_nestingStackPointer++ = mask;
}

inline OSStack_t
OSCriticalSection::popInterruptMask(void)
{
  return *--ms_nestingStackPointer;
}

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK) */

// ----------------------------------------------------------------------------

#endif /* OSCRITICALSECTION_H_ */

