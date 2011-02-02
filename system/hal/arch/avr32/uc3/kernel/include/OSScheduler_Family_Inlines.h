/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSSCHEDULER_INLINES_H_
#define HAL_FAMILY_OSSCHEDULER_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Code to save the current task 'context' on the task control block.
 *
 * The code uses the global variable g_ppCurrentStack that points to the
 * current context (i.e. the OSTask::m_pStack, where the pointer to the
 * current stack frame is stored). This variable is set during the context
 * switch in OSScheduler::contextSwitch().
 */

/*
 * portSAVE_CONTEXT_INT() and portRESTORE_CONTEXT_INT(): for INT0..3 exceptions.
 * portSAVE_CONTEXT_SCALL() and portRESTORE_CONTEXT_SCALL(): for the scall exception.
 *
 * Had to make different versions because registers saved on the system stack
 * are not the same between INT0..3 exceptions and the scall exception.
 */

// Task context stack layout:
// R8  (*)
// R9  (*)
// R10 (*)
// R11 (*)
// R12 (*)
// R14/LR (*)
// R15/PC (*)
// SR (*)
// R0
// R1
// R2
// R3
// R4
// R5
// R6
// R7

// (*) automatically done for INT0..INT3, but not for SCALL

#if defined(OS_INCLUDE_OSSCHEDULERIMPL_CONTEXT_PROCESSING)

// Save the current SP to the current task, via the global pointer.
inline void
OSSchedulerImpl::stackPointerSave(void)
  {
    asm volatile (
        " mov     r8, LO(%[pxCurrentTCB]) \n"
        " orh     r8, HI(%[pxCurrentTCB]) \n"
        " ld.w    r9, r8[0] \n"
        " st.w    r9[0], sp \n"
        :
        : [pxCurrentTCB] "i" (&g_ppCurrentStack)
        : "r9", "r8"
    );
  }

// Restore the current SP from the current task, via the global pointer
inline void
OSSchedulerImpl::stackPointerRestore(void)
  {
    asm volatile (
        " mov     r8, LO(%[pxCurrentTCB]) \n"
        " orh     r8, HI(%[pxCurrentTCB]) \n"
        " ld.w    r9, r8[0] \n"
        " ld.w    sp, r9[0] \n"
        :
        : [pxCurrentTCB] "i" (&g_ppCurrentStack)
        : "r8", "r9", "sp"
    );
  }

// Save the remaining r0-r7 registers on stack;
// the others were saved by the interrupt
inline void
OSSchedulerImpl::registersSave(void)
  {
    asm volatile (
        " stm     --sp, r0-r7 \n"
        :
        :
        : "sp"
    );
  }

// Restore r0-r7 registers from stack;
// the others will be restored when exiting the interrupt.
inline void
OSSchedulerImpl::registersRestore(void)
  {
    asm volatile (
        " ldm     sp++, r0-r7 \n"
        // R0-R7 should not be used below this line
        :
        :
        : "sp" // ro-r7 were also changed
    );
  }

/* Check if INT0 or higher were being handled (case where the OS tick interrupted another */
/* interrupt handler (which was of a higher priority level but decided to lower its priority */
/* level and allow other lower interrupt level to occur). */
/* In this case we don't want to do a task switch because we don't know what the stack */
/* currently looks like (we don't know what the interrupted interrupt handler was doing). */

/* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */
/* will just be restoring the interrupt handler, no way!!! */
/* So, since we won't do a vTaskSwitchContext(), it's of no use to save SP. */

#if false
inline void
OSSchedulerImpl::isContextSwitchAllowed(void)
  {
    asm volatile (
        " ld.w    r0, sp[9*4] \n" // Read SR in stack
        " bfextu  r0, r0, 22, 3 \n" // Extract the mode bits to R0.
        " cp.w    r0, 1 \n" // Compare the mode bits with supervisor mode(b'001)
        " brhi    0f \n"
        :
        :
        : "r0"
    );
  }
