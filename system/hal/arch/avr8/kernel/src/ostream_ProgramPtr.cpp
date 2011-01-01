/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSTREAM_PROGRAMPTR) && defined(OS_CONFIG_ARCH_AVR)

#include "hal/arch/avr8/kernel/include/ostream_ProgramPtr.h"

ostream & operator <<(ostream & out, OSProgramPtr_t pc)
  {
    unsigned short w;
    w = ( unsigned short ) pc;
    unsigned char c;

    unsigned char upp;
    upp = 0;

    if (out.flags() & ios_base::showbase)
      {
        out.put( '0');
        if ( (out.flags() & ios_base::uppercase ))
          out.put( 'X');
        else
          out.put( 'x');
      }

    c = ( (w >> 15) & 0x01);
    out.put(c + '0');
    ios_base::fmtflags f = out.flags();
    out.unsetf(ios_base::showbase);
    out << hex << ( unsigned short ) (w << 1);
    out.flags(f);

    return out;
  }

#endif

