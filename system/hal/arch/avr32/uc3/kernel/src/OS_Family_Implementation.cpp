/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

// common point for all exceptions
extern "C" void
os_exception_handler(unsigned short n, const char *s);

void
os_exception_handler(unsigned short n, const char *s = NULL)
{
#if defined(DEBUG)

  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString("Exception ");
  OSDeviceDebug::putDec(n);
  OSDeviceDebug::putString("=");
  OSDeviceDebug::putString(s);
  OSDeviceDebug::putNewLine();

  for (;;)
  ;

#else

#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_LOGEXCEPTIONDETAILS)

  // Allow the application to log the exception type
  OSApplicationImpl::logExceptionDetails(n);

#else

  n = n; // avoid 'unused parameter' warning

#endif

  OSImpl::SOFTreset();

#endif
}

extern "C" void
os_scall_handler(void) __attribute__( ( naked ));

void
os_scall_handler(void)
{
  SCALL_contextSave();
  OSScheduler::performContextSwitch();
  SCALL_contextRestore();
}

// Unrecoverable Exception.
extern "C" void
os_ex_unrecover(void) __attribute__( ( naked ));

void
os_ex_unrecover(void)
{
  os_exception_handler(3, "Unrecoverable Exception");
}

// TLB Multiple Hit.
extern "C" void
os_ex_tbl_mult_hit(void) __attribute__( ( naked ));

void
os_ex_tbl_mult_hit(void)
{
  os_exception_handler(4, "TLB Multiple Hit");
}

// Bus Error Data Fetch.
extern "C" void
os_ex_bus_err_data_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_data_fetch(void)
{
  os_exception_handler(5, "Bus Error Data Fetch");
}

// Bus Error Instruction Fetch.
extern "C" void
os_ex_bus_err_inst_fetch(void) __attribute__( ( naked ));

void
os_ex_bus_err_inst_fetch(void)
{
  os_exception_handler(6, "Bus Error Instruction Fetch");
}

// NMI.
extern "C" void
os_ex_nmi(void) __attribute__( ( naked ));

void
os_ex_nmi(void)
{
  os_exception_handler(7, "NMI");
}

// Instruction Address.
extern "C" void
os_ex_inst_add(void) __attribute__( ( naked ));

void
os_ex_inst_add(void)
{
  os_exception_handler(12, "Instruction Address");
}

// ITLB Protection.
extern "C" void
os_ex_itlb_prot(void) __attribute__( ( naked ));

void
os_ex_itlb_prot(void)
{
  os_exception_handler(14, "ITLB Protection");
}

// Breakpoint.
extern "C" void
os_ex_brk_point(void) __attribute__( ( naked ));

void
os_ex_brk_point(void)
{
  os_exception_handler(15, "Breakpoint");
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
  os_exception_handler(17, "Unimplemented Instruction");
}

// Privilege Violation.
extern "C" void
os_ex_privilege_violaton(void) __attribute__( ( naked ));

void
os_ex_privilege_violaton(void)
{
  os_exception_handler(18, "Privilege Violation");
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
  os_exception_handler(23, "Data Address (Write)");
}

// DTLB Protection (Read).
extern "C" void
os_ex_dtlb_protect_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_rd(void)
{
  os_exception_handler(26, "DTLB Protection (Read)");
}

// DTLB Protection (Write).
extern "C" void
os_ex_dtlb_protect_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_protect_wr(void)
{
  os_exception_handler(27, "DTLB Protection (Write)");
}

// ITLB Miss.
extern "C" void
os_ex_itlb_miss(void) __attribute__( ( naked ));

void
os_ex_itlb_miss(void)
{
  os_exception_handler(13, "ITLB Miss");
}

// DTLB Miss (Read).
extern "C" void
os_ex_dtlb_miss_rd(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_rd(void)
{
  os_exception_handler(24, "DTLB Miss (Read)");
}

// DTLB Miss (Write).
extern "C" void
os_ex_dtlb_miss_wr(void) __attribute__( ( naked ));

void
os_ex_dtlb_miss_wr(void)
{
  os_exception_handler(25, "DTLB Miss (Write)");
}

#endif /* OS_CONFIG_FAMILY_AVR32UC3 */

