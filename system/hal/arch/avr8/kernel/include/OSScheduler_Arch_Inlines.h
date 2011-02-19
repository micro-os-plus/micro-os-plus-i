/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSSCHEDULER_ARCH_INLINES_H_
#define HAL_OSSCHEDULER_ARCH_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Code to save the current task 'context' on the task control block.
 *
 * For simplicity, all general purpose registers are pushed onto the current
 * task stack and only the new stack pointer is saved in the task control block
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
 * current context (i.e. the OSTask::m_pStack, where the pointer to the
 * current stack frame is stored). This variable is set during the context
 * switch in OSScheduler::contextSwitch().
 */

inline void OSScheduler::contextSave(void)
  {
    // push all registers to stack
    // *g_pCurrentContext = SP (stack pointer)
    // leave interrupts disabled
    asm volatile(
        "push	r0			\n\t"
        "in	r0, __SREG__		\n\t"
        "cli				\n\t"
        "push	r0			\n\t"
        "push	r1			\n\t"
        "clr	r1			\n\t"
        "push	r2			\n\t"
        "push	r3			\n\t"
        "push	r4			\n\t"
        "push	r5			\n\t"
        "push	r6			\n\t"
        "push	r7			\n\t"
        "push	r8			\n\t"
        "push	r9			\n\t"
        "push	r10			\n\t"
        "push	r11			\n\t"
        "push	r12			\n\t"
        "push	r13			\n\t"
        "push	r14			\n\t"
        "push	r15			\n\t"
        "push	r16			\n\t"
        "push	r17			\n\t"
        "push	r18			\n\t"
        "push	r19			\n\t"
        "push	r20			\n\t"
        "push	r21			\n\t"
        "push	r22			\n\t"
        "push	r23			\n\t"
        "push	r24			\n\t"
        "push	r25			\n\t"
        "push	r26			\n\t"
        "push	r27			\n\t"
        "push	r28			\n\t"
        "push	r29			\n\t"
        "push	r30			\n\t"
        "push	r31			\n\t"
        "lds	r26, g_ppCurrentStack	\n\t"
        "lds	r27, g_ppCurrentStack+1 \n\t"
        "in	r0, __SP_L__		\n\t"
        "st	x+, r0			\n\t"
        "in	r0, __SP_H__		\n\t"
        "st	x+, r0			\n\t":: );
  }

/*
 * Opposite to contextSave().
 *
 * Interrupts have been disabled during the context save so we
 * can write to the stack pointer without problems.
 */

inline void OSScheduler::contextRestore(void)
  {
    // SP = *g_pCurrentContext
    // pop everything from stack
    // restore SREG according to new context
    asm volatile(
        "lds	r26, g_ppCurrentStack	\n\t"
        "lds	r27, g_ppCurrentStack+1 \n\t"
        "ld	r28, x+			\n\t"
        "out	__SP_L__, r28		\n\t"
        "ld	r29, x+			\n\t"
        "out	__SP_H__, r29		\n\t"
        "pop	r31			\n\t"
        "pop	r30			\n\t"
        "pop	r29			\n\t"
        "pop	r28			\n\t"
        "pop	r27			\n\t"
        "pop	r26			\n\t"
        "pop	r25			\n\t"
        "pop	r24			\n\t"
        "pop	r23			\n\t"
        "pop	r22			\n\t"
        "pop	r21			\n\t"
        "pop	r20			\n\t"
        "pop	r19			\n\t"
        "pop	r18			\n\t"
        "pop	r17			\n\t"
        "pop	r16			\n\t"
        "pop	r15			\n\t"
        "pop	r14			\n\t"
        "pop	r13			\n\t"
        "pop	r12			\n\t"
        "pop	r11			\n\t"
        "pop	r10			\n\t"
        "pop	r9			\n\t"
        "pop	r8			\n\t"
        "pop	r7			\n\t"
        "pop	r6			\n\t"
        "pop	r5			\n\t"
        "pop	r4			\n\t"
        "pop	r3			\n\t"
        "pop	r2			\n\t"
        "pop	r1			\n\t"
        "pop	r0			\n\t"
        "out	__SREG__, r0		\n\t"
        "pop	r0			\n\t":: );
  }

/*
 * Critical section management.
 *
 * Push SREG onto the stack and disable interrupts.
 *
 * Notice: The function context is addressed via Y, not SP, so using the
 * stack as temporary storage should be safe.
 */
inline void OSScheduler::criticalEnter(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)
    asm volatile(
        "in	__tmp_reg__, __SREG__       \n\t"
        "cli                                \n\t"
        "push   __tmp_reg__                 \n\t"::);
#endif
  }

/*
 * Pop SREG from the stack, restore interrupts to previous status
 */
inline void OSScheduler::criticalExit(void)
  {
#if !defined(OS_EXCLUDE_MULTITASKING)
    asm volatile(
        "pop	__tmp_reg__                \n\t"
        "out     __SREG__, __tmp_reg__     \n\t":: );
#endif
  }

#if false
/*
 * Manage the activity LED
 */
inline void OSScheduler::ledActiveInit(void)
  {
	/* Output */
	OS_CONFIG_ACTIVE_LED_PORT_INIT |= _BV(OS_CONFIG_ACTIVE_LED_BIT);
  }

/* Turn LED on (on interrupts) */
inline void OSScheduler::ISRledActiveOn(void)
  {
    OS_CONFIG_ACTIVE_LED_PORT |= _BV(OS_CONFIG_ACTIVE_LED_BIT);
  }

inline void OSScheduler::ledActiveOn(void)
  {
    OSScheduler::criticalEnter();
      {
        ISRledActiveOn();
      }
    OSScheduler::criticalExit();
  }

/* Turn LED off (on interrupts) */
inline void OSScheduler::ISRledActiveOff(void)
  {
    OS_CONFIG_ACTIVE_LED_PORT &= ~_BV(OS_CONFIG_ACTIVE_LED_BIT);
  }

/* Turn LED off (at sleep) */
inline void OSScheduler::ledActiveOff(void)
  {
    OSScheduler::criticalEnter();
      {
        ISRledActiveOff();
      }
    OSScheduler::criticalExit();
  }
#endif

#endif /*HAL_OSSCHEDULER_ARCH_INLINES_H_*/
