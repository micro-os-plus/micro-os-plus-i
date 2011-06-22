/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CHIPSELECTACTIVELOW)

#include "portable/kernel/include/OS.h"

#include "portable/devices/generic/include/ChipSelectActiveLow.h"

ChipSelectActiveLow::ChipSelectActiveLow(Gpio_t& gpio) :
  m_gpio(gpio)
{
  OSDeviceDebug::putConstructor("ChipSelectActiveLow", this);
}

#endif /* defined(OS_INCLUDE_CHIPSELECTACTIVELOW) */
