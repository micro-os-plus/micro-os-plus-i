/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTREAM_OSTHREAD)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/ostream_OSThread.h"

std::ostream& operator <<(std::ostream& out, OSThread& pt)
  {
    out << std::dec << pt.getID();
    out << ' ' << std::showbase << std::hex << (unsigned int) &pt;
    out << ' ' << pt.getName();
    out << ", pri="<< std::dec << (unsigned short) pt.getPriority();
#if defined(OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER)
    out << ", PC="<< pt.getProgramCounter();
#endif
    out << ", stk=" << std::hex << (unsigned int) pt.getStackBottom();
    out << "[" << std::dec << pt.getStackSize() << "] ";
    out << std::hex << (unsigned int) pt.getStack();
    out << "[" << std::dec << pt.getStackUsed() << "]";

    if (pt.isSuspended())
    out << ", suspended";

    if (pt.isWaiting())
    out << ", wait=" << std::hex << (unsigned short) pt.getEvent();

    out << std::noshowbase;

    return out;
  }

#endif
