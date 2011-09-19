/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSEVENTNOTIFIER)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSEventNotifier.h"

OSEventNotifier::OSEventNotifier()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSEventNotifier"), this);
}

#endif /* defined(OS_INCLUDE_OSEVENTNOTIFIER) */
