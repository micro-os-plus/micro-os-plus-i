/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OSSCHEDULER_INLINES_H_
#define HAL_ARCH_OSSCHEDULER_INLINES_H_

#include "hal/arch/avr32/lib/include/compiler.h"

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

void SCALLcontextSave(void) __attribute__( ( always_inline ) );
void SCALLcontextRestore(void) __attribute__( ( always_inline ) );
void AppContextRestore(void) __attribute__( ( always_inline ) );



inline void
AppContextRestore(void)
{
  /* Restore Context for cases other than INTi. */
  asm volatile (

      " mov     r8, LO(%[pxCurrentTCB]) \n"
      " orh     r8, HI(%[pxCurrentTCB]) \n"
      " ld.w    r0, r8[0] \n"
      " ld.w    sp, r0[0] \n"

      /* Restore R0..R7 */
      " ldm     sp++, r0-r7 \n"
      /* R0-R7 should not be used below this line */

      /* Skip PC and SR (will do it at the end) */
      " sub     sp, -2*4 \n"

      /* Restore R8..R12 and LR */
      " ldm     sp++, r8-r12, lr \n"

      /* Restore SR */
      " ld.w r0, sp[-8*4]\n\t" /* R0 is modified, is restored later. */
      " mtsr    %[SR], r0 \n"

      /* Restore r0 */
      " ld.w    r0, sp[-9*4] \n\t"

      /* Restore PC */
      " ld.w    pc, sp[-7*4]" /* Get PC from stack - PC is the 7th register saved */

      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack),
        [SR] "i" (AVR32_SR)
      :
  );

  /* Leave pxCurrentTCB variable access critical section */

  //portEXIT_CRITICAL();

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
SCALLcontextSave(void)
{
  // push all registers to stack
  // *g_ppCurrentStack = SP (stack pointer)
  // leave interrupts as they were

  /* Warning: the stack layout after SCALL doesn't match the one after an interrupt. */
  /* If SR[M2:M0] == 001 */
  /*    PC and SR are on the stack.  */
  /* Else (other modes) */
  /*    Nothing on the stack. */
  /* WARNING NOTE: the else case cannot happen as it is strictly forbidden to call */
  /* vTaskDelay() and vTaskDelayUntil() OS functions (that result in a taskYield()) */
  /* in an interrupt|exception handler. */

  asm volatile (

      /* in order to save R0-R7 */
      " sub sp, 6*4 \n"

      /* Save R0..R7 */
      " stm --sp, r0-r7 \n"

      /* in order to save R8-R12 and LR */
      /* do not use SP if interrupts occurs, SP must be left at bottom of stack */
      " sub r7, sp,-16*4 \n"

      /* Copy PC and SR in other places in the stack. */
      " ld.w r0, r7[-2*4] \n" /* Read SR */
      " st.w r7[-8*4], r0 \n" /* Copy SR */

      "ld.w r0, r7[-1*4] \n" /* Read PC */
      "st.w r7[-7*4], r0 \n" /* Copy PC */

      /* Save R8..R12 and LR on the stack. */
      "stm --r7, r8-r12, lr \n"

      /* Arriving here we have the following stack organizations: */
      /* R8..R12, LR, PC, SR, R0..R7. */
      /* Now we can finalize the save. */
      :
      :
      :
  );

  /* Disable the its which may cause a context switch (i.e. cause a change of */
  /* pxCurrentTCB). */

  /* Basically, all accesses to the pxCurrentTCB structure should be put in a */
  /* critical section because it is a global structure. */

  //portENTER_CRITICAL();

  /* Store SP in the first member of the structure pointed to by pxCurrentTCB */
  asm volatile (

      " mov r8, LO(%[pxCurrentTCB]) \n"
      " orh r8, HI(%[pxCurrentTCB]) \n"
      " ld.w r0, r8[0] \n"
      " st.w r0[0], sp \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack)
      :
  );
}

inline void
SCALLcontextRestore(void)
{
  /* Restore all registers */
  asm volatile (

      "mov     r8, LO(%[pxCurrentTCB]) \n"
      "orh     r8, HI(%[pxCurrentTCB]) \n"
      "ld.w    r0, r8[0] \n"
      "ld.w    sp, r0[0] \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack)
      :
  );

  /* Leave pxCurrentTCB variable access critical section */

  //portEXIT_CRITICAL();

  asm volatile (

      /* skip PC and SR */

      /* do not use SP if interrupts occurs, SP must be left at bottom of stack */
      " sub     r7, sp, -10*4 \n"
      " \n"
      /* Restore r8-r12 and LR */
      " ldm     r7++, r8-r12, lr \n"
      " \n"
      /* RETS will take care of the extra PC and SR restore. */
      /* So, we have to prepare the stack for this. */
      " ld.w    r0, r7[-8*4] \n" /* Read SR */
      " st.w    r7[-2*4], r0 \n" /* Copy SR */
      " ld.w    r0, r7[-7*4] \n" /* Read PC */
      " st.w    r7[-1*4], r0 \n" /* Copy PC */
      " \n"
      /* Restore R0..R7 */
      " ldm     sp++, r0-r7 \n"
      " sub     sp, -6*4 \n"
      " \n"
      " rets \n"
      :
      :
      :
  );

}

