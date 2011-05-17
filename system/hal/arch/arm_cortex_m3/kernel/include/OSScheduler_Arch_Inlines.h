/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OSSCHEDULER_INLINES_H_
#define HAL_ARCH_OSSCHEDULER_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Code to save the current thread 'context' on the thread control block.
 *
 * The code uses the global variable OSScheduler::ms_ppCurrentStack that points to the
 * current context (i.e. the OSThread::m_pStack, where the pointer to the
 * current stack frame is stored). This variable is set during the context
 * switch in OSScheduler::contextSwitch().
 */

inline void OSScheduler::contextSave(void)
  {
    // push all registers to stack
    // *OSScheduler::ms_ppCurrentStack = SP (stack pointer)
    // leave interrupts as they were
    asm volatile (
        " mrs r0, psp           \n" /* load r0 with current process stack */
        " stmdb r0!, {r4-r11}   \n" /* push the remaining registers on process stack. */
        "                       \n"
        " movw r3, #:lower16:OSScheduler::ms_ppCurrentStack \n"
        " movt r3, #:upper16:OSScheduler::ms_ppCurrentStack \n"
        " ldr r2, [r3]          \n" /* &OSThread::m_pStack */
        " str r0, [r2]          \n" /* save the new top of stack into OSThread::m_pStack */
        "                       \n"
        " push {r3, lr}         \n" /* save return addr on caller Stack */
        :::"r0", "r2", "r3"
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
        " pop {r3, lr}          \n" /* restore return address */
        "                       \n"
        " ldr r2, [r3]          \n" /* &OSThread::m_pStack */
        " ldr r0, [r2]          \n" /* OSThread::m_pStack */
        "                       \n"
        " ldmia r0!, {r4-r11}   \n" /* pop new thread registers */
        " msr psp, r0           \n" /* restore process stack */
        :::"r0", "r2", "r3"
  );
  }


/*
 * Critical section management.
 *
 * Push BASEPRI onto the stack and disable interrupts.
 *
 * Notice: The function context is addressed via R11, not SP, so using the
 * stack as temporary storage should be safe.
 */
inline void OSScheduler::criticalEnter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)
    asm volatile
    (
        " mrs r0, primask       \n"
        " push {r0}             \n"
        " cpsid i               \n"
        : : : "r0"
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
        " pop {r0}              \n"
        " msr primask, r0       \n"
        : : : "r0"
  );
#endif
  }

// release processor to next ready thread
inline void OSScheduler::yieldImpl(void)
  {
    asm volatile
    (
        " svc 0                 \n"
        :::
  );
  }

#endif /* HAL_ARCH_OSSCHEDULER_INLINES_H_ */
