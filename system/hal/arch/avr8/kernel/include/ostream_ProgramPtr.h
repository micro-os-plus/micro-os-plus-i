/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSTREAM_PROGRAMPTR_H_
#define HAL_OSTREAM_PROGRAMPTR_H_

#include "portable/stdlib/include/ostream"
#include "portable/kernel/include/OS.h"

ostream& operator <<(ostream& out, OSProgramPtr_t pc);

#endif /* HAL_OSTREAM_PROGRAMPTR_H_ */
