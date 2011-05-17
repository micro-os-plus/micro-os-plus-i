/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

inline void
OSCPUImpl::stackInit(void)
{
  asm volatile (
      "  \n"

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
      "  \n"

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
      "  \n"

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
      "  \n"

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
      " \n"

      :
      :
      :
);
}

inline void
OSCPUImpl::interruptsDisable(void)
{
  asm volatile
  (
      "  \n"

      :
      :
      :
);
}

inline void
OSCPUImpl::idle(void)
{
  asm volatile
  (
      "  \n"

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
      "  \n" /* TODO: updated it!!! */

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
      "  \n" /* TODO: updated it!!! */

      :
      :
      :
);
}

inline void
OSCPUImpl::watchdogReset(void)
{
  ; // TODO: fill it in
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
  register OSStack_t tmp;

  tmp = reg;

  asm volatile
  (
      "  \n" // push value onto stack

      :
      : [R] "r" (tmp)
      : "sp"
);
}

inline OSStack_t
OSCPUImpl::stackPop(void)
{
  register OSStack_t tmp;

  asm volatile
  (
      "  \n" // pop value from stack

      : [R] "=r" (tmp)
      :
      : "sp"
);
  return tmp;
}

inline OSInterruptsMask_t
OSCPUImpl::getInterruptsMask(void)
{
  register unsigned int tmp;

  asm volatile
  (
      "  \n"

      : [R] "=r" (tmp)
      :
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
      "  \n"

      :
      : [R] "r" (tmp)
      :
);
}

#endif /* HAL_FAMILY_OS_INLINES_H_ */
