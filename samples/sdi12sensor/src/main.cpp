/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/uOS.h"

#include "AppSDI12Sensor.h"

// ------------------------------------------------------------------------------

// active object allocated on static storage
// will create two threads, with given names

AppSDI12Sensor appSDI12("dacq", "periodic", "sdi12");

// ------------------------------------------------------------------------------
