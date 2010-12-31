/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef OSTREAM_OSTASK_H_
#define OSTREAM_OSTASK_H_

#include "portable/stdlib/include/ostream"
#include "portable/kernel/include/OS.h"

ostream & operator <<(ostream & out, OSTask& pt);

#endif /*OSTREAM_OSTASK_H_*/
