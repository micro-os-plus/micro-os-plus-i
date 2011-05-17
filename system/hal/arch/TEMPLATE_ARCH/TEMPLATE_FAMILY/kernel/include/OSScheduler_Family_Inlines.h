/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSSCHEDULER_INLINES_H_
#define HAL_FAMILY_OSSCHEDULER_INLINES_H_

// ----------------------------------------------------------------------------

/*
 * Overview
 *
 * These definitions handle the operations required to save and restore the
 * current thread 'context' to/from the thread control block (TCB).
 *
 * The processor registers are pushed onto/popped from the current thread stack,
 * and only the resulting stack pointer (SP) is stored in the current thread area.
 *
 * To make things simpler and faster, the code uses the global variable
 * OSScheduler::ms_ppCurrentStack that points to the current OSThread::m_pStack, where
 * the pointer to the current stack frame is located. This variable is
 * set during the context switch in OSScheduler::contextSwitch().
 *
 * - contextSave() : store the SP through the global pointer
 *      *OSScheduler::ms_ppCurrentStack = SP
 *
 * - contextRestore() : load the SP through the global pointer
 *      SP = *OSScheduler::ms_ppCurrentStack
 */


#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

/*
 *  Save the current SP to the current thread, via the global pointer.
 */


inline void
OSSchedulerImpl::stackPointerSave(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register unsigned int tmp1;
    register unsigned int tmp2;

    asm volatile
    (
        "  \n"
        "  \n" // RA = &OSScheduler::ms_ppCurrentStack
        "  \n" // RB = OSScheduler::ms_ppCurrentStack)
        "  \n" // *OSScheduler::ms_ppCurrentStack = SP

        : [RA] "=r" (tmp1), [RB] "=r" (tmp2)
        : [pTCB] "i" (&OSScheduler::ms_ppCurrentStack)
        :
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }


/*
 *  Restore the SP register for the current thread, via the global pointer
 */

inline void
OSSchedulerImpl::stackPointerRestore(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register unsigned int tmp1;
    register unsigned int tmp2;

    asm volatile
    (
        "  \n"
        "  \n" // RA = &OSScheduler::ms_ppCurrentStack
        "  \n" // RB = OSScheduler::ms_ppCurrentStack)
        "  \n" // SP = *OSScheduler::ms_ppCurrentStack

        : [RA] "=r" (tmp1), [RB] "=r" (tmp2)
        : [pTCB] "i" (&OSScheduler::ms_ppCurrentStack)
        : "sp"
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

/*
 * Save the global critical section nesting counter onto stack
 */

inline void
OSSchedulerImpl::criticalSectionNestingSave(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register unsigned int tmp1;
    register unsigned int tmp2;

    asm volatile
    (
        "  \n"
        "  \n"
        "  \n"
        "  \n " // push Nesting onto stack

        : [RA] "=r" (tmp1), [RB] "=r" (tmp2)
        : [pCSN] "i" (&OSCriticalSection::ms_nestingLevel)
        :
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

/*
 *  Restore the global critical section nesting counter from stack
 */

inline void
OSSchedulerImpl::criticalSectionNestingRestore(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    register unsigned int tmp1;
    register unsigned int tmp2;

    asm volatile
    (
        "  \n" // pop Nesting from stack
        "  \n"
        "  \n"
        "  \n"

        : [RA] "=r" (tmp1), [RB] "=r" (tmp2)
        : [pCSN] "i" (&OSCriticalSection::ms_nestingLevel)
        : "sp"
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

/*
 * Save the remaining R0-R7 registers onto the stack.
 */

inline void
OSSchedulerImpl::registersSave(void)
  {
    asm volatile
    (
        "  \n"

        :
        :
        : "sp"
  );
  }

/*
 * Restore the R0-R7 registers from the stack.
 */

inline void
OSSchedulerImpl::registersRestore(void)
  {
    asm volatile
    (
        "  \n"

        :
        :
        : "sp" // r0-r7 were also changed
  );
  }

/*
 * Check if we are allowed to start a context switch.
 *
 * In general we are not allowed when nested interrupts are used, i.e.
 * a higher priority interrupt occurred in another interrupt context.
 *
 * In this case, except saving/restoring registers, we should NOT attempt
 * anything else, not even storing the SP, since this will overwrite the
 * current thread context.
 *
 * In the AVR32 case, the processor mode is available as 3 bits in SR.
 * The value of 0 means user mode, 1 means system mode, both ok.
 * Anything higher than 1 means interrupt or exception mode, when context
 * switch is not allowed.
 *
 */

unsigned int getSRfromStack(void) __attribute__ ((always_inline));

inline unsigned int getSRfromStack(void)
  {
    register unsigned int ret;
    asm volatile
    (
        "  \n" // 8 without nesting

        : [RET] "=r" (ret)
        :
        :
  );

    return ret;
  }

inline bool
OSSchedulerImpl::isContextSwitchAllowed(void)
  {
    return ((getSRfromStack() >> 22) & 0x7) > 1 ? false : true;
  }

#endif /* OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING */

/*
 * Restore the artificially created context for the first thread.
 * Used only once at startup; all other threads will start via a regular
 * context switch.
 *
 * As a side-effect, this function also enables interrupts. This happens when
 * restoring the SR.
 *
 * The processor was (from reset) and remains in system mode.
 *
 * WARNING: Interrupts should be disabled when entering this code.
 *
 */

inline void
OSSchedulerImpl::FirstThread_contextRestore(void)
{

  OSSchedulerImpl::stackPointerRestore();
  OSSchedulerImpl::criticalSectionNestingRestore();
  OSSchedulerImpl::registersRestore();
  // R1-R7 should not be used below this line

  asm volatile
  (
      "  \n"

      :
      :
      : "r0", "sp"
);

}


/*
 * Perform a context switch by triggering a system call exception.
 */

inline void
OSSchedulerImpl::yield(void)
{
  asm volatile
  (
      " scall \n"
      :
      :
      :
);
}

#endif /* HAL_FAMILY_OSSCHEDULER_INLINES_H_ */
