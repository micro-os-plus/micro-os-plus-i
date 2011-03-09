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

#endif /* HAL_OS_ARCH_INLINES_H_ */
