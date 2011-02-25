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
        "		\n"
        ::
    );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::returnFromSubroutine(void)
  {
    asm volatile
    (
        " 		\n"
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
        " 		\n"
        :::
    );
  }

inline void OSImpl::interruptsDisable(void)
  {
    asm volatile
    (
        " 		\n"
        :::
    );
  }

inline void OSImpl::interruptsClearMask(void)
  {
    asm volatile
    (
        " 		\n"
        :::
    );
  }

inline void OSImpl::interruptsSetMask(void)
  {
    asm volatile
    (
        " 		\n"
        :::
    );
  }

#endif /*HAL_ARCH_OS_INLINES_H_*/
