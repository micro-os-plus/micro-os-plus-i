/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OS.h"

// ----- OSCriticalSection static variables -----------------------------------

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)
// Used in contextSave/Restore.
OSStack_t volatile OSCriticalSection::ms_nestingLevel;
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)
OSStack_t* volatile OSCriticalSection::ms_nestingStackPointer;
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK) */

// ----------------------------------------------------------------------------

#if defined(DEBUG)
OSCriticalSection::OSCriticalSection()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSCriticalSection"), this);
}
#endif /* defined(DEBUG) */

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

// When we cannot use the system stack, we no longer need to inline,
// so here are the usual routines to enter()/exit() critical sections.
void
OSCriticalSection::enter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
  OSCPUImpl::setInterruptsMask(tmp | OS_CFGINT_OSCRITICALSECTION_MASK);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

#if true
  // Push actual interrupt mask onto the thread stack.
  // The stack pointer is set at stackInitialise() and
  // updated at context switch time.
  pushInterruptMask(tmp);
#else
  OSThread* pCurrentThread;
  pCurrentThread = OSScheduler::getThreadCurrent();

#if defined(DEBUG) && defined(OS_DEBUG_OSCRITICALSECTION)
  OSDeviceDebug::putString("C[ ");
  OSDeviceDebug::putHex(tmp);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putHex(tmp | OS_CFGINT_OSCRITICALSECTION_MASK);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putPtr(pCurrentThread);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(ms_nestingLevel);
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSCRITICALSECTION) */

#if defined(DEBUG)
  if (pCurrentThread == 0)
    {
      OSDeviceDebug::putString("OSCriticalSection::enter() null");
      OSDeviceDebug::putNewLine();
      for (;;)
        ;
    }
#endif /* defined(DEBUG) */

  // Push current interrupt mask onto the current thread stack
  OSStack_t* pStack;
  pStack = &(pCurrentThread->m_criticalSectionNestingStack[0]);
  pStack[ms_nestingLevel] = tmp;
  ++ms_nestingLevel;
#endif

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  OSCPUImpl::interruptsDisable();
  ++ms_nestingLevel;

#else
#warning "Empty OSCriticalSection::enter()"
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

void
OSCriticalSection::exit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

#if true

  // Pop interrupt mask from the thread stack
  tmp = popInterruptMask();

#else
  OSThread* pCurrentThread;
  pCurrentThread = OSScheduler::getThreadCurrent();

#if defined(DEBUG)
  if (pCurrentThread == 0)
    {
      OSDeviceDebug::putString("OSCriticalSection::exit() null");
      OSDeviceDebug::putNewLine();
      for (;;)
        ;
    }
#endif /* defined(DEBUG) */


  // Pop current interrupt mask from the thread stack
  tmp = pCurrentThread->m_criticalSectionNestingStack[--ms_nestingLevel];

#if defined(DEBUG) && defined(OS_DEBUG_OSCRITICALSECTION)
  OSDeviceDebug::putString("C] ");
  OSDeviceDebug::putHex(tmp);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putPtr(pCurrentThread);
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(ms_nestingLevel);
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_DEBUG_OSCRITICALSECTION) */
#endif

  OSCPUImpl::setInterruptsMask(tmp);

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  if ((ms_nestingLevel > 0) && (--ms_nestingLevel == 0))
    {
      OSCPUImpl::interruptsEnable();
    }

#else
#warning "Empty OSCriticalSection::exit()"
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_OSREALTIME)

#if defined(DEBUG)
OSRealTimeCriticalSection::OSRealTimeCriticalSection()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSRealTimeCriticalSection"), this);
}
#endif

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

// When we cannot use the stack, we no longer need to inline, so here are
// the usual routines to enter()/exit() critical sections.
void
OSRealTimeCriticalSection::enter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
  OSCPUImpl::setInterruptsMask(tmp | (OS_CFGINT_OSCRITICALSECTION_MASKRT));
#else /* !defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

#if true
  OSCriticalSection::pushInterruptMask(tmp);
#else
  // Push current interrupt mask onto the thread stack
  OSScheduler::getThreadCurrent()->m_criticalSectionNestingStack[OSCriticalSection::ms_nestingLevel++]
      = tmp;
#endif

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  OSCPUImpl::interruptsDisable();
  ++OSCriticalSection::ms_nestingLevel;

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

void
OSRealTimeCriticalSection::exit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

#if true
  tmp = OSCriticalSection::popInterruptMask();
#else
  // Pop current interrupt mask from the thread stack
  tmp
      = OSScheduler::getThreadCurrent()->m_criticalSectionNestingStack[--OSCriticalSection::ms_nestingLevel];
  OSCPUImpl::setInterruptsMask(tmp);
#endif

  OSCPUImpl::setInterruptsMask(tmp);

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  if ((OSCriticalSection::ms_nestingLevel > 0) && (--OSCriticalSection::ms_nestingLevel == 0))
    {
      OSCPUImpl::interruptsEnable();
    }

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

// ============================================================================

#if defined(DEBUG)
OSTotalCriticalSection::OSTotalCriticalSection()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSTotalCriticalSection"), this);
}
#endif

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK)

// When we cannot use the stack, we no longer need to inline, so here are
// the usual routines to enter()/exit() critical sections.
void
OSTotalCriticalSection::enter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

  tmp = OSCPUImpl::getInterruptsMask();

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
  OSCPUImpl::setInterruptsMask(tmp | (OS_CFGINT_OSCRITICALSECTION_MASKALL));
#else /* !defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
  OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

#if true
  OSCriticalSection::pushInterruptMask(tmp);
#else
  // Push current interrupt mask onto the thread stack
  OSScheduler::getThreadCurrent()->m_criticalSectionNestingStack[OSCriticalSection::ms_nestingLevel++]
      = tmp;
#endif

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  OSCPUImpl::interruptsDisable();
  ++OSCriticalSection::ms_nestingLevel;

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

void
OSTotalCriticalSection::exit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_THREAD_STACK)

  register OSStack_t tmp;

#if true
  tmp = OSCriticalSection::popInterruptMask();
#else
  // Pop current interrupt mask from the thread stack
  tmp
      = OSScheduler::getThreadCurrent()->m_criticalSectionNestingStack[--OSCriticalSection::ms_nestingLevel];
  OSCPUImpl::setInterruptsMask(tmp);
#endif

  OSCPUImpl::setInterruptsMask(tmp);

#elif defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)

  if ((OSCriticalSection::ms_nestingLevel > 0) && (--OSCriticalSection::ms_nestingLevel == 0))
    {
      OSCPUImpl::interruptsEnable();
    }

#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}
#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_SYSTEM_STACK) */

#endif /* defined(OS_INCLUDE_OSREALTIME) */


// ----------------------------------------------------------------------------
