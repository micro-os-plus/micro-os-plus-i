/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

// ----- Static Data ----------------------------------------------------------

OSResetBits_t OSCPU::ms_resetBits;

//OSRealTimeCriticalSection OSRealTime::critical;

// ----------------------------------------------------------------------------

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)

OS::OS()
{
  OSDeviceDebug::putConstructor_P(PSTR("OS"), this);
}

OSCPU::OSCPU()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSCPU"), this);
}

#endif /* defined(DEBUG) */

void
OSCPU::saveResetBits(void)
{
  ms_resetBits = fetchResetBits();
}

extern "C" int
main(void) __attribute__((weak));

#if !defined(OS_EXCLUDE_RESET_HANDLER)

#define OS_CFGLONG_CONSTANT_MARKER (0x12345678)
#if defined(DEBUG)
uint_t g_constantMarker = OS_CFGLONG_CONSTANT_MARKER;
#endif /* defined(DEBUG) */

// init value for the stack pointer. defined in linker script
extern unsigned long __stack_end;

// start/end addresses for the .text section.
// defined in linker script
extern unsigned long __os_text_begin;
extern unsigned long __os_text_end;

// start/end addresses for the initialization values of the .data section.
// defined in linker script
extern unsigned long __os_data_load_begin;
extern unsigned long __os_data_load_end;

// start/end address for the .data section. defined in linker script
extern unsigned long __os_data_begin;
extern unsigned long __os_data_end;

// start/end address for the .bss section. defined in linker script
extern unsigned long __os_bss_begin;
extern unsigned long __os_bss_end;

// start/end address for the constructors section. defined in linker script
extern unsigned long __os_ctors_array_begin;
extern unsigned long __os_ctors_array_end;

void
OS::resetHandler(void)
{
  // Mandatory in the top position, before any C call
  OSCPU::stackInit();
  // from this point on, C calls are available

  // WARNING: no debug outputs before OSDeviceDebug::earlyInit()

  // Be sure we start with interrupts disabled
  OSCPU::interruptsDisable();

  // earlyInit() should also init GPIO, so that leds will be available
  OSCPU::earlyInit();

#if !defined(OS_EXCLUDE_MULTITASKING)

  OSScheduler::ledActiveInit();
  OSScheduler::ISR_ledActiveOn();

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

  dataInit();
  bssInit();

#if defined(DEBUG)
  // WARNING: No debug output before this point!
  OSDeviceDebug::earlyInit();
#endif /* defined(DEBUG) */

#if defined(DEBUG) && defined(OS_DEBUG_INIT_SECTIONS)

  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR(".text "));
  OSDeviceDebug::putPtr(&__os_text_begin);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putPtr(&__os_text_end);
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString_P(PSTR(".data "));
  OSDeviceDebug::putPtr(&__os_data_begin);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putPtr(&__os_data_end);
  OSDeviceDebug::putString_P(PSTR(" ("));
  OSDeviceDebug::putPtr(&__os_data_load_begin);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putPtr(&__os_data_load_end);
  OSDeviceDebug::putString_P(PSTR(")"));
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString_P(PSTR(".bss "));
  OSDeviceDebug::putPtr(&__os_bss_begin);
  OSDeviceDebug::putChar('-');
  OSDeviceDebug::putPtr(&__os_bss_end);
  OSDeviceDebug::putNewLine();

#endif

#if defined(DEBUG)
  if (g_constantMarker != OS_CFGLONG_CONSTANT_MARKER)
    {
      OSDeviceDebug::putString_P(PSTR("dataInit() failed "));
      OSDeviceDebug::putHex(g_constantMarker);
      OSDeviceDebug::putNewLine();
    }
#endif /* defined(DEBUG) */

  // will call OSScheduler::earlyInit() to init registered threads count
  OS::earlyInit();

  OS::staticConstructorsInit();

#if !defined(OS_EXCLUDE_MULTITASKING)

  // Threads were created and registered by class constructors
  // We're ready to rock...
  OSScheduler::start();

#else /* defined(OS_EXCLUDE_MULTITASKING)*/

#if defined(DEBUG)
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR("OS::main()"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

  OS::main();

  // Should never get here.
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("OS::main() failed, loop"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

#endif /* OS_EXCLUDE_MULTITASKING */

  // noreturn
  for (;;)
    ;
}

void
OS::dataInit(void)
{
  unsigned long* pSrc;
  unsigned long* pDest;

  /* Copy the data segment initialisers from flash to SRAM */
  pSrc = &__os_data_load_begin;
  pDest = &__os_data_begin;
  for (; pDest < &__os_data_end;)
    {
      *(pDest++) = *(pSrc++);
    }
}

void
OS::bssInit(void)
{
  unsigned long* pDest;

  /* Zero fill the bss segment. */
  for (pDest = &__os_bss_begin; pDest < &__os_bss_end;)
    {
      *(pDest++) = 0;
    }
}

void
OS::staticConstructorsInit(void)
{
  typedef void
  (*pFunc_t)(void);

  pFunc_t* p;

#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR("Constructors:"));
  OSDeviceDebug::putNewLine();
#endif

  p = (pFunc_t*) &__os_ctors_array_begin;
  for (; p < (pFunc_t*) &__os_ctors_array_end; ++p)
    {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS) && defined(OS_DEBUG_CONSTRUCTORS_INIT)
      OSDeviceDebug::putString_P(PSTR(".ctor "));
      OSDeviceDebug::putPtr((void*)*p);
      OSDeviceDebug::putString_P(PSTR(" @"));
      OSDeviceDebug::putPtr(p);
      OSDeviceDebug::putNewLine();
#endif
      (**p)();
    }
}