#else
inline bool
OSSchedulerImpl::isContextSwitchAllowed(void)
  {
    register bool bRet asm ("r8");

    // Return 0 (false) if SR mode bits > 1, i.e. in interrupt mode
    // Return 1 (true) if SR mode bits <= 1, i.e. in application or supervisor mode
    asm volatile (
        " ld.w    r0, sp[9*4] \n" // Read SR in stack
        " bfextu  r0, r0, 22, 3 \n" // Extract the mode bits to R0.
        " cp.w    r0, 1 \n" // Compare the mode bits with supervisor mode(b'001)
        " brhi    9f \n" // if interrupt or exception, jump
        " mov     r8, 1 \n" // return true
        " bral    8f \n"
        "9: "
        " mov     r8, 0 \n" // return false
        "8:"
        : "=r" (bRet)
        :
        : "r0"
    );
    return bRet;
  }

#endif
#endif

void
SCALL_contextSave(void) __attribute__( ( always_inline ) );
void
SCALL_contextRestore(void) __attribute__( ( always_inline ) );
void
FirstTask_contextRestore(void) __attribute__( ( always_inline ) );

// Restore artificial context for first task startup
inline void
OSSchedulerImpl::FirstTask_contextRestore(void)
{
#if true

  asm volatile (
      " mov     r8, LO(%[pxCurrentTCB]) \n"
      " orh     r8, HI(%[pxCurrentTCB]) \n"
      " ld.w    r0, r8[0] \n"
      " ld.w    sp, r0[0] \n"

      // Restore R0..R7
      " ldm     sp++, r0-r7 \n"
      // R0-R7 should not be used below this line
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack)
      :
  );

#else

  OSSchedulerImpl::stackPointerRestore();
  OSSchedulerImpl::registersRestore();

#endif

  asm volatile (
      // Skip PC and SR (will do it at the end)
      " sub     sp, -2*4 \n"

      // Restore R8..R12 and LR
      " ldm     sp++, r8-r12, lr \n"

      // Restore SR
      " ld.w    r0, sp[-8*4]\n\t" // R0 is modified, is restored later.
      " mtsr    %[SR], r0 \n"

      // Restore r0
      " ld.w    r0, sp[-9*4] \n"

      // Restore PC from stack - PC is the 7th register saved
      " ld.w    pc, sp[-7*4] \n"

      :
      : [SR] "i" (AVR32_SR)
      :
  );

  /* Leave pxCurrentTCB variable access critical section */

  //OSScheduler::criticalExit();

  asm volatile (

      /* skip PC and SR */

      /* do not use SP if interrupts occurs, SP must be left at bottom of stack */
      " sub     r7, sp, -10*4 \n"

      /* Restore r8-r12 and LR */
      " ldm     r7++, r8-r12, lr \n"

      /* RETS will take care of the extra PC and SR restore. */
      /* So, we have to prepare the stack for this. */
      " ld.w    r0, r7[-8*4] \n" /* Read SR */
      " st.w    r7[-2*4], r0 \n" /* Copy SR */
      " ld.w    r0, r7[-7*4] \n" /* Read PC */
      " st.w    r7[-1*4], r0 \n" /* Copy PC */

      /* Restore R0..R7 */
      " ldm     sp++, r0-r7 \n"
      " sub     sp, -6*4 \n"

      " rets \n"
      :
      :
      :
  );

}

