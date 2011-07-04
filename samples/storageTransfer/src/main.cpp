/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

// ----------------------------------------------------------------------------

#include "DataAcquisition.h"
#include "Storage.h"

// ----------------------------------------------------------------------------

DataAcquisition daq(100);
Storage storage;

unsigned long ticks;


#if defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK)


void
OSApplicationImpl::interruptTick(void)
{
  ticks++;
}

#endif /* defined(OS_INCLUDE_OSSAPPLICATIONIMPL_INTERRUPTTICK) */

// ----------------------------------------------------------------------------