inline void
OSScheduler::contextSave(void)
{
  // push all registers to stack
  // *g_ppCurrentStack = SP (stack pointer)
  // leave interrupts as they were

  /* When we come here */
  /* Registers R8..R12, LR, PC and SR had already been pushed to system stack */

  asm volatile (
      /* Save R0..R7 */
      " stm     --sp, r0-r7 \n"

      /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */
      /* interrupt handler (which was of a higher priority level but decided to lower its priority */
      /* level and allow other lower interrupt level to occur). */
      /* In this case we don't want to do a task switch because we don't know what the stack */
      /* currently looks like (we don't know what the interrupted interrupt handler was doing). */

      /* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */
      /* will just be restoring the interrupt handler, no way!!! */
      /* So, since we won't do a vTaskSwitchContext(), it's of no use to save SP. */

      " ld.w    r0, sp[9*4] \n" /* Read SR in stack */
      " bfextu  r0, r0, 22, 3 \n" /* Extract the mode bits to R0. */
      " cp.w    r0, 1 \n" /* Compare the mode bits with supervisor mode(b'001) */
      " brhi    LABEL_INT_SKIP_SAVE_CONTEXT_%[LINE] \n"

      /* Store SP in the first member of the structure pointed to by pxCurrentTCB */
      /* NOTE: we don't enter a critical section here because all interrupt handlers */
      /* MUST perform a SAVE_CONTEXT/RESTORE_CONTEXT in the same way as */
      /* portSAVE_CONTEXT_OS_INT/port_RESTORE_CONTEXT_OS_INT if they call OS functions. */
      /* => all interrupt handlers must use portENTER_SWITCHING_ISR/portEXIT_SWITCHING_ISR. */

      " mov     r8, LO(%[pxCurrentTCB]) \n"
      " orh     r8, HI(%[pxCurrentTCB]) \n"
      " ld.w    r0, r8[0] \n"
      " st.w    r0[0], sp \n"

      "LABEL_INT_SKIP_SAVE_CONTEXT_%[LINE]: \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack),
      [LINE] "i" (__LINE__)
      :
  );
}

/*
 * Opposite to contextSave().
 */

inline void
OSScheduler::contextRestore(void)
{
  // SP = *g_ppCurrentStack
  // pop everything from stack

#if false
  /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */
  /* interrupt handler (which was of a higher priority level but decided to lower its priority */
  /* level and allow other lower interrupt level to occur). */

  /* In this case we don't want to do a task switch because we don't know what the stack */
  /* currently looks like (we don't know what the interrupted interrupt handler was doing). */

  /* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */
  /* will just be restoring the interrupt handler, no way!!! */

  asm volatile (

      " ld.w r0, sp[9*4] \n" /* Read SR in stack */
      " bfextu r0, r0, 22, 3 \n" /* Extract the mode bits to R0. */
      " cp.w r0, 1 \n" /* Compare the mode bits with supervisor mode(b'001) */
      " brhi LABEL_INT_SKIP_RESTORE_CONTEXT_%[LINE] \n"
      :
      : [LINE] "i" (__LINE__)
      :
  );

  /* because it is here safe, always call vTaskSwitchContext() since an OS tick occurred. */

  /* A critical section has to be used here because vTaskSwitchContext handles FreeRTOS linked lists. */

  //portENTER_CRITICAL();

  //vTaskSwitchContext();

  //portEXIT_CRITICAL();
#endif

  /* Restore all registers */
  asm volatile (

      /* Set SP to point to new stack */
      " mov r8, LO(%[pxCurrentTCB]) \n"
      " orh r8, HI(%[pxCurrentTCB]) \n"
      " ld.w r0, r8[0] \n"
      " ld.w sp, r0[0] \n"
      "LABEL_INT_SKIP_RESTORE_CONTEXT_%[LINE]: \n"

      /* Restore R0..R7 */
      " ldm sp++, r0-r7 \n"

      /* Now, the stack should be R8..R12, LR, PC and SR */
      " rete \n"
      :
      : [pxCurrentTCB] "i" (&g_ppCurrentStack),
      [LINE] "i" (__LINE__)
      :
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
inline void
OSScheduler::criticalEnter(void)
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
inline void
OSScheduler::criticalExit(void)
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

// trigger a soft interrupt
inline void
OSScheduler::yieldImpl(void)
{
  asm volatile
  (
      " scall \n"
      : : :
  );
}

#endif /*HAL_ARCH_OSSCHEDULER_INLINES_H_*/
