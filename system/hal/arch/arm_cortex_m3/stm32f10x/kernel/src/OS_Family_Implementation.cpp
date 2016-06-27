/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_STM32F10X)


void __init_data_and_bss(void) __attribute__((noinline));
void __init_static_constructors(void) __attribute__((noinline));

extern "C" int main(void);

void Reset_Handler(void)
  {
#if false
    SystemInit();
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

    //__init_data_and_bss();

#if defined(DEBUG)
    OSDeviceDebug::earlyInit();
#endif

    OS::earlyInit();

    //__init_static_constructors();

    main(); // call standard main()

    for (;;)
      ;
  }

void OS::saveResetBits(void)
  {
    ms_resetBits = -1;
  }

#endif

