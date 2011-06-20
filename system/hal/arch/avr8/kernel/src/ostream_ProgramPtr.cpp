/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_INCLUDE_OSTREAM_PROGRAMPTR) && defined(OS_CONFIG_ARCH_AVR8)

#include "hal/arch/avr8/kernel/include/ostream_ProgramPtr.h"

std::ostream& operator <<(std::ostream& out, OSProgramPtr_t pc)
  {
    unsigned short w;
    w = (unsigned short) pc;
    unsigned char c;

    unsigned char upp;
    upp = 0;

    if (out.flags() & std::ios_base::showbase)
      {
        out.put('0');
        if ((out.flags() & std::ios_base::uppercase))
          out.put('X');
        else
          out.put('x');
      }

    c = ((w >> 15) & 0x01);
    out.put(c + '0');
    std::ios_base::fmtflags f = out.flags();
    out.unsetf(std::ios_base::showbase);
    out << std::hex << (unsigned short) (w << 1);
    out.flags(f);

    return out;
  }

#endif

