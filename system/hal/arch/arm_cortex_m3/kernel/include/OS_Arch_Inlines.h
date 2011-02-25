/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OS_INLINES_H_
#define HAL_ARCH_OS_INLINES_H_

inline void OSImpl::returnFromInterrupt(void)
  {
    asm volatile (
        " movw lr, #0xFFFD      \n"
        " movt lr, #0xFFFF      \n" /* return to Thread Mode, using Process Stack */
        " bx lr                 \n"
        " nop                   \n"
        ::
    );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::returnFromSubroutine(void)
  {
    asm volatile
    (
        " bx lr                 \n"
        " nop                   \n"
        ::
    );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::NOP(void)
  {
    asm volatile( " nop":: );
  }

inline void OSImpl::interruptsEnable(void)
  {
    asm volatile
    (
        " cpsie i       \n"
        :::
    );
  }

inline void OSImpl::interruptsDisable(void)
  {
    asm volatile
    (
        " cpsid i       \n"
        :::
    );
  }

inline void OSImpl::interruptsClearMask(void)
  {
    asm volatile
    (
        " mov r0, #0            \n"
        " msr basepri, r0       \n"
        :::"r0"
    );
  }

inline void OSImpl::interruptsSetMask(void)
  {
    asm volatile
    (
        " mov r0, %0            \n"
        " msr basepri, r0       \n"
        ::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY):"r0"
    );
  }

#endif /*HAL_ARCH_OS_INLINES_H_*/
