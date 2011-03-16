/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

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
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

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

extern unsigned long _evba;

inline void
OSCPUImpl::earlyInit(void)
{
  //Configure peripheral clock's
  pm_cksel(&AVR32_PM, CFG_INT_PBA_DIV, CFG_INT_PBA_SEL, CFG_INT_PBB_DIV,
      CFG_INT_PBB_SEL, CFG_INT_HSB_DIV, CFG_INT_HSB_SEL);

  // Switch to external Oscillator 0
  pm_switch_to_osc0(&AVR32_PM, OS_CFGLONG_OSCILLATOR_HZ,
      AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);

  // initialise local bus; without it GPIO does not work
  Set_system_register(AVR32_CPUCR,
      Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);

  // Set up EVBA so interrupts can be enabled later.
  Set_system_register(AVR32_EVBA, (int)&_evba);
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
OSCPUImpl::interruptsClearMask(void)
{
  ; // TODO: implement it
}

#if false
inline void
OSCPUImpl::interruptsSetMask(void)
{
  ; // TODO: implement it
}
#endif

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
      " ld.w    %[R], sp++ \n"  // pop value from stack

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

#endif /* HAL_FAMILY_OS_INLINES_H_ */
