/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_ARCH_OS_INLINES_H_
#define HAL_ARCH_OS_INLINES_H_

#include "hal/arch/avr32/lib/include/compiler.h"

inline void OSImpl::returnFromInterrupt(void)
  {
    asm volatile (
        "rete		\n"
        ::
    );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::returnFromSubroutine(void)
  {
    asm volatile
    (
        "mov pc, lr	\n"
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
  Enable_global_interrupt();
/*    asm volatile
    (
        " 		\n"
        :::
    );*/
  }

inline void OSImpl::interruptsDisable(void)
  {
  Disable_global_interrupt();
/*    asm volatile
    (
        " 		\n"
        :::
    );*/
  }

inline void OSImpl::interruptsClearMask(void)
  {
  Enable_global_interrupt();
/*    asm volatile
    (
        " 		\n"
        :::
    );*/
  }

inline void OSImpl::interruptsSetMask(void)
  {
  Disable_global_interrupt();
   /* asm volatile
    (
        " 		\n"
        :::
    );*/
  }

#endif /*HAL_ARCH_OS_INLINES_H_*/
