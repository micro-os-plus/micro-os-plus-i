/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_INLINES_H_
#define HAL_FAMILY_OS_INLINES_H_

inline void OSCPUImpl::idle(void)
  {
    asm volatile(" wfi"::);

#if false
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_cpu();
#endif
  }

inline void OSCPUImpl::sleep(void)
  {
    asm volatile(" wfi"::);

#if false
    set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
    sleep_cpu();
#endif
  }

inline void OSCPUImpl::deepSleep(void)
  {
    asm volatile(" wfi"::);

#if false
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_cpu();
#endif
  }

inline void OSCPUImpl::watchdogReset(void)
  {
    //IWDG_ReloadCounter();
  }

// as per manual, JMP 0 is not recommended since
// registers are not set to their initial status
inline void OSCPUImpl::softReset(void)
  {
#if false
    wdt_enable(WDTO_15MS);
    cli();
#endif
    for (;;)
      ; // trigger WD
  }

#endif /* HAL_FAMILY_OS_INLINES_H_ */
