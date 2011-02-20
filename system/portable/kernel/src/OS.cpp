/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

// static members
OSResetBits_t OS::ms_resetBits;

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)

OS::OS()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OS()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
  }

#endif /* defined(DEBUG) */

void
OS::saveResetBits(void)
{
  ms_resetBits = OSImpl::CPUfetchResetBits();
}


extern "C"
int  main( void ) __attribute__( ( weak ) );

#if !defined(OS_EXCLUDE_RESET_HANDLER)

extern "C" void
os_reset_handler(void) __attribute__( ( naked, noreturn ));

void
os_init_data_and_bss(void) __attribute__((noinline));

void
os_init_static_constructors(void) __attribute__((noinline));


//int xx = 0x12345678;

void
os_reset_handler(void)
{
  // mandatory in the first position, before any C call
  OSImpl::CPUstackInit();
  // from this point on, C calls are available

  // be sure we start with interrupts disabled
  OS::interruptsDisable();

  // should also init GPIO, so that leds will be available
  OSImpl::CPUinit();

  OSScheduler::ledActiveInit();
  OSScheduler::ISR_ledActiveOn();

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
#endif /* defined(DEBUG) */

  OS::earlyInit();

  os_init_static_constructors();

  main(); // call standard main()

  // noreturn
  for (;;)
    ;
}

/* init value for the stack pointer. defined in linker script */
extern unsigned long __stack_end;

/* start address for the initialization values of the .data section.
 defined in linker script */
extern unsigned long __os_data_load_start;

/* start address for the .data section. defined in linker script */
extern unsigned long __os_data_start;

/* end address for the .data section. defined in linker script */
extern unsigned long __os_data_end;

/* start address for the .bss section. defined in linker script */
extern unsigned long __os_bss_start;

/* end address for the .bss section. defined in linker script */
extern unsigned long __os_bss_end;

void os_init_data_and_bss(void)
  {
    unsigned long *pSrc, *pDest;

    /* Copy the data segment initializers from flash to SRAM */
    pSrc = &__os_data_load_start;
    pDest = &__os_data_start;
    for (; pDest < &__os_data_end;)
      {
        *(pDest++) = *(pSrc++);
      }
    /* Zero fill the bss segment. */
    for (pDest = &__os_bss_start; pDest < &__os_bss_end;)
      {
        *(pDest++) = 0;
      }
  }

/* start address for the constructors section. defined in linker script */
extern unsigned long __os_ctors_array_start;

/* end address for the constructors section. defined in linker script */
extern unsigned long __os_ctors_array_end;

typedef void (*pFunc_t)(void);

void os_init_static_constructors(void)
  {
    unsigned long *p;
    void (*pFunc)(void);

    p = &__os_ctors_array_start;
    for (; p < &__os_ctors_array_end; p++)
      {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS) && defined(OS_DEBUG_CONSTRUCTORS_INIT)
        OSDeviceDebug::putString("INIT=");
        OSDeviceDebug::putHex(*p);
        OSDeviceDebug::putNewLine();
#endif
        pFunc = (pFunc_t) (*p);
        (*pFunc)();
      }
}

#endif /* OS_EXCLUDE_RESET_HANDLER */

#if defined(OS_INCLUDE_NAKED_INIT)
// ----- init5 ---------------------------------------------------------------

void OS::nakedEarlyInit(void)
  {
    earlyInit();
  }
#endif

// runs before constructors
void OS::earlyInit(void)
  {
#if defined(DEBUG) && defined(OS_EXCLUDE_MULTITASKING)
    OSDeviceDebug::putString_P(PSTR("Multitasking: disabled"));
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
#if defined(OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR)
    OSDeviceDebug::putString_P(PSTR("SysTick: not naked"));
    OSDeviceDebug::putNewLine();
#endif /* defined(OS_EXCLUDE_OSTIMERTICKS_NAKED_ISR) */

    OSDeviceDebug::putString_P(PSTR("SysTick: "));
    OSDeviceDebug::putDec(OS_CFGINT_TICK_RATE_HZ);
    OSDeviceDebug::putString_P(PSTR(" ticks/sec"));
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) && defined(OS_EXCLUDE_OSTIMER) */

#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("OS::earlyInit()"));
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

    saveResetBits();

#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("ResetBits="));
    OSDeviceDebug::putHex((unsigned char) OS::getResetBits());
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

#if !defined(OS_EXCLUDE_MULTITASKING)
    OSScheduler::earlyInit();
#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
  }

#if defined(OS_INCLUDE_OS_BUSYWAITMILLIS)

void OS::busyWaitMillis(unsigned int n)
  {
    for (; n--;)
      {
        int i;
        // calibrate from OSC
        for (i = (OS_CFGLONG_SYSCLOCK_HZ / OS_CFGINT_BUSYWAIT_CALIBRATION ); i--;)
          {
            NOP();
          }
      }
  }

#endif /* OS_INCLUDE_OS_BUSYWAITMILLIS */

#if defined(OS_INCLUDE_OS_BUSYWAITMICROS)

void
OS::busyWaitMicros(unsigned int n)
{
  int i;
  // calibrate from OSC
  for (i = (OS_CFGLONG_SYSCLOCK_HZ / 1000 * n / OS_CFGINT_BUSYWAIT_CALIBRATION); i--;)
    {
      OSImpl::NOP();
    }
}

#endif /* OS_INCLUDE_OS_BUSYWAITMICROS */

// -----------------------------------------------------------------------------
// default main. may be redefined by application

int main()
  {
#if defined(DEBUG)
    OSDeviceDebug::putNewLine();
    OSDeviceDebug::putString_P(PSTR("main()"));
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */

#if !defined(OS_EXCLUDE_MULTITASKING)
    // tasks were created and registered by class constructors
    OSScheduler::start();
#else
    OS::main();
#endif /*OS_EXCLUDE_MULTITASKING*/

    //no return
  }

// -----------------------------------------------------------------------------

#if defined(OS_INCLUDE_NEW_DELETE)

void *operator
new( unsigned int n )
  {
    return malloc( n );
  }

void operator
delete( void *p )
  {
    return free( p );
  }

#else

void operator
delete(void *p __attribute__( ( unused ) ) )
  {
#if defined(DEBUG)
    ; // dummy, refered by virtual destructors
    OSDeviceDebug::putString_P(PSTR("delete("));
#if (__SIZEOF_POINTER__ == 2)
    OSDeviceDebug::putPtr(p);
#endif
    OSDeviceDebug::putChar(')');
    OSDeviceDebug::putNewLine();
#endif /* defined(DEBUG) */
  }

#endif /*OS_INCLUDE_NEW_DELETE*/

