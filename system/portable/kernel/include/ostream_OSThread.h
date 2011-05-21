/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTREAM_OSTHREAD_H_
#define OSTREAM_OSTHREAD_H_

#include "portable/kernel/include/OS.h"

#include "portable/stdlib/include/ostream"

std::ostream& operator <<(std::ostream& out, OSThread& pt);

#endif /* OSTREAM_OSTHREAD_H_ */
