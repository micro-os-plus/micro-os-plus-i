/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "AppSDI12Sensor.h"

// ------------------------------------------------------------------------------

// task allocated on static storage
// will create two tasks, with given names

AppSDI12Sensor appSDI12("dacq", "periodic", "sdi12");

// ------------------------------------------------------------------------------

// message to be displayed on the debugging device at startup
#if defined(DEBUG)
const char greeting[]= "AVI SDI12sensor v1.2";
#endif
