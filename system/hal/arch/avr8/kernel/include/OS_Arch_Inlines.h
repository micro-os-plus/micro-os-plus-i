/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OS_ARCH_INLINES_H_
#define HAL_OS_ARCH_INLINES_H_

inline void
OSCPUImpl::returnFromInterrupt(void)
{
  asm volatile( "reti":: );
  for (;;)
    ; // noreturn
}

inline void
OSCPUImpl::returnFromSubroutine(void)
{
  asm volatile( "ret":: );
  for (;;)
    ; // noreturn
}

inline void
OSCPUImpl::idle(void)
{
  set_sleep_mode( SLEEP_MODE_IDLE);
  sleep_cpu();
}

inline void
OSCPUImpl::sleep(void)
{
  set_sleep_mode( SLEEP_MODE_EXT_STANDBY);
  sleep_cpu();
}

inline void
OSCPUImpl::deepSleep(void)
{
  set_sleep_mode( SLEEP_MODE_PWR_SAVE);
  sleep_cpu();
}

inline void
OSCPUImpl::watchdogReset(void)
{
  wdt_reset();
}

inline void
OSCPUImpl::nop(void)
{
  asm volatile( "nop":: );
}

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void
OSCPUImpl::softReset(void)
{
  wdt_enable( WDTO_15MS);
  cli();
  for (;;)
    ; // trigger WD
}

inline void
OSCPUImpl::interruptsEnable(void)
{
  sei();
}

inline void
OSCPUImpl::interruptsDisable(void)
{
  cli();
}

inline OSResetBits_t
OSCPUImpl::fetchResetBits(void)
{
  OSResetBits_t ret;
  ret = MCUSR;
  MCUSR = 0;
  return ret;
}

// release processor to next ready task
inline void
OSSchedulerImpl::yield(void)
{
  OSScheduler::contextSave();
    {
      OSScheduler::performContextSwitch(); // same as from ISRs, no return
    }
  OSScheduler::contextRestore();

  // For unknown reasons, interrupts must be re-enabled here
  OSCPU::interruptsEnable();
  OSCPU::returnFromSubroutine(); // necessary because it is 'naked'
}

inline void
OSCPUImpl::stackPush(OSStack_t reg)
{
  register OSStack_t tmp;

  tmp = reg;

  asm volatile
  (
      " push %[R] \n" // push value onto stack

      :
      : [R] "r" (tmp)
      :
  );
}

inline OSStack_t
OSCPUImpl::stackPop(void)
{
  register OSStack_t tmp;

  asm volatile
  (
      " pop %[R] \n"  // pop value from stack

      : [R] "=r" (tmp)
      :
      :
  );
  return tmp;
}

inline OSInterruptsMask_t
OSCPUImpl::getInterruptsMask(void)
{
  register unsigned char tmp;

  asm volatile
  (
      " in    %[R], __SREG__ \n"

      : [R] "=r" (tmp)
      :
      :
  );

  return tmp;
}

inline void
OSCPUImpl::setInterruptsMask(OSInterruptsMask_t mask)
{
  register unsigned char tmp;

  tmp = mask;

  asm volatile
  (
      " out    __SREG__, %[R] \n"

      :
      : [R] "r" (tmp)
      :
  );
}

#endif /* HAL_OS_ARCH_INLINES_H_ */
