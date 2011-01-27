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
os_reset_handler(void) __attribute__( ( naked, noreturn ));

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

  // noreturn
  for (;;)
    ;
}

void
OS::saveResetBits(void)
{
  ms_resetBits = -1;
}

extern "C" void
os_ex_handle(unsigned short n);

void
os_ex_handle(unsigned short n)
{
#if defined(DEBUG)
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR("Exception = "));
  OSDeviceDebug::putDec(n);
  OSDeviceDebug::putNewLine();
#endif
  for (;;)
    ;
}

extern "C" void
os_scall_handler(void) __attribute__( ( naked ));

void
os_scall_handler(void)
{
  SCALL_contextSave();
  OSScheduler::contextSwitch();
  SCALL_contextRestore();
}

// Unrecoverable Exception.
extern "C" void
os_ex_unrecover(void) __attribute__( ( naked ));

void
os_ex_unrecover(void)
{
  os_ex_handle(3);
}

// TLB Multiple Hit.
extern "C" void
os_ex_tbl_mult_hit(void) __attribute__( ( naked ));

void
os_ex_tbl_mult_hit(void)
{
  os_ex_handle(4);
}

// Bus Error Data Fetch.
extern "C" void
os_ex_bus_err_data_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_data_fetch(void)
{
  os_ex_handle(5);
}

// Bus Error Instruction Fetch.
extern "C" void
os_ex_bus_err_inst_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_inst_fetch(void)
{
  os_ex_handle(6);
}

// NMI.
extern "C" void
os_ex_nmi(void) __attribute__( ( naked ));

void
os_ex_nmi(void)
{
  os_ex_handle(7);
}

// Instruction Address.
extern "C" void
os_ex_inst_add(void) __attribute__( ( naked ));

void
os_ex_inst_add(void)
{
  os_ex_handle(12);
}

// ITLB Protection.
extern "C" void
os_ex_itlb_prot(void) __attribute__( ( naked ));

void
os_ex_itlb_prot(void)
{
  os_ex_handle(14);
}

// Breakpoint.
extern "C" void
os_ex_brk_point(void) __attribute__( ( naked ));

void
os_ex_brk_point(void)
{
  os_ex_handle(15);
}

// Illegal Opcode.
extern "C" void
os_ex_illegal_opcode(void) __attribute__( ( naked ));

void
os_ex_illegal_opcode(void)
{
  os_ex_handle(16);
}

// Unimplemented Instruction.
extern "C" void
os_ex_unimpl_inst(void) __attribute__( ( naked ));

void
os_ex_unimpl_inst(void)
{
  os_ex_handle(17);
}

// Privilege Violation.
extern "C" void
os_ex_privilege_violaton(void) __attribute__( ( naked ));

void
os_ex_privilege_violaton(void)
{
  os_ex_handle(18);
}

// Data Address (Read).
extern "C" void
os_ex_data_rd(void) __attribute__( ( naked ));

void
os_ex_data_rd(void)
{
  os_ex_handle(22);
}

// Data Address (Write).
extern "C" void
os_ex_data_wr(void) __attribute__( ( naked ));

void
os_ex_data_wr(void)
{
  os_ex_handle(23);
}

// DTLB Protection (Read).
extern "C" void
os_ex_dtlb_protect_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_rd(void)
{
  os_ex_handle(26);
}

// DTLB Protection (Write).
extern "C" void
os_ex_dtlb_protect_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_wr(void)
{
  os_ex_handle(27);
}

// ITLB Miss.
extern "C" void
os_ex_itlb_miss(void) __attribute__( ( naked ));

void
os_ex_itlb_miss(void)
{
  os_ex_handle(13);
}

// DTLB Miss (Read).
extern "C" void
os_ex_dtlb_miss_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_rd(void)
{
  os_ex_handle(24);
}

// DTLB Miss (Write).
extern "C" void
os_ex_dtlb_miss_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_wr(void)
{
  os_ex_handle(25);
}

#endif /* OS_CONFIG_FAMILY_AVR32UC3 */

