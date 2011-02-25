/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "portable/stdlib/include/ostream"

OS os;

// low level debug device
OSDeviceDebug debug;

#if defined(OS_INCLUDE_CLOG)
// high level debug stream used to display task messages
ostream clog(&debug);
#endif
