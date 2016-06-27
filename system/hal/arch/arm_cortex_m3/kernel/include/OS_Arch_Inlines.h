/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OS_INLINES_H_
#define HAL_ARCH_OS_INLINES_H_

inline void OSCPUImpl::returnFromInterrupt(void)
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

inline void OSCPUImpl::returnFromSubroutine(void)
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

inline void OSCPUImpl::nop(void)
  {
    asm volatile(" nop"::);
  }

inline void OSCPUImpl::interruptsEnable(void)
  {
    asm volatile
    (
        " cpsie i       \n"
        :::
  );
  }

inline void OSCPUImpl::interruptsDisable(void)
  {
    asm volatile
    (
        " cpsid i       \n"
        :::
  );
  }

#if 0
inline void OSCPUImpl::interruptsClearMask(void)
  {
    asm volatile
    (
        " mov r0, #0            \n"
        " msr basepri, r0       \n"
        :::"r0"
  );
  }
#endif

inline void
OSCPUImpl::stackInit(void)
{

}


#endif /* HAL_ARCH_OS_INLINES_H_ */
