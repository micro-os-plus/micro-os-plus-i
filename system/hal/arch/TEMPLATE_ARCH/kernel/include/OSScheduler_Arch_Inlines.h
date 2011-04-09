/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OSSCHEDULER_INLINES_H_
#define HAL_ARCH_OSSCHEDULER_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Code to save the current task 'context' on the task control block.
 *
 * The code uses the global variable OSScheduler::ms_ppCurrentStack that points to the
 * current context (i.e. the OSTask::m_pStack, where the pointer to the
 * current stack frame is stored). This variable is set during the context
 * switch in OSScheduler::contextSwitch().
 */

inline void OSScheduler::contextSave(void)
  {
    // push all registers to stack
    // *OSScheduler::ms_ppCurrentStack = SP (stack pointer)
    // leave interrupts as they were
    asm volatile (
        "		\n"
        : : :
    );
  }

/*
 * Opposite to contextSave().
 */

inline void OSScheduler::contextRestore(void)
  {
    // SP = *OSScheduler::ms_ppCurrentStack
    // pop everything from stack
    asm volatile
    (
        "		\n"
        : : :
    );
  }


/*
 * Critical section management.
 *
 * Push BASEPRI onto the stack and disable interrupts.
 *
 * Notice: The function context is addressed via Rxx, not SP, so using the
 * stack as temporary storage should be safe.
 */
inline void OSScheduler::criticalEnter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)
    asm volatile
    (
        "		\n"
        : : :
    );
#endif
  }

/*
 * Pop BASEPRI from the stack, restore interrupts to previous status
 */
inline void OSScheduler::criticalExit(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)
    asm volatile
    (
        "		\n"
        "		\n"
        : : :
    );
#endif
  }

// release processor to next ready task
inline void OSScheduler::yieldImpl(void)
  {
    asm volatile
    (
        " 		\n"
        : : :
    );
  }

#endif /*HAL_ARCH_OSSCHEDULER_INLINES_H_*/
