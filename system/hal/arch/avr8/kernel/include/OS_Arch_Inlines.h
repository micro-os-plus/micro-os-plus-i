/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OS_ARCH_INLINES_H_
#define HAL_OS_ARCH_INLINES_H_

inline void OSImpl::returnFromInterrupt(void)
  {
    asm volatile( "reti":: );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::returnFromSubroutine(void)
  {
    asm volatile( "ret":: );
    for (;;)
      ; // noreturn
  }

inline void OSImpl::CPUidle(void)
  {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_cpu();
  }

inline void OSImpl::CPUsleep(void)
  {
    set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
    sleep_cpu();
  }

inline void OSImpl::CPUdeepSleep(void)
  {
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_cpu();
  }

inline void OSImpl::WDTreset(void)
  {
    wdt_reset();
  }

inline void OSImpl::NOP(void)
  {
    asm volatile( "nop":: );
  }

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void OSImpl::SOFTreset(void)
  {
    wdt_enable(WDTO_15MS);
    cli();
    for (;;)
      ; // trigger WD
  }

inline void OSImpl::interruptsEnable(void)
  {
    sei();
  }

inline void OSImpl::interruptsDisable(void)
  {
    cli();
  }

// release processor to next ready task
inline void OSScheduler::yieldImpl(void)
  {
    OSScheduler::contextSave();
      {
        contextSwitch(); // same as from ISRs, no return
      }
    OSScheduler::contextRestore();
    OS::interruptsEnable(); // be sure interrupts remain enabled
    OS::returnFromSubroutine(); // necessary because it is 'naked'
  }


#endif /*HAL_OS_ARCH_INLINES_H_*/