inline void
SCALL_contextSave(void)
{
  // push all registers to stack
  // *g_ppCurrentStack = SP (stack pointer)

  // leave interrupts as they were

  // Warning: the stack layout after SCALL doesn't match the one after an interrupt.
  // If SR[M2:M0] == 001
  //    PC and SR are on the stack.
  // Else (other modes)
  //    Nothing on the stack.

  // WARNING NOTE: the else case cannot happen as it is strictly forbidden to call
  // yield() and eventWait() in an interrupt|exception handler.

  asm volatile (

      // in order to save R0-R7
      " sub     sp, 6*4 \n"

      // Save R0..R7
      " stm     --sp, r0-r7 \n"

      // in order to save R8-R12 and LR
      // do not use SP if interrupts occurs, SP must be left at bottom of stack
      " sub     r7, sp,-16*4 \n"

      // Copy PC and SR in other places in the stack.
      " ld.w    r0, r7[-2*4] \n" // Read SR
      " st.w    r7[-8*4], r0 \n" // Copy SR

      " ld.w    r0, r7[-1*4] \n" // Read PC
      " st.w    r7[-7*4], r0 \n" // Copy PC

      // Save R8..R12 and LR on the stack.
      " stm     --r7, r8-r12, lr \n"

      // Arriving here we have the following stack organisations:
      // R8..R12, LR, PC, SR, R0..R7.
      // Now we can finalise the save.
      :
      :
      :
  );

  // Disable the its which may cause a context switch (i.e. cause a change of
  // pxCurrentTCB).

  // Basically, all accesses to the pxCurrentTCB structure should be put in a
  // critical section because it is a global structure.

  //OSScheduler::criticalEnter();

#if false

  OSSchedulerImpl::stackPointerSave();

#else
  // Store SP in the first member of the structure pointed to by pxCurrentTCB *
  asm volatile (

      " mov     r8, LO(%[pxCurrentTCB]) \n"
      " orh     r8, HI(%[pxCurrentTCB]) \n"
      " ld.w    r0, r8[0] \n"
      " st.w    r0[0], sp \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack)
      :
  );
#endif
}

inline void
SCALL_contextRestore(void)
{
  // Restore all registers
  asm volatile (

      " mov     r8, LO(%[pxCurrentTCB]) \n"
      " orh     r8, HI(%[pxCurrentTCB]) \n"
      " ld.w    r0, r8[0] \n"
      " ld.w    sp, r0[0] \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack)
      :
  );

  // Leave pxCurrentTCB variable access critical section

  //portEXIT_CRITICAL();

  asm volatile (

      // skip PC and SR

      // do not use SP if interrupts occurs, SP must be left at bottom of stack
      " sub     r7, sp, -10*4 \n"

      // Restore r8-r12 and LR
      " ldm     r7++, r8-r12, lr \n"

      // RETS will take care of the extra PC and SR restore.
      // So, we have to prepare the stack for this.
      " ld.w    r0, r7[-8*4] \n" // Read SR
      " st.w    r7[-2*4], r0 \n" // Copy SR
      " ld.w    r0, r7[-7*4] \n" // Read PC
      " st.w    r7[-1*4], r0 \n" // Copy PC

      // Restore R0..R7
      " ldm     sp++, r0-r7 \n"
      " sub     sp, -6*4 \n"

      " rets \n"
      :
      :
      :
  );
}


/*
 * Critical section management.
 *
 * To allow Real-Time interrupts to function, we do not disable al interrupts,
 * but selectively mask only non Real-Time levels.
 *
 * Push SR onto the stack and disable non Real Time interrupts.
 *
 * Notice: The function context is addressed via R07, not SP, so using the
 * stack as temporary storage should be safe.
 *
 * There were some rumors about the compiler aggressively optimising the stack,
 * leading to strange behaviours, but I cannot confirm them.
 */
inline void
OSScheduler::criticalEnter(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)
  asm volatile
  (
      " mfsr    r8, %[SR] \n"
      " st.w    --sp, r8 \n" // push value to stack
#if defined(OS_INCLUDE_OSSCHEDULER_CRITICALENTER_WITH_MASK)
      " orh     r8, %[MASK] \n" // selectively disable interrupts in MASK
      " mtsr    %[SR], r8 \n"
#else
      " ssrf    %[GM] \n"
#endif
      :
      : [MASK] "i" (0x001E),    // TODO: define a configuration macro
      [SR] "i" (AVR32_SR),
      [GM] "i" (AVR32_SR_GM_OFFSET)
      : "r8"
  );
#endif
}

/*
 * Pop SR from the stack, restore interrupts to previous status
 */
inline void
OSScheduler::criticalExit(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)
  asm volatile
  (
      " ld.w    r8, sp++ \n"
      " mtsr    %[SR], r8 \n"
      :
      : [SR] "i" (AVR32_SR)
      : "r8", "cc"              // "cc" mean configuration flags
  );
#endif
}

// perform a context switch by triggering a system call exception
inline void
OSSchedulerImpl::yield(void)
{
  asm volatile
  (
      " scall \n"
      : : :
  );
}

#endif /*HAL_FAMILY_OSSCHEDULER_INLINES_H_*/
