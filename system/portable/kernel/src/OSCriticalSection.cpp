/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#include "portable/kernel/include/OS.h"

// ----- OSCriticalSection static variables -----------------------------------

// Used in contextSave/Restore.
OSStack_t volatile OSCriticalSection::ms_nestingLevel;

// ----------------------------------------------------------------------------

#if defined(DEBUG)
OSCriticalSection::OSCriticalSection()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSCriticalSection"), this);
}
#endif

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

// When we cannot use the stack, we no longer need to inline, so here are
// the usual routines to enter()/exit() critical sections.
void
OSCriticalSection::enter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();
    tmp |= (OS_CFGINT_OSCRITICALSECTION_MASK);
    OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
    OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

    ++ms_nestingLevel;

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

void
OSCriticalSection::exit(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    if ((ms_nestingLevel > 0) && (--ms_nestingLevel == 0))
      {
#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
        register OSStack_t tmp;

        tmp = OSCPUImpl::getInterruptsMask();
        tmp &= ~(OS_CFGINT_OSCRITICALSECTION_MASK);
        OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
        OSCPUImpl::interruptsEnable();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */
      }

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

// ----------------------------------------------------------------------------

#if defined(DEBUG)
OSRealTimeCriticalSection::OSRealTimeCriticalSection()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSRealTimeCriticalSection"), this);
}
#endif

#if defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK)

// When we cannot use the stack, we no longer need to inline, so here are
// the usual routines to enter()/exit() critical sections.
void
OSRealTimeCriticalSection::enter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
    register OSStack_t tmp;

    tmp = OSCPUImpl::getInterruptsMask();
    tmp |= (OS_CFGINT_OSCRITICALSECTION_MASKALL);
    OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
    OSCPUImpl::interruptsDisable();
#endif /* defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

    ++OSCriticalSection::ms_nestingLevel;

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

void
OSRealTimeCriticalSection::exit(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    if ((OSCriticalSection::ms_nestingLevel > 0) && (--OSCriticalSection::ms_nestingLevel == 0))
      {
#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
        register OSStack_t tmp;

        tmp = OSCPUImpl::getInterruptsMask();
        tmp &= ~(OS_CFGINT_OSCRITICALSECTION_MASKALL);
        OSCPUImpl::setInterruptsMask(tmp);
#else /* !defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
        OSCPUImpl::interruptsEnable();
#endif /* defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */
      }

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

#endif /* defined(OS_EXCLUDE_OSCRITICALSECTION_USE_STACK) */

// ----------------------------------------------------------------------------
