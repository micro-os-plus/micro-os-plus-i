/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DATETIME_OSTREAM_H_
#define DATETIME_OSTREAM_H_

#include "portable/kernel/include/OS.h"

#include "portable/stdlib/include/ostream"

class DateTime;
class Time;

std::ostream&
operator <<(std::ostream& out, DateTime& dt);

std::ostream&
operator <<(std::ostream& out, Time& tm);

#endif /* DATETIME_OSTREAM_H_ */
