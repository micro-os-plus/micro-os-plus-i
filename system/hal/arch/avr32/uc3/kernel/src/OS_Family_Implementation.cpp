/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

void
os_init_data_and_bss(void) __attribute__((noinline));

void
os_init_static_constructors(void) __attribute__((noinline));

extern "C" int
main(void);

extern "C" void
os_reset_handler(void) __attribute__( ( naked ));

//int xx = 0x12345678;

void
OSImpl::CPUinit(void)
{
  // Switch to external Oscillator 0
  pm_switch_to_osc0(&AVR32_PM, OS_CFGLONG_OSCILLATOR_HZ,
      AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);

  // initialise local bus; without it GPIO does not work
  Set_system_register(AVR32_CPUCR,
      Get_system_register(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);

  // Set up EVBA so interrupts can be enabled later.
  Set_system_register(AVR32_EVBA, (int)&_evba);
}

void
os_reset_handler(void)
{
  asm volatile (
      " lda.w   sp, _estack\n"
      :::
  );

  // be sure we start with interrupts disabled
  OS::interruptsDisable();

  OSImpl::CPUinit();

  OSScheduler::ledActiveInit();
  OSScheduler::ISRledActiveOn();

  os_init_data_and_bss();

#if false
  if (xx != 0x12345678)
    {
      OSDeviceDebug::putString("dataInit() failed");
      OSDeviceDebug::putNewLine();
    }
#endif

#if defined(DEBUG)
  OSDeviceDebug::earlyInit();
#endif

  OS::earlyInit();

  os_init_static_constructors();

  main(); // call standard main()

  for (;;)
    ;
}

void
OS::saveResetBits(void)
{
  ms_resetBits = -1;
}

extern "C" void
os_scall_handler(void) __attribute__( ( naked ));

void
os_scall_handler(void)
{
  SCALLcontextSave();
  OSScheduler::contextSwitch();
  SCALLcontextRestore();
}

#endif /* OS_CONFIG_FAMILY_AVR32UC3 */

