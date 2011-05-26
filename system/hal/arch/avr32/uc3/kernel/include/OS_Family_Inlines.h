/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

#include "hal/arch/avr32/uc3/devices/onchip/include/Common.h"

#if OS_CFGINT_PBA_PRESCALLER_SEL == 0
#define CFG_INT_PBA_DIV          0
#define CFG_INT_PBA_SEL          0
#else
#define CFG_INT_PBA_DIV          1
#define CFG_INT_PBA_SEL          (OS_CFGINT_PBA_PRESCALLER_SEL - 1)
#endif

#if OS_CFGINT_PBB_PRESCALLER_SEL == 0
#define CFG_INT_PBB_DIV          0
#define CFG_INT_PBB_SEL          0
#else
#define CFG_INT_PBB_DIV          1
#define CFG_INT_PBB_SEL          (OS_CFGINT_PBB_PRESCALLER_SEL - 1)
#endif

#if OS_CFGINT_HSB_PRESCALLER_SEL == 0
#define CFG_INT_HSB_DIV          0
#define CFG_INT_HSB_SEL          0
#else
#define CFG_INT_HSB_DIV          1
#define CFG_INT_HSB_SEL          (OS_CFGINT_HSB_PRESCALLER_SEL - 1)
#endif

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
      " sleep 0 \n" /* TODO: updated it!!! */

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
      " sleep 0 \n" /* TODO: updated it!!! */

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
  register OSStack_t tmp; // asm("r8");

  tmp = reg;

  asm volatile
  (
      " st.w    --sp, %[R] \n" // push value onto stack

      :
      : [R] "r" (tmp)
      : "sp"
);
}

inline OSStack_t
OSCPUImpl::stackPop(void)
{
  register OSStack_t tmp; // asm("r8");

  asm volatile
  (
      " ld.w    %[R], sp++ \n" // pop value from stack

      : [R] "=r" (tmp)
      :
      : "sp"
);
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

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP00_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP00_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP00_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP01_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP01_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP01_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP02_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP02_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP02_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP03_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP03_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP03_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP04_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP04_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP04_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP05_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP05_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP05_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP06_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP06_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP06_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP07_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP07_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP07_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP08_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP08_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP08_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP09_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP09_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP09_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP10_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP10_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP10_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP11_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP11_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP11_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP12_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP12_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP12_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP13_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP13_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP13_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP14_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP14_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP14_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP15_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP15_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP15_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP16_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP16_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP16_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP17_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP17_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP17_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP18_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP18_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP18_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP19_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP19_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP19_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP20_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP20_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP20_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP21_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP21_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP21_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP22_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP22_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP22_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP23_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP23_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP23_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP24_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP24_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP24_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP25_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP25_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP25_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP26_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP26_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP26_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP27_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP27_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP27_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP28_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP28_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP28_PRIORITY) */

#if !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP29_PRIORITY)
#define OS_CFGINT_AVR32_UC3_INTC_GROUP29_PRIORITY (avr32::uc3::intc::PRIORITY_0)
#endif /* !defined(OS_CFGINT_AVR32_UC3_INTC_GROUP29_PRIORITY) */

#endif /* HAL_FAMILY_OS_INLINES_H_ */
