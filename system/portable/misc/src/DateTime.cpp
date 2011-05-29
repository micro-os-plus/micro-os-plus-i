/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DATETIME)

#include "portable/misc/include/DateTime.h"

// ----------------------------------------------------------------------------

DateTime::DateTime()
{
  debug.putConstructor_P(PSTR("DateTime"), this);
}

DateTime::~DateTime()
{
  debug.putDestructor_P(PSTR("DateTime"), this);
}

#endif /* defined(OS_INCLUDE_DATETIME) */
