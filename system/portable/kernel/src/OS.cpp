/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

// static members
unsigned char OS::ms_resetBits;

#if defined(DEBUG) && !defined(OS_EXCLUDE_MULTITASKING)
OS::OS()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OS()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }
#endif

#if defined(OS_INCLUDE_NAKED_INIT)
// ----- init5 ---------------------------------------------------------------

void OS::nakedEarlyInit(void)
  {
    earlyInit();
  }
#endif

void OS::earlyInit(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("OS::earlyInit()"));
    OSDeviceDebug::putNewLine();
#endif
    saveResetBits();
#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("ResetBits="));
    OSDeviceDebug::putHex((unsigned char) OS::getResetBits());
    OSDeviceDebug::putNewLine();
#endif
#if !defined(OS_EXCLUDE_MULTITASKING)
    OSScheduler::earlyInit();
#endif
  }

#if defined(OS_INCLUDE_OS_BUSYWAITMILLIS)

void OS::busyWaitMillis(schedTicks_t n)
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

#endif /*OS_INCLUDE_OS_BUSYWAITMILLIS*/

// -----------------------------------------------------------------------------
// default main
extern "C"
int  main( void ) __attribute__( ( weak ) );

int main()
  {
#if defined(DEBUG)
    OSDeviceDebug::putNewLine();
//#if defined(OS_CONFIG_ARCH_AVR8)
    OSDeviceDebug::putString_P(PSTR("main()"));
//#else
//    OSDeviceDebug::putString("main()");
//#endif
    OSDeviceDebug::putNewLine();
#endif
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
//#if defined(OS_CONFIG_ARCH_AVR8)
    OSDeviceDebug::putString_P(PSTR("delete("));
//#else
//    OSDeviceDebug::putString("delete(");
//#endif
#if (__SIZEOF_POINTER__ == 2)
    OSDeviceDebug::putHex((unsigned short)p);
#endif
    OSDeviceDebug::putChar(')');
    OSDeviceDebug::putNewLine();
#endif
  }

#endif /*OS_INCLUDE_NEW_DELETE*/

