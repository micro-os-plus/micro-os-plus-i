/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 * 	Based on open source code with Embedded C++ constraints.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_IOSTREAM) 

#include "portable/devices/debug/include/OSDeviceDebug.h"
#include "portable/stdlib/include/iostream"

namespace std
  {
    iostream::iostream(streambuf* sb) :
    ostream(sb), istream(sb)
      {
        OSDeviceDebug::putConstructor_P(PSTR("iostream"), this);
      }

    iostream::~iostream()
      {
        OSDeviceDebug::putDestructor_P(PSTR("iostream"), this);
      }

  } /* namespace std */

#endif /* defined(OS_INCLUDE_IOSTREAM) */
