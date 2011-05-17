/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#if !defined(OS_CONFIG_CLOCK_DIV_ENUM)

#if (OS_CFGINT_CLOCK_PRESCALER==1)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_1)
#elif (OS_CFGINT_CLOCK_PRESCALER==2)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_2)
#elif (OS_CFGINT_CLOCK_PRESCALER==4)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_4)
#elif (OS_CFGINT_CLOCK_PRESCALER==8)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_8)
#elif (OS_CFGINT_CLOCK_PRESCALER==16)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_16)
#elif (OS_CFGINT_CLOCK_PRESCALER==32)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_32)
#elif (OS_CFGINT_CLOCK_PRESCALER==64)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_64)
#elif (OS_CFGINT_CLOCK_PRESCALER==128)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_128)
#elif (OS_CFGINT_CLOCK_PRESCALER==256)
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_256)
#else
#define OS_CONFIG_CLOCK_DIV_ENUM		(clock_div_1)
#endif

#endif

// ----- init3 ---------------------------------------------------------------
// earliest moment we can use C code to initialise processor,
// before constructors
extern "C" void processor_init(void)
__attribute__((naked, section(".init3")));

void processor_init(void)
  {
    cli();
    // be sure we start with interrupts disabled
    wdt_enable(WDTO_2S);

    OSScheduler::ledActiveInit();
    OSScheduler::ISR_ledActiveOn();

    // prescaller should be configured early
    clock_prescale_set(OS_CONFIG_CLOCK_DIV_ENUM);

#if !defined(OS_EXCLUDE_MULTITASKING)
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
#endif
  }

void
OSImpl::familyEarlyInit(void)
{
#if defined(DEBUG)

  //OSDeviceDebug::putNewLine();
  OSDeviceDebug::putString("CPU=");
#if defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
  OSDeviceDebug::putDec(OS_CFGLONG_CPU_FREQUENCY_HZ);
  OSDeviceDebug::putString(" Hz");
#else
  OSDeviceDebug::putDec(OS_CFGLONG_CPU_FREQUENCY_HZ/1000);
  OSDeviceDebug::putString(" KHz");
#endif /* defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG) */
  OSDeviceDebug::putNewLine();

#endif /* defined(DEBUG) */
}


#endif

