/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSTREAM_OSTASK)

#include "portable/kernel/include/ostream_OSTask.h"

ostream & operator <<(ostream & out, OSTask& pt)
  {
    out << dec << pt.getID();
    out << ' '<< showbase << hex << ( unsigned int ) &pt;
    out << ' '<< pt.getName();
    out << ", pri="<< dec << ( unsigned short ) pt.getPriority();
#if defined(OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER)
    out << ", PC="<< pt.getProgramCounter();
#endif
    out << ", stk="<< hex << ( unsigned int ) pt.getStackBottom();
    out << "["<< dec << pt.getStackSize() << "] ";
    out << hex << ( unsigned int) pt.getStack();
    out << "["<< dec << pt.getStackUsed() << "]";

    if (pt.isSuspended() )
      out << ", suspended";

    if (pt.isWaiting() )
      out << ", wait="<< hex << ( unsigned short ) pt.getEvent();

    out << noshowbase;

    return out;
  }

#endif
