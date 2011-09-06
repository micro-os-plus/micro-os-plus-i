/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/lib/include/conf_isp.h"

#include "hal/arch/avr32/uc3/devices/onchip/include/Pm.h"

// In order to be able to program a project with both BatchISP and JTAGICE mkII
// without having to take the general-purpose fuses into consideration, add this
// function to the project and change the program entry point to _trampoline.

// the C equivalent of OS::resetHandler()
//extern "C"  void _ZN2OS12resetHandlerEv(void) __attribute__((naked, noreturn));

extern "C" void
_trampoline(void) __attribute__((naked, noreturn, section(".reset")));

void
_trampoline(void)
{
  // This must be linked @ 0x80000000 if it is to be run upon reset.
  asm (
      " rjmp    _after_trampoline \n" // Jump after trampoline

      " .org  %[ORG] \n"
      "_after_trampoline: \n"
      " lda.w   pc, %[START] \n" // Jump to the uOS++ startup routine.

      :
      : [ORG] "i" (PROGRAM_START_OFFSET), [START] "i" (OS::resetHandler)
      :
);
  for (;;)
    ; // noreturn
}

#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"

extern unsigned long _evba;

void
OSCPUImpl::earlyInit(void)
{
  //Configure peripheral clock's
  pm_cksel(&AVR32_PM, CFG_INT_PBA_DIV, CFG_INT_PBA_SEL, CFG_INT_PBB_DIV,
      CFG_INT_PBB_SEL, CFG_INT_HSB_DIV, CFG_INT_HSB_SEL);

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
OSImpl::familyEarlyInit(void)
{
#if defined(DEBUG)

  //OSDeviceDebug::putNewLine();

#if defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL)
  OSDeviceDebug::putString("Disable All Interrupts=true");
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_USE_NESTING_LEVEL) */

#if defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS)
  OSDeviceDebug::putString("Mask=");
  OSDeviceDebug::putHex((uint16_t)((OS_CFGINT_OSCRITICALSECTION_MASK) >> 16));
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_INCLUDE_OSCRITICALSECTION_MASK_INTERRUPTS) */

#if defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS)
  OSDeviceDebug::putString("RT Mask=");
  OSDeviceDebug::putHex((uint16_t)((OS_CFGINT_OSCRITICALSECTION_MASKRT) >> 16));
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_INCLUDE_OSREALTIMECRITICALSECTION_MASK_INTERRUPTS) */

#if defined(OS_INCLUDE_OSSCHEDULER_YIELD_MASK_INTERRUPTS)
  OSDeviceDebug::putString("YIELD Mask=");
  OSDeviceDebug::putHex((uint16_t)((OS_CFGINT_OSCRITICALSECTION_MASKYIELD) >> 16));
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_INCLUDE_OSSCHEDULER_YIELD_MASK_INTERRUPTS) */

  OSDeviceDebug::putString("DID=");
  OSDeviceDebug::putHex((unsigned long) __builtin_mfdr(0));
  OSDeviceDebug::putString(" rev. ");
  OSDeviceDebug::putChar('A'+(unsigned char)(((unsigned long) __builtin_mfdr(0)) >> 28));
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("UID=");
  for (int* p = (int*) 0x80800204; p < (int*) 0x80800212; ++p)
    OSDeviceDebug::putPtr((void*)*p);
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("CPU/HSB=");
  OSDeviceDebug::putDec(avr32::uc3::Pm::getCpuClockFrequencyHz());
  OSDeviceDebug::putString(" Hz");
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("PBA=");
  OSDeviceDebug::putDec(OS_CFGLONG_PBA_FREQUENCY_HZ);
  OSDeviceDebug::putString(" Hz");
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("PBB=");
  OSDeviceDebug::putDec(OS_CFGLONG_PBB_FREQUENCY_HZ);
  OSDeviceDebug::putString(" Hz");
  OSDeviceDebug::putNewLine();

#endif /* defined(DEBUG) */
}

// common point for all exceptions
extern "C" void
os_exception_handler(unsigned short n, const char* s);

void
os_exception_handler(unsigned short n, const char* s = NULL)
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
  s = s;

#endif

  OSCPU::softReset();

#endif
}


extern "C" void
os_scall_handler(void) __attribute__((naked));

