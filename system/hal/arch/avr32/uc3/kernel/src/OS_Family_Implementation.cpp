/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/lib/include/pm.h"

void
__init_data_and_bss(void) __attribute__((noinline));
void
__init_static_constructors(void) __attribute__((noinline));

extern "C" int
main(void);

extern "C" void
os_processor_init(void) __attribute__( ( naked ));

//int xx = 0x12345678;

void
os_processor_init(void)
{
  asm volatile (
      " lda.w   sp, _estack\n"
      :::
  );
#if true
  // Switch to external Oscillator 0
  pm_switch_to_osc0(&AVR32_PM, OS_CFGLONG_OSCILLATOR_HZ, AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
  gpio_local_init();
#endif

  // be sure we start with interrupts disabled
  OS::interruptsDisable();

  OSScheduler::ledActiveInit();
  OSScheduler::ISRledActiveOn();

#if false
  wdt_enable(WDTO_2S);

#if !defined(OS_EXCLUDE_MULTITASKING)
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
#endif
#endif

  __init_data_and_bss();

#if defined(DEBUG)
  OSDeviceDebug::earlyInit();
#endif

  OS::earlyInit();

  //__init_static_constructors();

  main(); // call standard main()

  for (;;)
    ;
}

void
OS::saveResetBits(void)
{
  ms_resetBits = -1;
}

#endif /* OS_CONFIG_FAMILY_AVR32UC3 */

