/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_TEMPLATE)

void
OSCPUImpl::earlyInit(void)
{
  // Be sure not to use debug here
  ;
}


void
OSImpl::familyEarlyInit(void)
{
#if defined(DEBUG)

  OSDeviceDebug::putString("DID=");
  //OSDeviceDebug::putHex((unsigned long) __builtin_mfdr(0));
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("UID=");
  //for (int* p = (int*) 0x80800204; p < (int*) 0x80800212; ++p)
  //  OSDeviceDebug::putPtr((void*)*p);
  OSDeviceDebug::putNewLine();

  OSDeviceDebug::putString("CPU=");
  OSDeviceDebug::putDec(OS_CFGLONG_CPU_FREQUENCY_HZ);
  OSDeviceDebug::putString(" Hz");
  OSDeviceDebug::putNewLine();

#endif /* defined(DEBUG) */
}

#endif /* defined(OS_CONFIG_FAMILY_TEMPLATE) */