void
os_scall_handler(void)
{
#if !defined(OS_EXCLUDE_MULTITASKING)

  SCALL_contextSave();
  OSScheduler::performContextSwitch();
  SCALL_contextRestore();

#else

  os_exception_handler(61, "SCALL");

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

}

// Unrecoverable Exception.
extern "C" void
os_ex_unrecover(void) __attribute__((naked));

void
os_ex_unrecover(void)
{
  os_exception_handler(3, "Unrecoverable Exception");
}

// TLB Multiple Hit.
extern "C" void
os_ex_tbl_mult_hit(void) __attribute__((naked));

void
os_ex_tbl_mult_hit(void)
{
  os_exception_handler(4, "TLB Multiple Hit");
}

// Bus Error Data Fetch.
extern "C" void
os_ex_bus_err_data_fetch(void) __attribute__((naked));

void
os_ex_bus_err_data_fetch(void)
{
  os_exception_handler(5, "Bus Error Data Fetch");
}

// Bus Error Instruction Fetch.
extern "C" void
os_ex_bus_err_inst_fetch(void) __attribute__((naked));

void
os_ex_bus_err_inst_fetch(void)
{
  os_exception_handler(6, "Bus Error Instruction Fetch");
}

// NMI.
extern "C" void
os_ex_nmi(void) __attribute__((naked));

void
os_ex_nmi(void)
{
#if defined(OS_INCLUDE_OSAPPLICATIONIMPL_NONMASKABLEINTERRUPTHANDLER)
  OSApplicationImpl::nonMaskableInterruptHandler();
  OSCPU::returnFromInterrupt();
#else
  os_exception_handler(7, "NMI");
#endif
}

// Instruction Address.
extern "C" void
os_ex_inst_add(void) __attribute__((naked));

void
os_ex_inst_add(void)
{
  os_exception_handler(12, "Instruction Address");
}

// ITLB Protection.
extern "C" void
os_ex_itlb_prot(void) __attribute__((naked));

void
os_ex_itlb_prot(void)
{
  os_exception_handler(14, "ITLB Protection");
}

// Breakpoint.
extern "C" void
os_ex_brk_point(void) __attribute__((naked));

void
os_ex_brk_point(void)
{
  os_exception_handler(15, "Breakpoint");
}

// Illegal Opcode.
extern "C" void
os_ex_illegal_opcode(void) __attribute__((naked));

void
os_ex_illegal_opcode(void)
{
  os_exception_handler(16);
}

// Unimplemented Instruction.
extern "C" void
os_ex_unimpl_inst(void) __attribute__((naked));

void
os_ex_unimpl_inst(void)
{
  os_exception_handler(17, "Unimplemented Instruction");
}

// Privilege Violation.
extern "C" void
os_ex_privilege_violaton(void) __attribute__((naked));

void
os_ex_privilege_violaton(void)
{
  os_exception_handler(18, "Privilege Violation");
}

// Data Address (Read).
extern "C" void
os_ex_data_rd(void) __attribute__((naked));

void
os_ex_data_rd(void)
{
  os_exception_handler(22);
}

// Data Address (Write).
extern "C" void
os_ex_data_wr(void) __attribute__((naked));

void
os_ex_data_wr(void)
{
  os_exception_handler(23, "Data Address (Write)");
}

// DTLB Protection (Read).
extern "C" void
os_ex_dtlb_protect_rd(void) __attribute__((naked));

void
os_ex_dtlb_protect_rd(void)
{
  os_exception_handler(26, "DTLB Protection (Read)");
}

// DTLB Protection (Write).
extern "C" void
os_ex_dtlb_protect_wr(void) __attribute__((naked));

void
os_ex_dtlb_protect_wr(void)
{
  os_exception_handler(27, "DTLB Protection (Write)");
}

// ITLB Miss.
extern "C" void
os_ex_itlb_miss(void) __attribute__((naked));

void
os_ex_itlb_miss(void)
{
  os_exception_handler(13, "ITLB Miss");
}

// DTLB Miss (Read).
extern "C" void
os_ex_dtlb_miss_rd(void) __attribute__((naked));

void
os_ex_dtlb_miss_rd(void)
{
  os_exception_handler(24, "DTLB Miss (Read)");
}

// DTLB Miss (Write).
extern "C" void
os_ex_dtlb_miss_wr(void) __attribute__((naked));

void
os_ex_dtlb_miss_wr(void)
{
  os_exception_handler(25, "DTLB Miss (Write)");
}

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

