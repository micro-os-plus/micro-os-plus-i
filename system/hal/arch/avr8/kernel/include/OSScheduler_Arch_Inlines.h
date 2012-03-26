/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSSCHEDULER_ARCH_INLINES_H_
#define HAL_OSSCHEDULER_ARCH_INLINES_H_

// ----------------------------------------------------------------------------

/*
 * Code to save the current thread 'context' on the thread control block.
 *
 * For simplicity, all general purpose registers are pushed onto the current
 * thread stack and only the new stack pointer is saved in the thread control block
 * (in other words, the 'context' is just a pointer to the stack).
 *
 * The first thing to do is save the flags then disable interrupts. This is to
 * guard our stack against having a context switch interrupt after we have already
 * pushed the registers onto the stack - causing the 32 registers to be on the
 * stack twice.
 *
 * R1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1.
 *
 * The interrupts have been disabled during the call to contextSave()
 * so we need not worry about reading/writing to the stack pointer.
 *
 * The code uses the global variable g_pCurrentContext that points to the
 * current context (i.e. the OSThread::m_pStack, where the pointer to the
 * current stack frame is stored). This variable is set during the context
 * switch in OSScheduler::contextSwitch().
 */

/*
 * Save all registers onto the stack.
 */

inline void
OSSchedulerImpl::registersSave(void)
  {
    asm volatile
    (
        " push  r0 \n"
        " in    r0, __SREG__ \n"
        " cli      \n"          // Enter a critical region
        " push  r0 \n"
        " push  r1 \n"
        " clr   r1 \n"
        " push  r2 \n"
        " push  r3 \n"
        " push  r4 \n"
        " push  r5 \n"
        " push  r6 \n"
        " push  r7 \n"
        " push  r8 \n"
        " push  r9 \n"
        " push  r10 \n"
        " push  r11 \n"
        " push  r12 \n"
        " push  r13 \n"
        " push  r14 \n"
        " push  r15 \n"
        " push  r16 \n"
        " push  r17 \n"
        " push  r18 \n"
        " push  r19 \n"
        " push  r20 \n"
        " push  r21 \n"
        " push  r22 \n"
        " push  r23 \n"
        " push  r24 \n"
        " push  r25 \n"
        " push  r26 \n"
        " push  r27 \n"
        " push  r28 \n"
        " push  r29 \n"
        " push  r30 \n"
        " push  r31 \n"

        :
        :
        :
  );
  }

/*
 * Restore all registers from the stack.
 */

inline void
OSSchedulerImpl::registersRestore(void)
  {
    asm volatile
    (
        " pop   r31 \n"
        " pop   r30 \n"
        " pop   r29 \n"
        " pop   r28 \n"
        " pop   r27 \n"
        " pop   r26 \n"
        " pop   r25 \n"
        " pop   r24 \n"
        " pop   r23 \n"
        " pop   r22 \n"
        " pop   r21 \n"
        " pop   r20 \n"
        " pop   r19 \n"
        " pop   r18 \n"
        " pop   r17 \n"
        " pop   r16 \n"
        " pop   r15 \n"
        " pop   r14 \n"
        " pop   r13 \n"
        " pop   r12 \n"
        " pop   r11 \n"
        " pop   r10 \n"
        " pop   r9 \n"
        " pop   r8 \n"
        " pop   r7 \n"
        " pop   r6 \n"
        " pop   r5 \n"
        " pop   r4 \n"
        " pop   r3 \n"
        " pop   r2 \n"
        " pop   r1 \n"
        " pop   r0 \n"
        " out   __SREG__, r0 \n" // Restore interrupts
        " pop   r0 \n"

        :
        :
        :
  );
  }

inline bool
OSSchedulerImpl::isContextSwitchAllowed(void)
  {
    // The AVR8 architecture does not implement nested interrupts
    return true;
  }

inline  bool
OSSchedulerImpl::isYieldAllowed(void)
{
  if ((OSCPUImpl::getInterruptsMask() & 0x80) == 0)
    return false;
  else
    return true;
}

inline void
OSSchedulerImpl::stackPointerSave(void)
  {
    asm volatile
    (
        " lds   r26, %[pTCB] \n"
        " lds   r27, %[pTCB]+1 \n"
        " in    r0, __SP_L__ \n"
        " st    x+, r0 \n"
        " in    r0, __SP_H__ \n"
        " st    x+, r0 \n"

        :
        : [pTCB] "i" (&OSScheduler::ms_ppCurrentStack)
        :
  );
  }

inline void
OSSchedulerImpl::stackPointerRestore(void)
  {
    asm volatile
    (
        " lds   r26, %[pTCB] \n"
        " lds   r27, %[pTCB]+1 \n"
        " ld    r28, x+ \n"
        " out   __SP_L__, r28 \n"
        " ld    r29, x+ \n"
        " out   __SP_H__, r29 \n"

        :
        : [pTCB] "i" (&OSScheduler::ms_ppCurrentStack)
        :
  );
  }

#if false
/*
 * Critical section management.
 *
 * Push SREG onto the stack and disable interrupts.
 *
 * Notice: The function context is addressed via Y, not SP, so using the
 * stack as temporary storage should be safe.
 */
inline void
OSCriticalSection::enter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    asm volatile
    (
        " in    __tmp_reg__, __SREG__ \n"
        " cli           \n"
        " push  __tmp_reg__ \n"

        :
        :
        :
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

/*
 * Pop SREG from the stack, restore interrupts to previous status
 */
inline void
OSCriticalSection::exit(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)

    asm volatile
    (
        " pop   __tmp_reg__ \n"
        " out   __SREG__, __tmp_reg__ \n"

        :
        :
        :
  );

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

#endif

// save the critical section nesting counter
inline  void
OSSchedulerImpl::criticalSectionNestingSave(void)
{
  ;
}

// restore the critical section nesting counter
inline void
OSSchedulerImpl::criticalSectionNestingRestore(void)
{
  ;
}

#endif /* HAL_OSSCHEDULER_ARCH_INLINES_H_ */
