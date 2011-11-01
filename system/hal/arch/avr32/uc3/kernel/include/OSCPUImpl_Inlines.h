/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSCPUIMPL_INLINES_H_
#define HAL_FAMILY_OSCPUIMPL_INLINES_H_

#include <avr32/io.h>

inline void
OSCPUImpl::stackInit(void)
{
  asm volatile (
      " lda.w   sp, _estack \n"

      :
      :
      :
);
}

inline OSResetBits_t
OSCPUImpl::fetchResetBits(void)
{
  return -1; // TODO: fetch bits telling reset reason.
}

inline void
OSCPUImpl::returnFromInterrupt(void)
{
  asm volatile (
      " rete \n"

      :
      :
      :
);
  for (;;)
    ; // noreturn
}

inline void
OSCPUImpl::returnFromSubroutine(void)
{
  asm volatile
  (
      " mov pc, lr \n"

      :
      :
      :
);
  for (;;)
    ; // noreturn
}

inline void
OSCPUImpl::nop(void)
{
  asm volatile
  (
      " nop \n"

      :
      :
      :
);
}

inline void
OSCPUImpl::interruptsEnable(void)
{
  asm volatile
  (
      " csrf    %[GM] \n"

      :
      : [GM] "i" (AVR32_SR_GM_OFFSET)
      :
);
}

inline void
OSCPUImpl::interruptsDisable(void)
{
  asm volatile
  (
      " ssrf    %[GM] \n"

      :
      : [GM] "i" (AVR32_SR_GM_OFFSET)
      :
);
}

inline void
OSCPUImpl::idle(void)
{
  asm volatile
  (
      " sleep 0 \n"

      :
      :
      :
);
}

inline void
OSCPUImpl::sleep(void)
{
  asm volatile
  (
      " sleep 2 \n" // Standby, OSC & PLL still running

      :
      :
      :
);
}

inline void
OSCPUImpl::deepSleep(void)
{
  asm volatile
  (
      " sleep 3 \n" // Stop, only OSC32 & BOD

      :
      :
      :
);
}

inline void
OSCPUImpl::watchdogReset(void)
{
  ; // TODO: fill it in
  asm volatile
  (
      " nop \n"

      :
      :
      :
);
}

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void
OSCPUImpl::softReset(void)
{
#if false
  wdt_enable(WDTO_15MS);
  cli();
#endif
  for (;;)
    ; // trigger WD
}

inline void
OSCPUImpl::stackPush(OSStack_t reg)
{
  register OSStack_t tmp; // asm("r8");

  tmp = reg;
#if true
  asm volatile
  (
      " st.w    --sp, %[R] \n" // push value onto stack

      :
      : [R] "r" (tmp)
      : "sp"
);
#else
  *((OSStack_t*) __builtin_alloca(1)) = tmp;
#endif
}

inline OSStack_t
OSCPUImpl::stackPop(void)
{
  register OSStack_t tmp; // asm("r8");

#if true
  asm volatile
  (
      " ld.w    %[R], sp++ \n" // pop value from stack

      : [R] "=r" (tmp)
      :
      : "sp"
);
#else
  asm volatile
  (
      " ld.w    %[R], sp[0] \n" // pop value from stack

      : [R] "=r" (tmp)
      :
      : "sp"
);
#endif
  return tmp;
}

inline OSInterruptsMask_t
OSCPUImpl::getInterruptsMask(void)
{
  register unsigned int tmp; //asm("r8");

  asm volatile
  (
      " mfsr    %[R], %[SR] \n"

      : [R] "=r" (tmp)
      : [SR] "i" (AVR32_SR)
      :
);

  return tmp;
}

inline void
OSCPUImpl::setInterruptsMask(OSInterruptsMask_t mask)
{
  register unsigned int tmp; //asm("r8");

  tmp = mask;

  asm volatile
  (
      " mtsr    %[SR], %[R] \n"

      :
      : [SR] "i" (AVR32_SR), [R] "r" (tmp)
      :
);
}

#endif