#endif /* OS_EXCLUDE_RESET_HANDLER */

#if defined(OS_INCLUDE_NAKED_INIT)

// ----- init5 ----------------------------------------------------------------

void OS::nakedEarlyInit(void)
  {
    earlyInit();
  }
#endif

// runs before constructors
void
OS::earlyInit(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString_P(PSTR("OS::earlyInit()"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

#if defined(DEBUG) && defined(OS_EXCLUDE_MULTITASKING)
  OSDeviceDebug::putString_P(PSTR("Multithreading: disabled"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_EXCLUDE_MULTITASKING) */

#if defined(DEBUG) && defined(OS_EXCLUDE_PREEMPTION)
  OSDeviceDebug::putString_P(PSTR("Preemption: disabled"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_EXCLUDE_PREEMPTION) */

#if defined(DEBUG) && defined(OS_EXCLUDE_OSTIMER)
  OSDeviceDebug::putString_P(PSTR("SysTick: disabled"));
  OSDeviceDebug::putNewLine();
#else
#if defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION)
  OSDeviceDebug::putString_P(PSTR("SysTick: not naked"));
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_EXCLUDE_OSTIMERTICKS_ISR_PREEMPTION) */

  OSDeviceDebug::putString_P(PSTR("SysTick="));
  OSDeviceDebug::putDec((unsigned short) OS_CFGINT_TICK_RATE_HZ);
  OSDeviceDebug::putString_P(PSTR(" ticks/sec"));
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_EXCLUDE_OSTIMER) */

  OSCPU::saveResetBits();

#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("ResetBits="));
  OSDeviceDebug::putHex((unsigned char) OSCPU::getResetBits());
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString_P(PSTR("Oscillator="));
#if defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
  OSDeviceDebug::putDec(OS_CFGLONG_OSCILLATOR_HZ);
  OSDeviceDebug::putString_P(PSTR(" Hz"));
#else
  OSDeviceDebug::putDec(OS_CFGLONG_OSCILLATOR_HZ/1000);
  OSDeviceDebug::putString_P(PSTR(" KHz"));
#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG) */
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

  OS::familyEarlyInit();

#if !defined(OS_EXCLUDE_MULTITASKING)
  OSScheduler::earlyInit();
#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
}

#if defined(OS_INCLUDE_OS_BUSYWAITMILLIS)

void
OS::busyWaitMillis(uint_t n)
{
  for (; n--;)
    {
      int i;
      // calibrate from OSC
      for (i = (OS_CFGLONG_CPU_FREQUENCY_HZ / 10000
          * OS_CFGINT_BUSYWAIT_CALIBRATION / 10000); i--;)
        {
          OSCPU::nop();
        }
    }
}

#endif /* OS_INCLUDE_OS_BUSYWAITMILLIS */

#if defined(OS_INCLUDE_OS_BUSYWAITMICROS)

void
OS::busyWaitMicros(uint_t n)
{
  int i;
  // calibrate from OSC
  for (i = (OS_CFGLONG_CPU_FREQUENCY_HZ / 10000
      * OS_CFGINT_BUSYWAIT_CALIBRATION / 10000 * n / 1000); i--;)
    {
      OSCPU::nop();
    }
}

#endif /* OS_INCLUDE_OS_BUSYWAITMICROS */

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_NEW_DELETE)

#warning "malloc() and free() are not thread safe!"

void* operator
new(std::size_t n)
  {
    return malloc(n);
  }

void operator
delete(void* p)
  {
    return free(p);
  }

#else

void
operator
delete(void* p __attribute__((unused)))
{
#if defined(DEBUG)
  ; // dummy, refered by virtual destructors
  OSDeviceDebug::putString_P(PSTR("delete("));
  //#if (__SIZEOF_POINTER__ == 2)
  OSDeviceDebug::putPtr(p);
  //#endif
  OSDeviceDebug::putChar(')');
  OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
}

#endif /* OS_INCLUDE_NEW_DELETE */

// ----------------------------------------------------------------------------

// Nothing smart to do, normally should never be called, just a reference.
// Without it here we risk the linker will add a lot of useless stuff from
// the standard C++ library.
extern "C" void
__cxa_pure_virtual(void);

void
__cxa_pure_virtual(void)
{
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("Pure Virtual"));
  OSDeviceDebug::putNewLine();
#endif
}
