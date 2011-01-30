/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

// common point for all exceptions
extern "C" void
os_exception_handler(unsigned short n);

void
os_exception_handler(unsigned short n)
{
#if defined(DEBUG)
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString("Exception = ");
  OSDeviceDebug::putDec(n);
  OSDeviceDebug::putNewLine();
#endif
  for (;;)
    ;

  // TODO: on release, perform a soft reset
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
  os_exception_handler(3);
}

// TLB Multiple Hit.
extern "C" void
os_ex_tbl_mult_hit(void) __attribute__( ( naked ));

void
os_ex_tbl_mult_hit(void)
{
  os_exception_handler(4);
}

// Bus Error Data Fetch.
extern "C" void
os_ex_bus_err_data_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_data_fetch(void)
{
  os_exception_handler(5);
}

// Bus Error Instruction Fetch.
extern "C" void
os_ex_bus_err_inst_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_inst_fetch(void)
{
  os_exception_handler(6);
}

// NMI.
extern "C" void
os_ex_nmi(void) __attribute__( ( naked ));

void
os_ex_nmi(void)
{
  os_exception_handler(7);
}

// Instruction Address.
extern "C" void
os_ex_inst_add(void) __attribute__( ( naked ));

void
os_ex_inst_add(void)
{
  os_exception_handler(12);
}

// ITLB Protection.
extern "C" void
os_ex_itlb_prot(void) __attribute__( ( naked ));

void
os_ex_itlb_prot(void)
{
  os_exception_handler(14);
}

// Breakpoint.
extern "C" void
os_ex_brk_point(void) __attribute__( ( naked ));

void
os_ex_brk_point(void)
{
  os_exception_handler(15);
}

// Illegal Opcode.
extern "C" void
os_ex_illegal_opcode(void) __attribute__( ( naked ));

void
os_ex_illegal_opcode(void)
{
  os_exception_handler(16);
}

// Unimplemented Instruction.
extern "C" void
os_ex_unimpl_inst(void) __attribute__( ( naked ));

void
os_ex_unimpl_inst(void)
{
  os_exception_handler(17);
}

// Privilege Violation.
extern "C" void
os_ex_privilege_violaton(void) __attribute__( ( naked ));

void
os_ex_privilege_violaton(void)
{
  os_exception_handler(18);
}

// Data Address (Read).
extern "C" void
os_ex_data_rd(void) __attribute__( ( naked ));

void
os_ex_data_rd(void)
{
  os_exception_handler(22);
}

// Data Address (Write).
extern "C" void
os_ex_data_wr(void) __attribute__( ( naked ));

void
os_ex_data_wr(void)
{
  os_exception_handler(23);
}

// DTLB Protection (Read).
extern "C" void
os_ex_dtlb_protect_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_rd(void)
{
  os_exception_handler(26);
}

// DTLB Protection (Write).
extern "C" void
os_ex_dtlb_protect_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_wr(void)
{
  os_exception_handler(27);
}

// ITLB Miss.
extern "C" void
os_ex_itlb_miss(void) __attribute__( ( naked ));

void
os_ex_itlb_miss(void)
{
  os_exception_handler(13);
}

// DTLB Miss (Read).
extern "C" void
os_ex_dtlb_miss_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_rd(void)
{
  os_exception_handler(24);
}

// DTLB Miss (Write).
extern "C" void
os_ex_dtlb_miss_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_wr(void)
{
  os_exception_handler(25);
}

#endif /* OS_CONFIG_FAMILY_AVR32UC3 */

